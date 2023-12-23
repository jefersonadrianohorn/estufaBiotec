#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

const int portaAnalogica = 32; // Defina a porta analógica que deseja ler
const int numLeituras = 100; // Número de leituras para calcular a média
const float soloSeco = 2558.93; // Média das leituras do solo seco
const float soloUmido = 1136.86; // Média das leituras do solo úmido
const int pinoLED = 2; // Pino do LED
int exaustorPin = 12;
int irrigacaoPin = 13;
int dhtPin = 26;
int dhtType = DHT22;
DHT dht(dhtPin, dhtType);

AsyncWebServer servidorWeb(80);
const int portaGPIO = 25; // Definindo a porta GPIO 25
unsigned long tempoInicio = 0;
bool avisoEnviado = false;
bool estado_porta = LOW; // Variável global para acompanhar o estado da porta


const char* ssid = "Marli"; // Coloque aqui o nome da sua rede Wi-Fi
const char* password = "Chacarabeijaflor@1975"; // Coloque aqui a senha da sua rede Wi-Fi
float umidadeSoloAtual = 0.0; // Variável global para armazenar a umidade do solo atualizada

void configurarWiFi() {
  Serial.println("Conectando à rede Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Tentando conectar à rede Wi-Fi...");
  }

  Serial.println("Conectado à rede Wi-Fi com sucesso!");
  Serial.print("Endereço IP atribuído pelo DHCP: ");
  Serial.println(WiFi.localIP());
}

void configurarServidorWeb() {
  servidorWeb.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    
    String html = "<html>"
                 "<head>"
                 "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"
                 "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                 "<style>"
                 "body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; background-color: #f7f7f7; color: #333; }"
                 "h1 { margin-top: 20px; color: #222; }"
                 ".dashboard { display: flex; flex-direction: column; align-items: center; padding: 20px; }"
                 ".switch-container { display: flex; flex-direction: column; align-items: center; width: 100%; max-width: 500px; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1); }"
                 ".switch { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; width: 100%; }"
                 ".switch-label { flex-grow: 1; margin: 0 10px; font-weight: bold; color: #444; }"
                 ".switch-input { display: none; }"
                 ".switch-slider { position: relative; width: 50px; height: 24px; background-color: #ccc; border-radius: 12px; cursor: pointer; }"
                 ".switch-slider:before { content: ''; position: absolute; top: 2px; left: 2px; width: 20px; height: 20px; background-color: white; border-radius: 50%; transition: 0.4s; }"
                 ".switch-input:checked + .switch-slider:before { transform: translateX(26px); background-color: #00cc66; }"
                 ".switch-slider.round { border-radius: 24px; }"
                 ".switch-slider.round:before { border-radius: 50%; }"
                 ".sensor-container { margin-top: 30px; display: flex; flex-direction: column; align-items: center; }"
                 ".sensor-label { margin-bottom: 5px; font-size: 16px; color: #555; }"
                 ".sensor-value { font-size: 24px; font-weight: bold; color: #222; display: flex; align-items: center; justify-content: center; }"
                 ".sensor-value img { margin-right: 10px; width: 40px; height: 40px; }"
                 ".rotate-animation { animation-name: rotate; animation-duration: 2s; animation-iteration-count: infinite; animation-timing-function: linear; }"
                 ".alert-icon { width: 65px; height: 65px; }"
                 "@keyframes rotate { from { transform: rotate(0deg); } to { transform: rotate(360deg); } }"
                 "@media screen and (max-width: 600px) { .switch-container { width: 90%; padding: 15px; } .switch-label { font-size: 14px; } .sensor-label { font-size: 14px; } .sensor-value { font-size: 20px; } .sensor-value img { width: 30px; height: 30px; margin-right: 0; margin-bottom: 5px; } } }"
                 "</style>"
                 "</head>"
                 "<body>"
                 "<h1>Painel Estufa Biotec</h1>"
                 "<div class='dashboard'>"
                 "<div class='switch-container'>"
                 "<div class='switch'>"
                 "<img src='https://jefersonadrianohorn.github.io/estufaBiotec/helice_rodando.png' width='80' height='80' id='exhaust-image' />"
                 "<span class='switch-label'>Exaustor</span>"
                 "<label class='switch'>"
                 "<input type='checkbox' class='switch-input' id='exhaust-switch' onclick='alternarExaustor()'>"
                 "<span class='switch-slider round'></span>"
                 "</label>"
                 "</div>"
                 "<div class='switch'>"
                 "<img src='https://jefersonadrianohorn.github.io/estufaBiotec/irriga%C3%A7%C3%A3o.png' width='80' height='80' />"
                 "<span class='switch-label'>Irrigação</span>"
                 "<label class='switch'>"
                 "<input type='checkbox' class='switch-input' id='irrigation-switch' onclick='alternarIrrigacao()'>"
                 "<span class='switch-slider round'></span>"
                 "</label>"
                 "</div>"
                 "<div class='sensor-container'>"
                 "<div class='sensor-value' id='temperature-value'>"
                 "<img src='https://jefersonadrianohorn.github.io/estufaBiotec/temperatura.png' alt='Ícone de temperatura'> <span id='temperature'></span>"
                 "</div>"
                 "<div class='sensor-value' id='humidity-value'>"
                 "<img src='https://jefersonadrianohorn.github.io/estufaBiotec/umidade_Ar.png' alt='Ícone de umidade'> <span id='humidity'></span>"
                 "</div>"
                 "<div class='sensor-value' id='soil-moisture-value'>"
                 "<img src='https://jefersonadrianohorn.github.io/estufaBiotec/Umidade_Solo.png' alt='Ícone de umidade do solo'> <span id='soil-moisture'></span>"
                 "</div>"
                 "<div class='sensor-value' id='door-status'>"
                 "Status da Porta: <span id='door-status-value'></span> <img src='https://jefersonadrianohorn.github.io/estufaBiotec/alerta.gif' alt='Ícone de alerta' class='alert-icon' id='door-alert-icon' style='display: none;'>"
                 "</div>"
                 "</div>"
                 "</div>"
                 "</div>"
                 "<script>"
                 "function alternarExaustor() { var xhr = new XMLHttpRequest(); xhr.open('GET', '/alternarExaustor', true); xhr.send(); }"
                 "function alternarIrrigacao() { var xhr = new XMLHttpRequest(); xhr.open('GET', '/alternarIrrigacao', true); xhr.send(); }"
                 "function atualizarEstado() { var xhr = new XMLHttpRequest(); xhr.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { var estado = JSON.parse(this.responseText); document.getElementById('exhaust-switch').checked = estado.estadoExaustor; document.getElementById('irrigation-switch').checked = estado.estadoIrrigacao; document.getElementById('temperature').innerText = estado.temperatura; document.getElementById('humidity').innerText = estado.umidade; document.getElementById('soil-moisture').innerText = estado.umidadeSolo; var exhaustImage = document.getElementById('exhaust-image'); if (estado.estadoExaustor) { exhaustImage.src = 'https://jefersonadrianohorn.github.io/estufaBiotec/helice_rodando.png'; exhaustImage.classList.add('rotate-animation'); } else { exhaustImage.src = 'https://jefersonadrianohorn.github.io/estufaBiotec/helice_rodando.png'; exhaustImage.classList.remove('rotate-animation'); } document.getElementById('door-status-value').innerText = estado.estadoPorta ? 'Porta Aberta' : 'Porta Fechada'; var doorAlertIcon = document.getElementById('door-alert-icon'); doorAlertIcon.style.display = estado.estadoPorta ? 'inline' : 'none'; } }; xhr.open('GET', '/obterEstado', true); xhr.send(); } setInterval(atualizarEstado, 1000);"
                 "</script>"
                 "</body>"
                 "</html>";


    request->send(200, "text/html", html);
  });

  servidorWeb.on("/alternarExaustor", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(exaustorPin, !digitalRead(exaustorPin));
    request->send(200);
  });

  servidorWeb.on("/alternarIrrigacao", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(irrigacaoPin, !digitalRead(irrigacaoPin));
    request->send(200);
  });

  servidorWeb.on("/obterEstado", HTTP_GET, [](AsyncWebServerRequest *request){
  bool estadoExaustor = digitalRead(exaustorPin);
  bool estadoIrrigacao = digitalRead(irrigacaoPin);
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  float umidadeSoloAtualWeb = umidadeSoloAtual; // Obtenha a umidade do solo atualizada para a página web
  int estadoPorta = digitalRead(portaGPIO); // Lê o estado atual da porta

  String estadoJson = "{\"estadoExaustor\":" + String(estadoExaustor) + ",\"estadoIrrigacao\":" + String(estadoIrrigacao) + ",\"temperatura\":" + String(temperatura) + ",\"umidade\":" + String(umidade) + ",\"umidadeSolo\":" + String(umidadeSoloAtualWeb) + ",\"estadoPorta\":" + String(estadoPorta) + "}";
  request->send(200, "application/json", estadoJson);
});


  servidorWeb.begin();
}
void verificarEstadoPorta() {
  int novoEstadoPorta = digitalRead(portaGPIO); // Lê o estado da porta

  if (novoEstadoPorta != estado_porta) {
    tempoInicio = millis(); // Reseta o tempo sempre que o estado da porta mudar
    estado_porta = novoEstadoPorta; // Atualiza o estado da porta
  }

  if (estado_porta == HIGH && millis() - tempoInicio >= 1500 && !avisoEnviado) {
    Serial.println("Estado HIGH detectado por mais de 15 segundos!");
    digitalWrite(pinoLED, HIGH);
    // Outras ações podem ser incluídas aqui, se necessário
    avisoEnviado = true; // Define a flag de aviso para evitar repetição
  }

  if (estado_porta == LOW) {
    avisoEnviado = false; // Reseta a flag de aviso se o estado da porta mudar para LOW
  }
}
void setup() {
  Serial.begin(115200);

  pinMode(exaustorPin, OUTPUT);
  pinMode(irrigacaoPin, OUTPUT);
  dht.begin();

  configurarWiFi();
  configurarServidorWeb();
  
  if (!MDNS.begin("estufa")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  analogReadResolution(12); // Define a resolução da leitura analógica como 12 bits (0-4095)
  pinMode(pinoLED, OUTPUT); // Define o pino do LED como saída
    pinMode(portaGPIO, INPUT_PULLUP); // Ativa o pull-up interno na porta GPIO 25

}
void loop() {
  
   float temperatura = dht.readTemperature();
  
  // Controle do exaustor baseado na temperatura
  if (temperatura > 35.0) { // Modificado para 35 graus Celsius
    digitalWrite(exaustorPin, HIGH); // Liga o exaustor
  } else {
    delay(2500);
    digitalWrite(exaustorPin, LOW); // Desliga o exaustor se a temperatura estiver abaixo de 35 graus Celsius
  }


  // Leitura e controle do sensor de umidade do solo
  int somaLeituras = 0; // Variável para armazenar a soma das leituras

  for (int i = 0; i < numLeituras; i++) {
    int leitura = analogRead(portaAnalogica); // Realiza a leitura analógica
    somaLeituras += leitura; // Adiciona a leitura atual à soma total
    delay(100); // Aguarda um breve intervalo entre as leituras (ajuste conforme necessário)
  }

  float media = somaLeituras / (float)numLeituras; // Calcula a média das leituras

  float umidadePercentual = map(media, soloUmido, soloSeco, 100, 0); // Calcula a porcentagem de umidade do solo

  if (umidadePercentual < 0) {
    digitalWrite(pinoLED, HIGH); // Acende o LED se o valor for negativo (fora do solo)
    Serial.println("Sensor fora do solo!");
  } else {
    digitalWrite(pinoLED, LOW); // Desliga o LED se o valor for positivo (dentro do solo)
    Serial.print("Média das ");
    Serial.print(numLeituras);
    Serial.print(" leituras: ");
    Serial.print(media);
    Serial.print(" | Umidade do solo: ");
    Serial.print(umidadePercentual);
    Serial.println("%");
    umidadeSoloAtual = umidadePercentual; // Armazena o valor da umidade do solo na variável global
  }

  delay(10); // Aguarda um segundo antes da próxima medição (ajuste conforme necessário)

    verificarEstadoPorta(); // Chama a função para verificar o estado da porta

}
