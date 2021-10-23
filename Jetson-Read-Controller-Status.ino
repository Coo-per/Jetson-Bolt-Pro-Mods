#include <SoftwareSerial.h>

//Only need to connect Arduino GND to black wire on Jetson Bolt Pro and Arduino Digital Pin 2 to Brown wire on Jetson Bolt Pro
//SoftwareSerial RX on pin 2 -> connect to BROWN wire on the Jetson Bolt Pro
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
 *    •TEMP:     C/F
 *    •VOLTAGE:  V
 *    •CURRENT:  A
 *    •ODO:      KM/MI
 *    •TIME:     MIN
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

/*
 * //START: AAA10A53, Current Speed: 0x0000-0x0122?, ?, END: Checksum + BB
 * Message AAA10A53 appears to contain the current speed, maybe throttle position: values 0 to 0122
 * AAA10A53 00 00 00 1A 48BB   BEFORE THROTTLE
 * AAA10A53 01 22 00 1B 6ABB   FULL THROTTLE
 * AAA10A53 00 00 00 1B 49BB   AFTER THROTTLE
 * 
 */

String str;
int incoming;
int incomingArr[9];
int index = 0;

void loop()
{
  if (mySerial.available()) {
      incoming = mySerial.read();
      if (incoming != 0xBB) {
        incomingArr[index] = incoming;
        index++;
      } else {

        if (incomingArr[1] == 0xA1) { //Only read messages from A1
          for (int i = 0; i < 9; i++) {
            if (incomingArr[i] <= 0x0f)
              Serial.print(0);
            Serial.print(incomingArr[i], HEX);
          }
          Serial.println(incoming, HEX);
        }

        index = 0;
      }
  }
  if (Serial.available())
    mySerial.write(Serial.read());
}
