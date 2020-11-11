 /*File used for a keypad with multiple press
  *
  *Code commented out provides the multi press aspect. In order to use this feature, uncomment all variables and methods
  *that use 'first', 'second' and 'third'  
  *
  *The code has been adapted from the keypad library provided by arduino. 
  */

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

void setup() {
  Serial.begin(115200);
  numpad.begin( makeKeymap(numberKeys) ); 
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
}

void loop() {
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
}

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
