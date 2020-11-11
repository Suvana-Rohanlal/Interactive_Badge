/*
 * File to use WiFi as a communication method.
 * A server is created that will allow many devices to be
 * connected. A web address will be created and you will be 
 * allowed to access the question/answer page. 
 * 
 * created for a wemos lolin32
 *
 *libraries taken from expressify esp32 
 *
*/

/*
 *The WiFi library allows for a connection using Wifi.
 *The WebServer library takes care of the web page(HTML part)
*/
#include<Arduino.h>
#include<WiFi.h>
#include<WebServer.h>
#include<AsyncTCP.h>
#include<ESPAsyncWebServer.h>

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
 
/*
 * The set up method connects to the WiFi network provided and creates the server
 * requests. 
 * 
 * The server is started in this method.
 */
void setup() {
  Serial.begin(115200);
    
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
    request->send_P(200, "text/html", index_html,processor);
  });

  
  server.onNotFound(notFound);
   // server.on("/",handle_root);
    
    server.begin();
  
}

/*The loop method checks if there is any data available on serial monitor.
 *
 *The global question variable is then populated with the value from 
 *serial monitor to be used in the processor method.
 */
void loop() {
  if(Serial.available()){
  question = Serial.readString();  
  Serial.println(question);
  }
}
