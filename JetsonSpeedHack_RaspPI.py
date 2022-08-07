#!/usr/bin/python3

import serial
import time
import math

""" 

@@@@@@@@@@@@@@@@@@@@ Disclaimer! @@@@@@@@@@@@@@@@@@@@

Everything in this program is DO AT YOUR OWN RISK and for educational purposes ONLY.
It may not be legal to ride in your city/state/country with the use of a speedhack
setting higher than factory.
These bikes ship with a speed setting at 15 mph, and using the official Ride Jetson
app 19 mph is the highest setting. This is considered "Safe" by the community of those
who have already done this speedhack using the genuine Bluetooth module from the
Jetson Bolt (Non-Pro) model.
 
@@@@@@@@@@@@@@ How to use this program @@@@@@@@@@@@@@

1) SSH to your PI and enable the serial pins on your raspberry pi via option 2 at the following link: https://learn.adafruit.com/adafruits-raspberry-pi-lesson-5-using-a-console-cable/enabling-serial-console
2) Install python serial on PI sudo apt-get install python3-serial
3) Upload the program your Raspberry PI board.
4) Enter a value for TARGET_SPEED_MPH: 3 mph to 19 mph.
5) With the Jetson Bolt Pro OFF, and recommended 36v battery disconnected - Connect the following pin:

   PI PIN 6 (Ground)        | --connect to--> | BLACK Wire on Jetson Bolt Pro
   PI PIN 8 GPIO 14 (TXD)   | --connect to--> | GREY Wire on Jetson Bolt Pro
   

6) Connect 36v battery and power the Jetson Bolt Pro ON
7) Run the program sudo python3 JetsonSpeedHack_RaspPI.py
8) The headlight will flash twice indicating Speed hack is complete.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
"""

TARGET_SPEED_MPH = 19

""" /*
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
 */ """

AGREE_TO_TERMS=False

#  Calculate checksum
def checksum(SpeedArray):
    calculatedCheckSum = 0x00
    for arryItem in range(0, len(SpeedArray)-2):
        calculatedCheckSum ^= SpeedArray[arryItem]
    return calculatedCheckSum

def main():
        
    SoftwareSerial =  serial.Serial("/dev/ttyS0", baudrate=115200, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=1,bytesize=serial.EIGHTBITS)

    kmph = int(math.trunc(TARGET_SPEED_MPH * 1.609))  #Convert mph to kmph

    setSpeedArray = bytearray([0xaa, 0x06, 0x06, kmph, 0x00, 0xbb]) #  Initialize byte array
    headLightsOnArray = bytearray([0xaa, 0x07, 0x06, 0x01, 0xaa, 0xbb]) # Store bytes for turning hightlights on
    headLightsOffArray = bytearray([0xaa, 0x07, 0x06, 0x00, 0xab, 0xbb]) # Store bytes for turning hightlights off

    if TARGET_SPEED_MPH < 3:            # Only accept values higher than 3
        kmph = 3
    if kmph > 255 and AGREE_TO_TERMS:    # Don't allow setting higher than 0xff (255)
        kmph = 0xff
    if kmph > 30 and (not AGREE_TO_TERMS):     # If the user has not agreed to terms
        kmph = 30                       # Do not accept speed higher than 19 mph (30 kmph)

    #  Set kmph after checks
    setSpeedArray[3] = kmph

    #  Set checksum value 
    setSpeedArray[4] = checksum(setSpeedArray)  #  Calculate and add the checksum to byte array

    #  Write bytes serially to pin 3. This should be connected to the GREY wire of the Jetson Bolt Pro
    print("Write Speed: "+ str(kmph) + " kmph")
    print("Writting response length")
    SoftwareSerial.write(setSpeedArray)

    #  Flash the headlights twice to confirm speed programming complete
    for x in range(0,2):    # Loop twice to flash lights
        time.sleep(.2)
        print("turn lights on")
        SoftwareSerial.write(headLightsOnArray) # Turn lights on

        time.sleep(.2)
        SoftwareSerial.write(headLightsOffArray)  # Turn lights Off
        print("turn lights off")

if __name__ == "__main__":
    main()