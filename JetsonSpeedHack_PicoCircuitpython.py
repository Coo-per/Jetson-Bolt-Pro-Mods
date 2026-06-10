import board
import busio
import math
import time

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
1) Install Thonny for Windows, Mac, or Linux
2) Select Thonny > Preferencs > Interpreter
3) In the Interpreter settings ensure that you have CircuitPython (generic) selected
4) Copy this code into the a new file in thonny
5) Enter a value for TARGET_SPEED_MPH: 3 mph to 19 mph
6) save and select Raspberry Pi Pico when prompted
7) Name it code.py so this runs when it is plugged into the Jetson
8) With the Jetson Bolt Pro OFF, and recommended 36v battery disconnected - Connect the following pin:
   PICO Pin 3 (Ground)       | --connect to--> | BLACK Wire on Jetson Bolt Pro
   PICO PIN 6  (GP 4)        | --connect to--> | GREY Wire on Jetson Bolt Pro
   PICO Pin 39 (VSYS)        | --connect to--> | ORANGE Wire on Jetson Bolt Pro
9) Connect 36v battery and power the Jetson Bolt Pro ON
10) The headlight will flash twice indicating Speed hack is complete.
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


uart = busio.UART(tx=board.GP4, rx=None, baudrate=115200, bits=8, parity=None) # init uart with baudrate on GP4

kmph = int(math.trunc(TARGET_SPEED_MPH * 1.609))  # Convert mph to kmph

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

#  Write bytes serially to pin 6 (GP4). This should be connected to the GREY wire of the Jetson Bolt Pro
print("Write Speed: "+ str(kmph) + " kmph")
print("Writting response length")
uart.write(setSpeedArray)

#  Flash the headlights twice to confirm speed programming complete
for x in range(0,2):    # Loop twice to flash lights
    time.sleep(.2)
    print("turn lights on")
    uart.write(headLightsOnArray) # Turn lights on

    time.sleep(.2)
    uart.write(headLightsOffArray)  # Turn lights Off
    print("turn lights off")