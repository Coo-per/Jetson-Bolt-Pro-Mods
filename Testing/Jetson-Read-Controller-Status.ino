#include <SoftwareSerial.h>

//Connect Arduino GND to black wire on Jetson Bolt Pro and Arduino Digital
//Pin 2 to Brown wire on Jetson Bolt Pro
//SoftwareSerial RX on pin 2 -> connect to BROWN wire on the Jetson Bolt Pro
SoftwareSerial mySerial(2, 3); // RX, TX

/*
 * @@@@@@@@@@@@@@@ To find: @@@@@@@@@@@@@@@
 *    •Command to switch headlight on/off:  On: AA070601AABB | Off: AA070600ABBB
 *    •MAX SPEED:         a3, byte 4 = KMPH
 *    •CURRENT SPEED:     a1, (byte 5 * 0xff) + byte 6) - Might be in RPM
 *    •TEMPERATURE:       a1, byte 8 AND DUPLICATED IN a7, byte 8 = C
 *    •VOLTAGE:  V
 *    •CURRENT:  A 
 *    •ODOMETER:          a2, byte 5 = unknown format
 *    •RUN TIME:          a3, byte 8 = minutes
 *    •HEADLIGHT STATUS:  a4, byte 8 = 0x00 off, 0x10 on
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

 /*
  * ID |  BYTE  4  5  6  7  8
  * a1          ** ** ** 00 **  XXXX (4) | CURRENT SPEED (5 & 6) | TEMP (8)
  * a2          00 ** 00 ** **  ODO (5, probably 4 too) | XXXX (7) | XXXX (8)
  * a3          ** 00 00 00 **  MAX SPEED (4) | RUN TIME (8)
  * a4          01 ac 00 00 **  HEADLIGHT STATUS (8)
  * a7          0a 0a d1 00 **  TEMP AGAIN (8)
  */

int maxSpeedKM = 0;   // kmph
float speedRPM = 0.0; // RPM
int temperature = 0;  // Celsius
float voltage = 0.0f; // UNKNOWN
int odo = 0;          // unknown format
int runTime = 0;      // Minutes
int headlight = 0;    // 0x00 off | 0x10 on

int incoming;
int incomingArr[9];
int index = 0;

void setup()
{ 
  // Open serial communications 115200 BAUD
  Serial.begin(115200);
  while (!Serial) { ; }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  
}

void loop()
{
  if (mySerial.available()) {
      incoming = mySerial.read();
      if (incoming != 0xBB) {
        incomingArr[index] = incoming;
        index++;
      } else {
        
        if (checkSum(incomingArr, 10) == incomingArr[8]) {
          if (incomingArr[1] == 0xA1) { //Only read messages from A1
            speedRPM = (incomingArr[5-1] * 0xff) + incomingArr[6-1];
            temperature = incomingArr[8-1];
            //printRegister(incomingArr);
          } else if (incomingArr[1] == 0xA2) {
            odo = incomingArr[5-1];
            //printRegister(incomingArr);
          } else if (incomingArr[1] == 0xA3) {
            maxSpeedKM = incomingArr[4-1];
            runTime = incomingArr[8-1];
            //printRegister(incomingArr);
          } else if (incomingArr[1] == 0xA4) {
            headlight = incomingArr[8-1];
            //printRegister(incomingArr);
          } else if (incomingArr[1] == 0xA7) {
            //printRegister(incomingArr);
          }
        }
        
        index = 0;
      }

      printAll();
      //speed not working
      //voltage unknown
  
  }
  if (Serial.available())
    mySerial.write(Serial.read());
}

void printAll() {
  Serial.println();
  Serial.print("MAX: ");
  Serial.print(maxSpeedKM);
  Serial.print("km|");
  Serial.print(maxSpeedKM/1.609);
  Serial.print("mi\tSpeed: ");
  Serial.print(speedRPM); //Went to 383.. might be in RPM
  Serial.print("RPM\t\tHeadlight: ");
  Serial.print(headlight);
  //Serial.println(voltage);

  Serial.print("\nODO: ");
  Serial.print(odo);
  Serial.print("\tRun Time: ");
  Serial.print(runTime);
  Serial.print(" Minutes\tTemp: ");
  Serial.print(temperature);
  Serial.print("C / ");
  Serial.print( ( temperature * (9/5) ) + 35);
  Serial.println("F\t");
}

void printRegister( int arr[] ) {
  for (int i = 0; i < 9; i++) {
    if (arr[i] <= 0x0f)
      Serial.print(0);
    Serial.print(arr[i], HEX);
  }
    Serial.println(0xbb, HEX);
}

int checkSum(int inArray[], int arraySize) {
  int checksum = 0x00;
  for(int i = 0; i < arraySize - 2; i++)
    checksum ^= inArray[i];
  return checksum;
}
