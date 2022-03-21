//==================================
// ESP32 Acces point with Websockets
// By: Shawn Hymel
// Credit: https://shawnhymel.com/1882/how-to-create-a-web-server-with-websockets-using-an-esp32-in-arduino/
//===================================

#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <Stepper.h>

#define STEPS (4)

// Constants
const char *ssid = "ESP32-AP";
const char *password =  "12345678";
char *received_states;
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
Stepper lu_stepper(STEPS, 12, 27, 33, 15);
Stepper ru_stepper(STEPS, 32, 14, 22, 23);
int stepper_speed = 4000;

// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int lu_stepper_state = 0;
int ru_stepper_state = 0;
int ld_stepper_state = 0;
int rd_stepper_state = 0;

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
      // Print out raw message
      Serial.println("%s\n",(char*)payload);
      Serial.println("First toggle_state is: %d\n", received_states[0]);

      if(strcmp((char *)payload, "getState") == 0) {
        sprintf(msg_buf, "%d%d%d%d", lu_stepper_state, ru_stepper_state, ld_stepper_state, rd_stepper_state)
      } else {

        if(received_states[0] == 49) {
          lu_stepper_state = 1;
        } else {
          lu_stepper_state = 0;
        }
        
        if(received_states)[1] == 49) {
          ru_stepper_state = 1;
        } else {
          ru_stepper_state = 0;
        }

        if(received_states)[2] == 49) {
          ld_stepper_state = 1;
        } else {
          ld_stepper_state = 0;
        }

        if(received_states)[1] == 49) {
          rd_stepper_state = 1;
        } else {
          rd_stepper_state = 0;
        }
        
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
  lu_stepper.setSpeed(stepper_speed);
  ru_stepper.setSpeed(stepper_speed);

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

  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);

  // Start web server
  server.begin();

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  
}

void loop() {

  if(lu_stepper_state) {
    lu_stepper.step(STEPS);
  }
  if(ru_stepper_state) {
    ru_stepper.step(STEPS);
  }
  // Look for and handle WebSocket data
  webSocket.loop();
}
