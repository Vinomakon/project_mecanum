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

int lu_forward = 19;
int lu_backward = 16;
int ru_forward = 21;
int ru_backward = 17;
int ld_forward = 32;
int ld_backward = 14;
int rd_forward = 23;
int rd_backward = 22;

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
  request->send(SPIFFS, "/style.css", "text/js");
}

void onPictureRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/car_picture.png", "image/png");
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

  server.on("/car_picture.png", HTTP_GET, onPictureRequest);

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
