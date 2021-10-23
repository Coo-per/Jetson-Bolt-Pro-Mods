This Repository is dedicated to mods for the Jetson Bolt Pro. At the moment the only mod is speedhack - more to come?

https://www.youtube.com/watch?v=HBDkVcntbl0

**************

Credit and thanks goes to NTWM420 on reddit for letting me meet up with them and tinker with their original Jetson bluetooth module.
He is a nice guy that just wants to help everyone that owns this bike.

**************

If you have technical know-how and just want to send commands serially yourself. The TLDR is..
Baud rate: 115200, TX the HEX message on the Grey wire: aa06061eb4bb

Otherwise, keep reading.

**************

If you're just here for the speed hack download JetsonSpeedHack.ino and follow the instructions inside.
You will need an arduino, and upload the code JetsonSpeedHack.ino

You CANNOT use the DSD TECH HM-10 from Amazon BY DEFAULT. The default baud rate is 9600 and the only way to activate Baud rate 115200 is to first send it an AT command: AT+BAUD4 via a computer using an arduino or other serial comm device. You can also set AT+MODE2 via an arduino/serial comm and this will let you send AT commands directly from the phone via bluetooth to use the device with less limitation.

You might as well just buy an arduino though and skip the HM-10.

**************

@@@@@@@@@@@@@@@@@@@@ Disclaimer! @@@@@@@@@@@@@@@@@@@@

Everything in this program is DO AT YOUR OWN RISK and for educational purposes ONLY.
It may not be legal to ride in your city/state/country with the use of a speedhack
setting higher than factory.
These bikes ship with a speed setting at 15 mph, and using the official Ride Jetson
app 19 mph is the highest setting. This is considered "Safe" by the community of those
who have already done this speedhack using the genuine Bluetooth module from the
Jetson Bolt (Non-Pro) model.

@@@@@@@@@@@@@@ How to use this program @@@@@@@@@@@@@@

Recommended to first program with 3 mph- test the throttle and see the wheel spinning slowly.
Then run through all the steps again with 19 mph
In this way you will know that the speed hack has FOR SURE been applied.

1) Enter a value for TARGET_SPEED_MPH: 3 mph to 19 mph.
2) Upload the program your arduino board.
3) With the Jetson Bolt Pro OFF, and recommended 36v battery disconnected - Connect the following pins:
   ARDUINO GND       | --connect to--> | BLACK Wire on Jetson Bolt Pro
   ARDUINO DIGITAL 3 | --connect to--> | GREY Wire on Jetson Bolt Pro
   ARDUINO 5V        | --connect to--> | ORANGE Wire on Jetson Bolt Pro
   
4) Connect 36v battery and power the Jetson Bolt Pro ON
5) Speed hack complete. For good measure.. while everything is still connected you can press the reset
   button on the arduino which will run the program once more


@@@@@@ TO INCREASE ABOVE 19 MPH, PLEASE READ ALL OF THE BELOW @@@@@@

Per the "Ride Jetson" App for Android, the minimum speed is 3 mph (5 km/h) and the
maximum speed is 19 mph (30 km/h) [really it's 18.6 mph rounded up since calculation is from km].
These are the settings that we know will work and is considered "SAFE"

That said, the speed can be increased further above 19 mph (30 km/h) if you're feeling frisky however it is
unknown what this will result in. This means DO IT AT YOUR OWN RISK - only if you know what you are doing.
Setting the speed higher MAY result in hotter motor tempuratures, higher Amps through the controller
and battery. OR, and this is probably the case... the controller may have an auto-shutoff built in which
prevent too many Amps, too hot motor temp, etc.

If you accept these risks:
To increase speed higher, change TARGET_SPEED_MPH to a value higher than 19. Next change the below from
                         AGREE_TO_TERMS false -> AGREE_TO_TERMS true
This will unlock the protection built into this program which prevent higher than 19 mph

**************

In the future if time permits, would like to add a Jetson Bolt Pro speedometer which includes Adjustable speed on the fly, 1-click headlight on/off, Temperature, Voltage, Current, ODO, Ride Time display.
If you would like to help with that project please help me find the messages from the rx pin! :D
That project is in Jetson-Read-Controller-Status.ino which just reads commands from the Jetson Bolt Pro controller. Currently only reading messages from A1. But the Jetson  sends messages from A1 A2, A3, A4, A7... possibly others, I havn't looked into it much.
