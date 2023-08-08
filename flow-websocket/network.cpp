#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer.git
#include <WebSocketsServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <arduino.h>
#include "variables.h"

// Replace with your network credentials
extern const char* hostname = "flow";
extern const char* ssid = "flow1";
extern const char* password = "Name1234";

const unsigned long webUpdateInterval = 50; //Basic update interval for webpage data
unsigned long webPreviousMillis = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Json stuff for sending messages to websockets for UI/debugging
StaticJsonDocument<200> doc_tx;
StaticJsonDocument<200> doc_rx;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>FlowSensorOutput</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">

  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onopen = function(e) {console.log("[socket] socket.onopen "); };
      Socket.onerror = function(e) {  console.log("[socket] socket.onerror "); };
      Socket.onmessage = function(event) {  
        processCommand(event);
      };
    }

    // Function to process commands recieved by the client from the ucontroller
    function processCommand(event){
      var obj = JSON.parse(event.data);
      document.getElementById('deltaX').innerHTML = obj.deltaX;
      document.getElementById('deltaY').innerHTML = obj.deltaY;
      document.getElementById('sumX').innerHTML = obj.sumX;
      document.getElementById('sumY').innerHTML = obj.sumY;
      document.getElementById('distance').innerHTML = obj.distance;
      document.getElementById('button').addEventListener('click', posReset);
      console.log(obj);
      console.log("[socket] " + event.data);
      //document.getElementById("my_value").innerHTML = obj.value;
    }

    window.onload = function(event){
      init();
    }

//    function sendText(){
//      Socket.send(document.getElementById("txBar").value);
//      document.getElementById("txBar").value = "";
//    }
//    function sendBrightness(){
//      Socket.send("#"+document.getElementById("brightness").value);
//    } 
   
  </script>
</head>
<body style="max-width:400px;margin: auto;font-family:Arial, Helvetica, sans-serif;text-align:center; padding-top:50px">
  <h2>Stupid Flow Sensor</h2>
  <div> deltaX: <span id='deltaX'>-</span></div>
  <div> deltaY: <span id='deltaY'>-</span></div>
  <div> SumX: <span id='sumX'>-</span></div>
  <div> SumY: <span id='sumY'>-</span></div>
  <div> VDistance: <span id='distance'>-</span></div>
  <div><button id="button" class="button">Reset</button></p>
</body>
</html>
)rawliteral";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        webSocket.sendTXT(num, "0");
      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] got Text: %s\n", num, payload);
       // try to decipher the JSON string received
      DeserializationError error = deserializeJson(doc_rx, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      break;
      // else {

      // }
  }
}

// response for 404 error
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void initWifi(){
  // very useful https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
  WiFi.softAP(ssid, password);
//  if (WIFI_TYPE == 1){
//    // For just connecting to a known hotspot
//    WiFi.mode(WIFI_STA);                      // instantiate wifi in station mode
//    WiFi.begin(ssid, password);               // make sure this is set in the config when you set the WIFI_TYPE
//    Serial.println("Connecting to WiFi..");
//    while (WiFi.status() != WL_CONNECTED) {
//      delay(1000);
//      Serial.print(".");                      // Wait for wifi connection not sure how it would timeout...
//    }
//  } else if (WIFI_TYPE == 2) {
//    // For rocking your own hotspot because rural
//    int channel = 6;        // Wi-Fi channel number (1-13)
//    int ssid_hidden = 0;    // (0 = broadcast SSID, 1 = hide SSID)
//    int max_connection = 4; //maximum simultaneous connected clients (1-4)
//   
//    WiFi.mode(WIFI_AP);
//    WiFi.softAP(ssid, password, channel, ssid_hidden, max_connection);
//  } else if (WIFI_TYPE == 3){
//    WiFi.mode(WIFI_AP_STA); 
//  } else { // Just turn wifi off
//    Serial.println("Continuing without configuring wifi");
//  }
}

void startNetwork(){

  // Setup OTA updates
  //setupOTA("TemplateSketch", ssid, password); // Not currently used due to sketch size restrictions

  // setup Wi-Fi via function call ^
  initWifi();

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send_P(200, "text/html", index_html);
  });

  // Route for 404 error
  server.onNotFound(notFound);

  // begin the http server
  server.begin();

  // begin the websocket server
  webSocket.begin();

  // setup the callback for webSocket events 
  webSocket.onEvent(webSocketEvent);
}

void doNetworking(){
  // spin the websocket
  webSocket.loop();

  // do we need to send a message...
  unsigned long currentMillis = millis();
  if (currentMillis - webPreviousMillis >= webUpdateInterval){
    // Make and send a json object
    String jsonString="";
    JsonObject object = doc_tx.to<JsonObject>();
    object["deltaX"] = deltaX;
    object["deltaY"] = deltaY;
    object["sumX"] = sumX;
    object["sumY"] = sumY;
    object["distance"] = distance;

    serializeJson(doc_tx, jsonString);
    webSocket.broadcastTXT(jsonString);
    Serial.println(jsonString);
    webPreviousMillis = currentMillis;
    
  }
}


 
