/*This file provides a client connection to the host server.
 *
 *This file was adapted from the 'WiFiClientBasic' file created 
 *by expressify.
 *
 */
#include <WiFi.h>
#include <WiFiMulti.h>
#include<WebServer.h>
#include<AsyncTCP.h>
#include<ESPAsyncWebServer.h>

WiFiMulti WiFiMulti;
AsyncWebServer server(80);

const char* ssid = "Eduroam For Who";//"Eduroam For Who";//"WR7010-2.4G-631";
const char* password ="Whitebread303";// "Whitebread303";//"fuac62756";

/* 
 *  The setup method connects to the network provided. A server is created for further
 *  communication.
 *
 */
void setup() {
  Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to WiFi network... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print("WiFi failed!");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      if(Serial.available()){
      String answer = Serial.readString();
      request->send_P(200, "text/plain", "");
      }
    });

    server.begin();
    delay(500);  
}

/*The loop method connects to the host(teacher's device) and retrieves any information sent 
 *from the host.
 *
 *The client waits for the server data to become available using a delay.
 */

void loop() {
   const uint16_t port = 1337;
    const char * host = "192.168.1.10"; // ip or dns

    Serial.print("Connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        return;
    }

      client.print("GET /index.html HTTP/1.1\n\n");

  int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000)
  {
    maxloops++;
    delay(1); //delay 1 msec
  }
  if (client.available() > 0)
  {
    //read back one line from the server
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
  else
  {
    Serial.println("client.available() timed out ");
  }

    Serial.println("Closing connection.");
    client.stop();

    Serial.println("Waiting 5 seconds before restarting...");
    delay(5000);

}
