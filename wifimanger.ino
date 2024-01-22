#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <cstring>

const char* ssid = "estufa modo reconfiguraçao";
const char* password = "123456789";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(5, INPUT_PULLUP);

  if (digitalRead(5) == LOW) {
    configureAP();
  } else {
    connectToWiFi();
    startWebServer();
  }
}

void loop() {
  // Seu código principal aqui
}

void configureAP() {
  Serial.println("Modo de configuração ativado");
  WiFi.softAP(ssid, password);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
   
   String pagina = R"(<!DOCTYPE html>
<html lang='pt-br'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Configuração de Rede</title>
<style>body {
    font-family: Arial, sans-serif;
    margin: 20px;
    background-color: #f4f4f4;
}

h2 {
    color: #333;
}

form {
    max-width: 400px;
    margin: 0 auto;
    background-color: #fff;
    padding: 20px;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
}

label {
    display: block;
    margin-bottom: 8px;
    color: #333;
}

input {
    width: calc(100% - 16px);
    padding: 8px;
    margin-bottom: 16px;
    box-sizing: border-box;
    border: 1px solid #ccc;
    border-radius: 4px;
}

.error-message {
    color: red;
    margin-bottom: 16px;
}

button {
    background-color: #4CAF50;
    color: white;
    padding: 12px 20px;
    border: none;
    border-radius: 4px;
    cursor: pointer;
    font-size: 16px;
}

/* Estilo do modal */
.modal {
    display: none;
    position: fixed;
    z-index: 1;
    left: 0;
    top: 0;
    width: 100%;
    height: 100%;
    overflow: auto;
    background-color: rgba(0, 0, 0, 0.5);
    padding-top: 60px;
}

.modal-content {
    background-color: #fefefe;
    margin: 5% auto;
    padding: 20px;
    border: 1px solid #888;
    width: 80%;
}

.close {/
    color: #aaa;
    float: right;
    font-size: 28px;
    font-weight: bold;
}

.close:hover,
.close:focus {
    color: black;
    text-decoration: none;
    cursor: pointer;
}

/* Estilo do loader */
#wifi-loader {
    --background: #62abff;
    --front-color: #000;
    --back-color: #c3c8de;
    --text-color: #414856;
    width: 64px;
    height: 64px;
    border-radius: 50%;
    position: fixed;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    display: none;
    justify-content: center;
    align-items: center;
}

#wifi-loader svg {
    position: absolute;
    display: flex;
    justify-content: center;
    align-items: center;
}

#wifi-loader svg circle {
    position: absolute;
    fill: none;
    stroke-width: 6px;
    stroke-linecap: round;
    stroke-linejoin: round;
    transform: rotate(-100deg);
    transform-origin: center;
}

#wifi-loader svg circle.back {
    stroke: var(--back-color);
}

#wifi-loader svg circle.front {
    stroke: var(--front-color);
}

#wifi-loader svg.circle-outer {
    height: 86px;
    width: 86px;
}

#wifi-loader svg.circle-outer circle {
    stroke-dasharray: 62.75 188.25;
}

#wifi-loader svg.circle-outer circle.back {
    animation: circle-outer135 1.8s ease infinite 0.3s;
}

#wifi-loader svg.circle-outer circle.front {
    animation: circle-outer135 1.8s ease infinite 0.15s;
}

#wifi-loader svg.circle-middle {
    height: 60px;
    width: 60px;
}

#wifi-loader svg.circle-middle circle {
    stroke-dasharray: 42.5 127.5;
}

#wifi-loader svg.circle-middle circle.back {
    animation: circle-middle6123 1.8s ease infinite 0.25s;
}

#wifi-loader svg.circle-middle circle.front {
    animation: circle-middle6123 1.8s ease infinite 0.1s;
}

#wifi-loader svg.circle-inner {
    height: 34px;
    width: 34px;
}

#wifi-loader svg.circle-inner circle {
    stroke-dasharray: 22 66;
}

#wifi-loader svg.circle-inner circle.back {
    animation: circle-inner162 1.8s ease infinite 0.2s;
}

#wifi-loader svg.circle-inner circle.front {
    animation: circle-inner162 1.8s ease infinite 0.05s;
}

#wifi-loader .text {
    position: absolute;
    bottom: -40px;
    display: flex;
    justify-content: center;
    align-items: center;
    text-transform: lowercase;
    font-weight: 500;
    font-size: 14px;
    letter-spacing: 0.2px;
}

#wifi-loader .text::before,
#wifi-loader .text::after {
    content: attr(data-text);
}

#wifi-loader .text::before {
    color: var(--text-color);
}

.success-message p {
    color: green;
    text-align: center;
    font-weight: bold;
}


#wifi-loader .text::after {
    color: var(--front-color);
    animation: text-animation76 3.6s ease infinite;
    position: absolute;
    left: 0;
}

@keyframes circle-outer135 {
    0% {
        stroke-dashoffset: 25;
    }

    25% {
        stroke-dashoffset: 0;
    }

    65% {
        stroke-dashoffset: 301;
    }

    80% {
        stroke-dashoffset: 276;
    }

    100% {
        stroke-dashoffset: 276;
    }
}

@keyframes circle-middle6123 {
    0% {
        stroke-dashoffset: 17;
    }

    25% {
        stroke-dashoffset: 0;
    }

    65% {
        stroke-dashoffset: 204;
    }

    80% {
        stroke-dashoffset: 187;
    }

    100% {
        stroke-dashoffset: 187;
    }
}

@keyframes circle-inner162 {
    0% {
        stroke-dashoffset: 9;
    }

    25% {
        stroke-dashoffset: 0;
    }

    65% {
        stroke-dashoffset: 106;
    }

    80% {
        stroke-dashoffset: 97;
    }

    100% {
        stroke-dashoffset: 97;
    }
}

@keyframes text-animation76 {
    0% {
        clip-path: inset(0 100% 0 0);
    }

    50% {
        clip-path: inset(0);
    }

    100% {
        clip-path: inset(0 0 0 100%);
    }
}



/**
 * Extracted from: SweetAlert
 * Modified by: Istiak Tridip
 */
 .success-checkmark {
    width: 80px;
    height: 115px;
    margin: 0 auto;
    
    .check-icon {
        width: 80px;
        height: 80px;
        position: relative;
        border-radius: 50%;
        box-sizing: content-box;
        border: 4px solid #4CAF50;
        
        &::before {
            top: 3px;
            left: -2px;
            width: 30px;
            transform-origin: 100% 50%;
            border-radius: 100px 0 0 100px;
        }
        
        &::after {
            top: 0;
            left: 30px;
            width: 60px;
            transform-origin: 0 50%;
            border-radius: 0 100px 100px 0;
            animation: rotate-circle 4.25s ease-in;
        }
        
        &::before, &::after {
            content: '';
            height: 100px;
            position: absolute;
            background: #FFFFFF;
            transform: rotate(-45deg);
        }
        
        .icon-line {
            height: 5px;
            background-color: #4CAF50;
            display: block;
            border-radius: 2px;
            position: absolute;
            z-index: 10;
            
            &.line-tip {
                top: 46px;
                left: 14px;
                width: 25px;
                transform: rotate(45deg);
                animation: icon-line-tip 0.75s;
            }
            
            &.line-long {
                top: 38px;
                right: 8px;
                width: 47px;
                transform: rotate(-45deg);
                animation: icon-line-long 0.75s;
            }
        }
        
        .icon-circle {
            top: -4px;
            left: -4px;
            z-index: 10;
            width: 80px;
            height: 80px;
            border-radius: 50%;
            position: absolute;
            box-sizing: content-box;
            border: 4px solid rgba(76, 175, 80, .5);
        }
        
        .icon-fix {
            top: 8px;
            width: 5px;
            left: 26px;
            z-index: 1;
            height: 85px;
            position: absolute;
            transform: rotate(-45deg);
            background-color: #FFFFFF;
        }
    }
}

@keyframes rotate-circle {
    0% {
        transform: rotate(-45deg);
    }
    5% {
        transform: rotate(-45deg);
    }
    12% {
        transform: rotate(-405deg);
    }
    100% {
        transform: rotate(-405deg);
    }
}

@keyframes icon-line-tip {
    0% {
        width: 0;
        left: 1px;
        top: 19px;
    }
    54% {
        width: 0;
        left: 1px;
        top: 19px;
    }
    70% {
        width: 50px;
        left: -8px;
        top: 37px;
    }
    84% {
        width: 17px;
        left: 21px;
        top: 48px;
    }
    100% {
        width: 25px;
        left: 14px;
        top: 45px;
    }
}

@keyframes icon-line-long {
    0% {
        width: 0;
        right: 46px;
        top: 54px;
    }
    65% {
        width: 0;
        right: 46px;
        top: 54px;
    }
    84% {
        width: 55px;
        right: 0px;
        top: 35px;
    }
    100% {
        width: 47px;
        right: 8px;
        top: 38px;
    }
}
</style></head>
<body>

    <h2 style='text-align: center;'>Configuração de Rede BIOTEC</h2>


    <form id='networkForm' method='post' action='#'>
        <label for='ssid'>SSID da Rede:</label>
        <input type='text' id='ssid' name='ssid' required>

        <label for='senha'>Senha da Rede:</label>
        <input type='password' id='senha' name='senha' required>

        <div id='errorMessage' class='error-message'></div>

        <button type='button' onclick='validarSenha()' style='background-color: #4CAF50; color: white; padding: 12px 20px; border: none; border-radius: 4px; cursor: pointer; font-size: 16px; display: block; margin: 0 auto;'>Configurar Rede</button>

        <!-- Modal -->
        <div id='myModal' class='modal'>
            <div class='modal-content'>
                <span class='close' onclick='fecharModal()'>&times;</span>
                <p>SSId: <span id='modalSSID'></span></p>
                <p>Senha: <span id='modalSenha'></span></p>
                <button onclick='editarFormulario()'>Editar</button>
                <button type='button' onclick='configurarRede()'>Enviar</button>

                <!-- Nova div para exibir mensagem de erro no modal -->
                <div id='errorMessageModal' class='error-message' style='color: red; margin-top: 10px;'></div>
            </div>
        </div>

        <!-- Loader -->
        <div id='wifi-loader'>
            <svg viewBox='0 0 86 86' class='circle-outer'>
                <circle r='40' cy='43' cx='43' class='back'></circle>
                <circle r='40' cy='43' cx='43' class='front'></circle>
                <circle r='40' cy='43' cx='43' class='new'></circle>
            </svg>
            <svg viewBox='0 0 60 60' class='circle-middle'>
                <circle r='27' cy='30' cx='30' class='back'></circle>
                <circle r='27' cy='30' cx='30' class='front'></circle>
            </svg>
            <svg viewBox='0 0 34 34' class='circle-inner'>
                <circle r='14' cy='17' cx='17' class='back'></circle>
                <circle r='14' cy='17' cx='17' class='front'></circle>
            </svg>
            <div data-text='Enviando' class='text'></div>
        </div>
<!-- Success Message -->
<div id='successMessage' class='success-message' style='display: none;'>
    
    <div class='success-checkmark'>
        <div class='check-icon'>
          <span class='icon-line line-tip'></span>
          <span class='icon-line line-long'></span>
          <div class='icon-circle'></div>
          <div class='icon-fix'></div>
        </div>
        
      </div>
      <p>Configuração enviada com sucesso!</p>
</div>

    </form>

    <script>function validarSenha() {
        var senha = document.getElementById('senha').value;
        var errorMessage = document.getElementById('errorMessage');
    
        if (senha.length < 8) {
            errorMessage.innerText = 'A senha deve ter no mínimo 8 caracteres.';
            // Não exibir o modal se a senha for inválida
            return;
        } else {
            errorMessage.innerText = ''; // Limpar mensagem de erro
            exibirModal();
        }
    }
    

    function exibirModal() {
        var ssid = document.getElementById('ssid').value;
        var senha = document.getElementById('senha').value;
    
        document.getElementById('modalSSID').innerText = ssid;
        document.getElementById('modalSenha').innerText = senha;
    
        // Exibir o modal
        document.getElementById('myModal').style.display = 'block';
    }
    
    function fecharModal() {
        // Fechar o modal
        document.getElementById('myModal').style.display = 'none';
    }
    
    function editarFormulario() {
        // Fechar o modal
        fecharModal();
    }
    
    function exibirLoader() {
    // Esconder o modal atual
    fecharModal();

    // Exibir o loader
    document.getElementById('wifi-loader').style.display = 'flex';

    // Simular envio (substitua pelo código real de envio)
    setTimeout(function () {
        // Após o envio (simulado), esconder o loader
        document.getElementById('wifi-loader').style.display = 'none';

        // Exibir a mensagem de sucesso
        document.getElementById('successMessage').style.display = 'block';

        // Adicionar redirecionamento após 5 segundos
        setTimeout(function () {
            window.location.href = '/reiniciar';
        }, 5000); // Redirecionar após 5 segundos
    }, 3000); // Simulando um envio de 3 segundos
}

function configurarRede() {
    var ssid = document.getElementById('ssid').value;
    var senha = document.getElementById('senha').value;
    var errorMessage = document.getElementById('errorMessage');

    if (senha.length < 8) {
        errorMessage.innerText = 'A senha deve ter no mínimo 8 caracteres.';
        return;
    }

    // Substitua a URL abaixo pelo seu endpoint real
    var endpoint = '/save';

    // Exibir o loader
    exibirLoader();

    // Simulação de uma solicitação POST usando o Fetch API
    fetch(endpoint, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'ssid=' + encodeURIComponent(ssid) + '&password=' + encodeURIComponent(senha),
    })
    .then(response => response.json())
    .then(data => {
        // Fechar o modal após o envio bem-sucedido
        fecharModal();
    })
    .catch(error => {
        console.error('Erro ao enviar a solicitação:', error);
    });
}

    </script>

</body>
</html>
)";

    request->send(200, "text/html", pagina);
});





  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
    String newSSID = request->arg("ssid");
    String newPassword = request->arg("password");

    // Salvar as credenciais
    saveCredentials(newSSID.c_str(), newPassword.c_str());

    // Enviar uma resposta de sucesso
    request->send(200, "text/plain", "Credenciais salvas com sucesso. Reiniciando...");

    // Log dos dados recebidos
    Serial.print("SSID recebido: ");
    Serial.println(newSSID);
    Serial.print("Senha recebida: ");
    Serial.println(newPassword);

    // Aguardar um curto período para garantir que a resposta seja enviada
    delay(1000);

    // Reiniciar o dispositivo
    ESP.restart();
});


  server.begin();
}

void saveCredentials(const char* newSSID, const char* newPassword) {
  EEPROM.begin(512);

  EEPROM.writeString(0, newSSID);
  EEPROM.writeString(strlen(newSSID) + 1, newPassword);

  EEPROM.commit();
  EEPROM.end();
}

void connectToWiFi() {
  EEPROM.begin(512);

  String savedSSID = EEPROM.readString(0);
  String savedPassword = EEPROM.readString(strlen(savedSSID.c_str()) + 1);

  WiFi.begin(savedSSID.c_str(), savedPassword.c_str());

  Serial.println("Conectando à rede WiFi...");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.println("Ainda não conectado...");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Falha ao se reconectar. Modo de configuração ativado.");
    configureAP();
  } else {
    Serial.println("Conectado à rede WiFi");
    Serial.println("Endereço IP: " + WiFi.localIP().toString());
  }

  EEPROM.end();
}

void startWebServer() {
  // Adicione aqui as configurações do seu servidor web após a conexão WiFi ser estabelecida
  // Exemplo: server.on("/", HTTP_GET, handleRoot);
}
