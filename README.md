# VariYO

VariYo is an open source project (both hardware and software) of a compact vario for paragliding (or other forms of freeflight). With hardware being more or less frozen, software is still being updated to include more and more features and hopefully this page will be updated as well.

## Hardware
Hardware platform is single PCB with:

* Barometer
* Gyroscope/accelerometer/magnetometer
* Temperature and humidity sensor
* GNSS reviever (GPS/GLONASS/Galileo/Beidou)
* Bluetooth
* EEPROM
* SD card
* Card reader chip for connecting with PC
* EEPROM
* ARM M0+ microcontroller
* Battery charging IC
* E-paper driving circuitry
* 5-way joystick button (as I hate some non-intuitive button interface and it can be used even in not superthick gloves)

and externally connected:

* Piezo buzzer
* 200x200 1.54'' e-paper display (updated twice a second)
* Li-po battery (I am using 1200mAh, giving around 24h battery life with GPS and roughly 40h without)

###PCB
As I have access to Altium designer at work, it's done in it so to modify with any free software would require some export. But gerber data for manufacturing are available as well, design is in compliance with JLCPCB design rules.
Board is two layer, 50*70mm.

###Components
Components were chosen to be as much hand-solderable as possible. However there are limited options for such devices, so some not so simple packages are there as well 

* one 0.4mm pitch DFN package with exposed pad
* multiple 0.5mm pitch QFN packages

So hot air soldering is probably must.

### Enclosure
Enclosure is designed to be 3d-printable and consists of two parts. STL file in repo. There is silicon rubber cap for the 5-way button which might be harder to DIY, I casted it in a mould made on lathe, but 3d printing it might be possible. 


## Software

Implemented vario-related fucntionality so far is:

* Two varios with user definable averaging period (fusing batometer and accelerometer for instant response)
* Altitude (user adujstable sea level pressure)
* Altitude above ground level (90m mesh)
* Altitude above take-off
* Ground speed
* Heading
* Temperature
* Humidity
* Flight time
* Lift to Drag aka glide ratio
* Wind speed (approximation)
* Wind direction
* Saving tracklog in .gpx format
* Piezo buzzer (adjustable volume, rise and sink trhreshold)
* After-flight summary (take-off, landing, min and max altitude; max rise and sink (10s avg), flight time)

Tthen obviously system stuff such as time, monitoring battery charge status

And some not so standard fetures for paragliding vario:
* Magnetic compass
* Temperature and gumidity logger (every minute, hour or day)
* Chess for parawaiting and stuff (no UI tho, only PvP)
* Since the .gpx tracklog contains many additional data originally for debugging, my [friend](https://github.com/jurajpalenik) did a python script to convert the tracklog into .srt format to use as a subtitles to actioncam footage (containing vario, alt, speed,...).

###Programming
Programming can be done via USB if SAM-BA bootloader is present in the microcontroller. To burn the bootloader, microcontroller programmer is however required (or getting somewhere preprogrammed chip, i know they are on Arduino Zero)




## Acknowledgments

###Arduino libraries
Probably no need to write more, but for those who dont know: https://www.arduino.cc/
###GxEPD2
Library for controlling the e-paper display. Developed by [ZinggJM](https://github.com/ZinggJM/GxEPD2), who can be found on arduino forum in a crazy long thread and his GitHub page is here.
###Adafruit
GxEPD2 library uses some features of [AdafruitGFX library](https://github.com/adafruit/Adafruit-GFX-Library).
###NeoGPS
Minimalistic GPS library by [SlashDevin](https://github.com/SlashDevin/NeoGPS).
###x-io technologies
From those guys, i used Madgwick AHRS library
###Circle fit by Taubin
Algorythm to fit circle on bunch of points. However much I'm looking i can't find the webpage I downloaded it from and there is no header in the source code, so can't give you guys credit, sorry :-(
###Ellipsoid fit
C port of [this](https://www.mathworks.com/matlabcentral/fileexchange/24693-ellipsoid-fit) MATLAB library
###Kalman filter
The core of it all. [This guy](https://github.com/har-in-air) did [kalman filter IMU/barometer vario](http://pataga.net/imukalmanvario.html), with source codes also on GitHub and some really good documentation on his webpage. So naturally i used his Kalman filter code.
###MAX17055
For the fuel gauge, there is a [library](https://github.com/AwotG/Arduino-MAX17055_Driver) from Awot Ghirmai and Ole Dreessen.

