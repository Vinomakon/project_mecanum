//==================================
// ESP32 Acces point with Websockets
// By: Shawn Hymel
// Credit: https://shawnhymel.com/1882/how-to-create-a-web-server-with-websockets-using-an-esp32-in-arduino/
//===================================

#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>


// Constants
const char *ssid = "ESP32-AP";
const char *password =  "12345678";
char *received_states;
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;

// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int lu_state = 0;
int ru_state = 0;
int ld_state = 0;
int rd_state = 0;
int state_update = false;

int lu_forward = 22;
int lu_backward = 23;
int ru_forward = 32;
int ru_backward = 14;
int ld_forward = 17;
int ld_backward = 21;
int rd_forward = 19;
int rd_backward = 16;

/***********************************************************
 * Functions
 */

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch(type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;

    // Handle text messages from client
    case WStype_TEXT:
      received_states = (char*)payload;

      if(strcmp((char *)payload, "getState") == 0) {
        sprintf(msg_buf, "%d%d%d%d", lu_state, ru_state, ld_state, rd_state);
        Serial.printf("Sending to [%u]: %s\n\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);
      } else {

        switch(received_states[0]) {
          case 49:
            lu_state = 1;
            break;
          case 50:
            lu_state = 2;
            break;
          case 48:
            lu_state = 0;
            break;
          default:
            Serial.printf("\nERROR with states\n");
            break;
        }

        switch(received_states[1]) {
          case 49:
            ru_state = 1;
            break;
          case 50:
            ru_state = 2;
            break;
          case 48:
            ru_state = 0;
            break;
          default:
            Serial.printf("\nERROR with states\n");
            break;
        }

        switch(received_states[2]) {
          case 49:
            ld_state = 1;
            break;
          case 50:
            ld_state = 2;
            break;
          case 48:
            ld_state = 0;
            break;
          default:
            Serial.printf("\nERROR with states\n");
            break;
        }

        switch(received_states[3]) {
          case 49:
            rd_state = 1;
            break;
          case 50:
            rd_state = 2;
            break;
          case 48:
            rd_state = 0;
            break;
          default:
            Serial.printf("\nERROR with states\n");
            break;
        }
        
        state_update = true;
      }
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}

void onScriptRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/script.js", "text/js");
}

void onCarPictureRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/car_picture.png", "image/png");
}

void onNeutralStateRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/neutral.png", "image/png");
}

void onUpStateRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/up.png", "image/png");
}

void onDownStateRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/down.png", "image/png");
}

void onForwardRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/forward.png", "image/png");
}

void onBackwardRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/backward.png", "image/png");
}

void onRightRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/right.png", "image/png");
}

void onLeftRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/left.png", "image/png");
}

void onDLURequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/diagonal_lu.png", "image/png");
}

void onDRURequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/diagonal_ru.png", "image/png");
}

void onDLDRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/diagonal_ld.png", "image/png");
}

void onDRDRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/diagonal_rd.png", "image/png");
}

void onRCWRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/rotate_cw.png", "image/png");
}

void onRCCWRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/rotate_ccw.png", "image/png");
}

void onStopRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/stop.png", "image/png");
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

/***********************************************************
 * Main
 */

void setup() {
  // Init LED and turn off
  pinMode(lu_forward, OUTPUT);
  pinMode(lu_backward, OUTPUT);
  pinMode(ru_forward, OUTPUT);
  pinMode(ru_backward, OUTPUT);
  pinMode(ld_forward, OUTPUT);
  pinMode(ld_backward, OUTPUT);
  pinMode(rd_forward, OUTPUT);
  pinMode(rd_backward, OUTPUT);
  // Start Serial port
  Serial.begin(115200);

  // Make sure we can read the file system
  if( !SPIFFS.begin()){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }

  // Start access point
  WiFi.softAP(ssid, password);

  // Print our IP address
  Serial.println();
  Serial.println("AP running");
  Serial.print("My IP address: ");
  Serial.println(WiFi.softAPIP());

  // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);

  // On HTTP request for style sheet, provide style.css
  server.on("/style.css", HTTP_GET, onCSSRequest);
  server.on("/script.js", HTTP_GET, onScriptRequest);

  server.on("/car_picture.png", HTTP_GET, onCarPictureRequest);
  server.on("/neutral.png", HTTP_GET, onNeutralStateRequest);
  server.on("/up.png", HTTP_GET, onUpStateRequest);
  server.on("/down.png", HTTP_GET, onDownStateRequest);
  
  server.on("/forward.png", HTTP_GET, onForwardRequest);
  server.on("/backward.png", HTTP_GET, onBackwardRequest);
  server.on("/right.png", HTTP_GET, onRightRequest);
  server.on("/left.png", HTTP_GET, onLeftRequest);
  server.on("/diagonal_lu.png", HTTP_GET, onDLURequest);
  server.on("/diagonal_ru.png", HTTP_GET, onDRURequest);
  server.on("/diagonal_ld.png", HTTP_GET, onDLDRequest);
  server.on("/diagonal_rd.png", HTTP_GET, onDRDRequest);
  server.on("/rotate_cw.png", HTTP_GET, onRCWRequest);
  server.on("/rotate_ccw.png", HTTP_GET, onRCCWRequest);
  server.on("/stop.png", HTTP_GET, onStopRequest);

  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);

  // Start web server
  server.begin();

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  
}
void loop() {

  if (state_update){

    digitalWrite(lu_forward, LOW);
    digitalWrite(lu_backward, LOW);
    digitalWrite(ru_forward, LOW);
    digitalWrite(ru_backward, LOW);
    digitalWrite(ld_forward, LOW);
    digitalWrite(ld_backward, LOW);
    digitalWrite(rd_forward, LOW);
    digitalWrite(rd_backward, LOW);
    
    switch(lu_state) {
      case 1:
        digitalWrite(lu_forward, HIGH);
        Serial.printf("LU is on");
        break;
      case 2:
        digitalWrite(lu_backward, HIGH);
        break;
      case 0:
        digitalWrite(lu_forward, LOW);
        digitalWrite(lu_backward, LOW);
        Serial.printf("LU is off");
        break;
      default:
        break;
    }

    switch(ru_state) {
      case 1:
        digitalWrite(ru_forward, HIGH);
        break;
      case 2:
        digitalWrite(ru_backward, HIGH);
        break;
      case 0:
        digitalWrite(ru_forward, LOW);
        digitalWrite(ru_backward, LOW);
        break;
      default:
        break;
    }

    switch(ld_state) {
      case 1:
        digitalWrite(ld_forward, HIGH);
        break;
      case 2:
        digitalWrite(ld_backward, HIGH);
        break;
      case 0:
        digitalWrite(ld_forward, LOW);
        digitalWrite(ld_backward, LOW);
        break;
      default:
        break;
    }

    switch(rd_state) {
      case 1:
        digitalWrite(rd_forward, HIGH);
        break;
      case 2:
        digitalWrite(rd_backward, HIGH);
        break;
      case 0:
        digitalWrite(rd_forward, LOW);
        digitalWrite(rd_backward, LOW);
        break;
      default:
        break;
    }
    state_update = false;
  }
  // Look for and handle WebSocket data
  webSocket.loop();
}
