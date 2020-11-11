/*File containing the overal student device
 * 
 * Includes communcation and keypad
 * 
 * Display is an individual subsystem but was not tested
 * 
 * Created by Suvana
 * Adapted from the ESP32 libraries by Expressify
 * 
 */
#include <WiFi.h>
#include <WiFiMulti.h>
#include<WebServer.h>
#include<AsyncTCP.h>
#include<ESPAsyncWebServer.h>

WiFiMulti WiFiMulti;
AsyncWebServer server(80);

#include <Keypad.h>
#include <ctype.h>

 /*****************************************COMMUNICATION CODE START********************************/
const char* ssid = "Eduroam For Who";//"Eduroam For Who";//"WR7010-2.4G-631";
const char* password ="Whitebread303";// "Whitebread303";//"fuac62756";

 /*****************************************COMMUNICATION CODE END********************************/
  
 /*****************************************KEYPAD CODE START********************************/


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

/*char first[ROWS][COLS] = {
    { 'a','d','g','1' },
    { 'j','m','p' ,'4'},
    { 's','v','y','7'},
    { ' ','.','#',',' }
};

char second[ROWS][COLS] = {
    { 'b','e','h','2' },
    { 'k','n','q' ,'5'},
    { 't','w','z','8'},
    { ' ','.','#',',' }
};

char third[ROWS][COLS] = {
    { 'c','f','i','3' },
    { 'l','o','r' ,'6'},
    { 'u','x','0','9'},
    { ' ','.','#',',' }
};*/

char numberKeys[ROWS][COLS] = {
    { '1','2','3','A' },
    { '4','5','6','B' },
    { '7','8','9','X' },
    { ' ','0','#','Y' }
};

int alpha = 0;  

byte rowPins[ROWS] = {0, 4, 3, 22};   //Change according to the pins you use
byte colPins[COLS] = {5, 18, 32, 19}; //Change according to the pins you use

//creates the matrix used to read the pins
Keypad numpad( makeKeymap(numberKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) ); 
/*Keypad firstpad( makeKeymap(first), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );
Keypad secondpad( makeKeymap(second), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );
Keypad thirdpad( makeKeymap(third), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );*/

unsigned long startTime;
char key;
/*****************************************KEYPAD CODE END********************************/

void setup() {
    Serial.begin(115200);
     /*****************************************COMMUNICATION CODE START********************************/
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
 /*****************************************COMMUNICATION CODE END********************************/

  /*****************************************KEYPAD CODE START********************************/
  /*firstpad.begin( makeKeymap(first) );
  secondpad.begin( makeKeymap(second) );
  thirdpad.begin( makeKeymap(third) );*/

  numpad.addEventListener(keypadEvent_num);  // Add an event listener.
  numpad.setHoldTime(500);
 /* firstpad.addEventListener(keypadEvent_first);  // Add an event listener.
  firstpad.setHoldTime(500);                   // Default is 1000mS

  secondpad.addEventListener(keypadEvent_second);  // Add an event listener.
  secondpad.setHoldTime(500);                   // Default is 1000mS
  thirdpad.addEventListener(keypadEvent_third);  // Add an event listener.
  thirdpad.setHoldTime(500);*/
    /*****************************************KEYPAD CODE END********************************/

}

void loop() {

   /*****************************************COMMUNICATION CODE START********************************/
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
 /*****************************************COMMUNICATION CODE END********************************/
  /*****************************************KEYPAD CODE START********************************/
    key = numpad.getKey();
  /*if(millis()-startTime>200){
      
   }

  if(alpha == 0){
    key = firstpad.getKey();
    alpha=0;
  }else if(alpha == 1){
    key = secondpad.getKey(); 
    alpha=0;  
  }else if(alpha == 2){
    key = thirdpad.getKey();
    alpha=0;
  }*/
 /*****************************************KEYPAD CODE END********************************/


}

 /*****************************************KEYPAD CODE START********************************/
 static char virtKey = NO_KEY;      // Stores the last virtual key press. (Alpha keys only)
static char physKey = NO_KEY;      // Stores the last physical key press. (Alpha keys only)
static char buildStr[12];
static byte buildCount;
static byte pressCount;

static byte kpadState;

// Take care of some special events.

/*void keypadEvent_first(KeypadEvent key) {
    // in here when in alpha mode.
    kpadState = firstpad.getState( );
    swOnState( key );
} // end ltrs keypad events

void keypadEvent_second(KeypadEvent key) {
    // in here when in alpha mode.
    kpadState = secondpad.getState( );
    swOnState( key );
} // end ltrs keypad events

void keypadEvent_third(KeypadEvent key) {
    // in here when in alpha mode.
    kpadState = secondpad.getState( );
    swOnState( key );
} // end ltrs keypad events*/


void keypadEvent_num( KeypadEvent key ) {
    // in here when using number keypad
    kpadState = numpad.getState( );
    swOnState( key );
} // end numbers keypad events 

void swOnState( char key ) {
    switch( kpadState ) {
        case PRESSED:
            if (millis()-startTime>200) {              // This is a letter key so we're using the letter keymap.
                if (physKey != key) {        // New key so start with the first of 3 characters.
                    pressCount = 0;
                    virtKey = key;
                    physKey = key;
                }
                else {                       // Pressed the same key again...
                    virtKey++;                   // so select the next character on that key.
                    pressCount++;                // Tracks how many times we press the same key.
                }
                    if (pressCount > 2) {    // Last character reached so cycle back to start.
                        pressCount = 0;
                        virtKey = key;
                    }
                    Serial.print(virtKey);   // Used for testing.
                }
                if (isdigit(key) || key == ' ' || key == '.')
                    Serial.print(key);
                if (key == '#')
                    Serial.println();
                break;

   
        case RELEASED:
            if (buildCount >= sizeof(buildStr))  buildCount = 0;  // Our string is full. Start fresh.
            break;
    }  // end switch-case
}// end switch on state function
 /*****************************************KEYPAD CODE END********************************/
