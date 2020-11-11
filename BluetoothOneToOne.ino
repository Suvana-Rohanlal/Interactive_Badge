/* This file includes the investigation into bluetooth for the device.
   The 'BluetoothSerial' library was produced by Expressify for ESP32.
   This investigation is in regards to a one to one communication. 
   The development board will communicate with an android phone using
   the 'Serial Blurtooth Terminal' available on the play store. */

#include "BluetoothSerial.h"

//object to call upon function in the BluetoothSerial library
BluetoothSerial bt;

//configuration setting if not configured
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Variable declarations
char  ans;
char  ques;
bool  connectedDevice = false;

void setup() {
  //ESP32 uses a baud rate of 115200
  Serial.begin(115200);
  
  //Name of the bluetooth device 
  bt.begin("EEE2045F");
  delay(20);
  Serial.println("'EEE2045F' is available for pairing.");

}

void loop() {
  device_connected();
  if(connectedDevice == true){
  question();
  answer();
  delay(20);
  }
}

/*Function to retrieve and display the question
 sent from the teacher's device. Checks if there is something sent 
 then displays it.*/
void question(){
    if(bt.available()){
    ques = bt.read();// + '/n Enter your answer:';
    Serial.write(ques);
  }
}

/*Function to retrieve and display the answer
 sent from the students's device. Checks if there is something sent 
 then displays it.*/
void answer(){
    if(Serial.available()){
    ans = Serial.read();
    bt.write(ans);
    Serial.write(ans);  
  }
}

/*Function to check if the devices are connected to each other.*/
void device_connected(){
    if(bt.hasClient()){
      connectedDevice = true;
      delay(20);
      }
}
