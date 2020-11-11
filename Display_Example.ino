/*File created for the tft shield
 *
 *Code could not be tested because shield was not compatible.
 *
 *Created by suvana
 *Adapted from TFT lcd code provided by waveshare
 */
#include <Arduino.h>
#include <SPI.h> //enables SPI
#include <Adafruit_GFX.h> //enables graphics
#include <Waveshare4InchTftShield.h> //enables touch
#include <ILI9486_SPI.h> //Hardware-specific library
#include <TFT_eSPI.h> // Hardware-specific library


TFT_eSPI tft = TFT_eSPI();       // Invoke custom 

#define LIGHTBLUE  0x001F//61b2cd  //RGB: 97, 178, 205
#define BLACK   0x0000 

void setup(){
tft.fillScreen(LIGHTBLUE);
tft.setTextColor(BLACK);
tft.setTextSize(3);

tft.setTextSize(5);
//tft.text("Question :\n ", 50, 25); // 50 and 25 being the coordinates

 //Draw buttons in a line
tft.drawRect(250 ,200 , 100, 100 , BLACK );
tft.drawRect(140,200,100,100,BLACK);
tft.drawRect(190,200,100,100,BLACK);
}

void loop() {
int button_on = 0;
  int pixel_x, pixel_y;   
 /* TSPoint p = ts.getPoint();
  if (p.z > ts.pressureThreshhold) {
    
    pixel_x = map(p.y, TS_LEFT=757, TS_RT=104, 0, 480);// mapping the values
    pixel_y = map(p.x, TS_TOP=142, TS_BOT=858, 0, 320);
  
       // if the LCD sens the touch in following coordinates, then the                 condition will be executed 
  
       if(pixel_x>295 && pixel_x<350 && pixel_y>200 && pixel_y<300 ){
        //submit pressed
        }
         if(pixel_x>140&& pixel_x<195 && pixel_y>200 && pixel_y<300 ){
        //back pressed
        }
        if(pixel_x>195&& pixel_x<295 && pixel_y>200 && pixel_y<300 ){
        //space pressed
        }
       }*/

}
