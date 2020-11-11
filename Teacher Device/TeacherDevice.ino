/*File created for the overall teacher's device
 *
 *File contains keypad, communication, storage and display aspects.
 *
 *Created by Suvana
 *
 *Adapted from all the ESP32 libraries by expressify
 *
 */
#include <AnswerAnalysis.h>
#include<Arduino.h>
#include<WiFi.h>
#include<WebServer.h>
#include<AsyncTCP.h>
#include<ESPAsyncWebServer.h>

AnswerAnalysis answers;
  /*****************************************KEYPAD CODE START********************************/
#include <Keypad.h>
#include <ctype.h>

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
    

/*****************************************SERVER CODE START********************************/
const char* ssid = "Eduroam For Who";//"Eduroam For Who";//"WR7010-2.4G-631";
const char* password ="Whitebread303";// "Whitebread303";//"fuac62756";

AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "input1";
String PARAM_QUESTION = "questionString";
uint8_t* ans;
String question ="";

/*index_html stores the html code used to produce the user interface
 *R"rawliteral is used to ensure all the characters used in html is not changed
 *according to arduino.
 *
 *Divs are created for the answer and question. This allows for styling and easy 
 *access to the data stored on the fields.
 */
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>EEE2045F</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">  
   
  <style>
  body{
    background-color: powderblue;
    text-align:center;
  }
  .question{
    border:5px outset black;
    background-color: white;
    text-align: center;
  }

  .answer{
    border:5px outset black;
    background-color: white;
  }
  </style>
</head>
<body>
<h1>Welcome to EEE2045F</h1>
<div class="question">
<h2>Question</h2>
<form action="/question">
    <input type="submit" value="Get Question">
  </form><br>
<p>%TheQuestion%</p>
</div>
<div class="answer">
  <h2>Answer</h2> 
  <form action="/get">
    Enter your answer: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
</div>
</body></html>
)rawliteral";

/*This method allows for a dynamic variable. The variable is updated whenever this
 * method is called. 
 *
 *The method checks if the placeholder is equal to a specific name and returns
 *the string to the placeholder.
 */
String processor(const String& var)
{
  if(var == "TheQuestion"){

    return question.c_str();
  }
  return String();
}

/*Method used to determine if the webpage is available*/ 
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

/*****************************************SERVER CODE END********************************/

void setup() {
    Serial.begin(115200);
  /*****************************************SERVER CODE START********************************/
 WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 // delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  server.on("/question", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html,processor);
   }
  );


   /*Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    *
    *The answer is extracted from the input field and displayed
    *on serial monitor.
    *
    *The webpage then returns to the main screen
    */
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;

    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();

      inputParam = PARAM_INPUT_1;
    }
  else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

     Serial.println(inputMessage);
     answers.write(inputMessage);
    request->send_P(200, "text/html", index_html,processor);
  });

  
  server.onNotFound(notFound);
   // server.on("/",handle_root);
    
    server.begin();

 /*****************************************SERVER CODE END********************************/

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
 if(Serial.available()){
  question = Serial.readString();  
  Serial.println(question);
  }
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
