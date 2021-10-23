#include <SoftwareSerial.h>

//Connect Arduino GND to GND on Jetson Bluetooth Module
//Connect Arduino Digital Pin 2 to TX on Jetson Bluetooth Module
SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{ 
  // Open serial communications 115200 BAUD
  Serial.begin(115200);
  while (!Serial) { ; }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  
}

/*
 * @@@@@@@@@@@@@@@ To find: @@@@@@@@@@@@@@@
 *    •Command to switch headlight on/off
 *    •OTHERS?
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

void loop()
{
  if (mySerial.available()) {
    int d = mySerial.read();
    if (d <= 0x0f)
      Serial.print("0");
    Serial.print(d, HEX);
    if (d == 0xBB)
      Serial.println("");
  }
}
