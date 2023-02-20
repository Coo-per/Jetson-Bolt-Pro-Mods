#include <SoftwareSerial.h>

// Credit and thanks goes to NTWM420 on reddit for letting me meet up with them and tinker
// with their original Jetson bluetooth module.
// He is a nice guy that wants to help everyone that owns this bike.

/*
 * @@@@@@@@@@@@@@@@@@@@ Disclaimer! @@@@@@@@@@@@@@@@@@@@
 * 
 * Everything in this program is DO AT YOUR OWN RISK and for educational purposes ONLY.
 * It may not be legal to ride in your city/state/country with the use of a speedhack
 * setting higher than factory.
 * These bikes ship with a speed setting at 15 mph, and using the official Ride Jetson
 * app 19 mph is the highest setting. This is considered "Safe" by the community of those
 * who have already done this speedhack using the genuine Bluetooth module from the
 * Jetson Bolt (Non-Pro) model.
 * 
 * @@@@@@@@@@@@@@ How to use this program @@@@@@@@@@@@@@
 * 
 * 1) Enter a value for TARGET_SPEED_MPH: 3 mph to 19 mph.
 * 2) Upload the program your arduino board.
 * 3) With the Jetson Bolt Pro OFF, and recommended 36v battery disconnected - Connect the following pins:
 *    ARDUINO GND       | --connect to--> | BLACK Wire on Jetson Bolt Pro
 *    ARDUINO DIGITAL 3 | --connect to--> | GREY Wire on Jetson Bolt Pro
 *    ARDUINO 5V        | --connect to--> | ORANGE Wire on Jetson Bolt Pro
 *    
 * 4) Connect 36v battery and power the Jetson Bolt Pro ON
 * 5) The headlight will flash twice indicating Speed hack is complete. For good measure.. while everything is still
 *    connected you can press the reset button on the arduino which will run the program once more - just to be sure.
 * 
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */
#define TARGET_SPEED_MPH 19

/*
 * @@@@@@@@@@@@@@ TO INCREASE ABOVE 19 MPH, PLEASE READ ALL OF THE BELOW @@@@@@@@@@@@@@
 * 
 * Per the "Ride Jetson" App for Android, the minimum speed is 3 mph (5 km/h) and the
 * maximum speed is 19 mph (30 km/h) [really it's 18.6 mph rounded up since calculation is from km].
 * These are the settings that we know will work and is considered "SAFE"
 * 
 * That said, the speed can be increased further above 19 mph (30 km/h) if you're feeling frisky however it is
 * unknown what this will result in. This means DO IT AT YOUR OWN RISK - only if you know what you are doing.
 * Setting the speed higher MAY result in hotter motor tempuratures, higher Amps through the controller
 * and battery. OR, and this is probably the case... the controller may have an auto-shutoff built in which
 * prevent too many Amps, too hot motor temp, etc.
 * 
 * If you accept these risks:
 * To increase speed higher, change TARGET_SPEED_MPH to a value higher than 19. Next change the below from
 *                          AGREE_TO_TERMS false -> AGREE_TO_TERMS true
 * This will unlock the protection built into this program which prevent higher than 19 mph
 * 
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */
#define AGREE_TO_TERMS false

//SoftwareSerial TX on pin 3 -> connect to GREY wire on the Jetson Bolt Pro
SoftwareSerial mySerial(2, 3); // RX, TX

//Start message: [aa] | Message Destination: [xxxx] | Data: [xx] | Checksum: [xx] | End message: [bb]
//To change max speed, 0606 is the message destination and data is desired max speed (KM) in hexadecimal
//For example 19 MPH the command is: aa06061eb4bb
int kmph = TARGET_SPEED_MPH * 1.609;  //Convert mph to kmph
byte bA[] = {0xaa, 0x06, 0x06, kmph, 0x00, 0xbb}; //Initialize byte array
byte headlightsOn[] = {0xaa, 0x07, 0x06, 0x01, 0xaa, 0xbb};
byte headlightsOff[] = {0xaa, 0x07, 0x06, 0x00, 0xab, 0xbb};

void setup()
{ 
  // Open serial communications 115200 BAUD
  Serial.begin(115200);
  while (!Serial) { ; }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);

  if (TARGET_SPEED_MPH < 3)             // Only accept values higher than 3
    kmph = 3;
  if (kmph > 0xff && AGREE_TO_TERMS)    // Don't allow setting higher than 0xff (255)
    kmph = 0xff;
  if (kmph > 30 && !AGREE_TO_TERMS)     // If the user has not agreed to terms
    kmph = 30;                          // Do not accept speed higher than 19 mph (30 kmph)

  bA[3] = kmph;
    
  int SizeOfArray = sizeof(bA) / sizeof(bA[0]);
  bA[4] = checkSum(bA, SizeOfArray);  //Calculate and add the checksum to byte array

  //Flash the headlights twice to confirm speed programming complete
  delay(500); // Wait for Jetson controller to boot
  for(int i = 0; i < SizeOfArray; i++) // Headlights on
    mySerial.write(headlightsOn[i]);
  delay(500);
  for(int i = 0; i < SizeOfArray; i++) // Headlights off
    mySerial.write(headlightsOff[i]);
  delay(250);
  for(int i = 0; i < SizeOfArray; i++) //Write bytes serially to pin 3. This should be connected to the GREY wire of the Jetson Bolt Pro
    mySerial.write(bA[i]);
  delay(250);
  for(int i = 0; i < SizeOfArray; i++) // Headlights on
    mySerial.write(headlightsOn[i]);
  delay(500);
  for(int i = 0; i < SizeOfArray; i++) // Headlights off
    mySerial.write(headlightsOff[i]);

}

byte checkSum(byte byteArray[], int arraySize) {
  byte checksum = 0x00;
  for(int i = 0; i < arraySize - 2; i++)
    checksum ^= byteArray[i];
  return checksum;
}

void loop()
{
  
}
