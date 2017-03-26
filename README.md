

![enter image description here](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/buttonsize1.jpg?raw=true)
![enter image description here](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/buttonsize2.jpg?raw=true)

**The Button Size Node is a low cost wireless Arduino IDE compatible (the Atmel ATMega328P 8MHz) microcontroller with RFM 69 HW(CW) radio on board and few other nice additions.** 
------------------------------------------------------------------------

Best sutable for Home Automation, IOT. Could be used as core board for radio controlling any DIY project. Check Specs below. It replaces this setup:

![](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/replceA.jpg?raw=true)

## Specification: ##
 - Dimensions 42.2mm x 20.9mm
 - Temperature and humidity sensor Si7021 
 - High Accuracy Temperature Sensor ±0.4 °C (max), –10 to 85 °C
 - Precision Relative Humidity Sensor ± 3% RH (max), 0–80% RH
 - Light sensor BH1750,  spectral responsibility is approximately human eye response.
 - Authentication security - Atmel ATSHA204A Crypto Authentication Chip
 - External JDEC EPROM
 - Dualoptiboot bootloader. Implements over the air (OTA) firmware update ability
 - RFM69-HW (high power version) or CW (low power consumption version) 433 MHz Radio transceiver
 - Battery voltage sensor (via divider)
 - Supply voltage up to 6.5 Volts
 - The Digital and Analog pins are 3.3 volts
 - Battery connector CR2032 240mAh
 - FTDI  header for programming
 - You can cut with scissors, sensors and battery holder part, if you need just controller and radio


**Pin out:** 


Arduino Pins|	Description
------------|--------------
A0, A1, A2 |	Available ARDUINO analog GPIO / DIGITAL GPIO
A6 |	Connected to Battery voltage sensor (via divider)
A4 |	Connected to si1132 and bh1750 SDA 
A5 |	Connected to si1132 and bh1750 SCL
A3 |	Connected to  ATSHA204A
D3, D4, D5, D6,D7, D9 |	Available ARDUINO digital GPIO
D8 |	Connected to CS FLASH chip (OTA) M25P40
MISO, MOSI, SCK, RST |	Connected to ISP header
ANT |	RFM69 antenna
Bat+ | Unregulated power up to 6.5 Volts
Gnd | Ground
Scissors line | you cat cut with scissors, sensors and battery holder part, if you need just controller and radio


**Arduino IDE Settings**

![Arduino IDE Settings](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/IDEsettings.jpg?raw=true)



How to use it as home automation (IOT) node controller
------------------------------------------------------


ButtonSizeNode.ino is the Arduino example sketch using [MySensors](https://www.mysensors.org/) API. 

- **Controller Setup.**  
Burn the ButtonSizeNode.ino sketch into the board and it will became  one of the MySensors home automation network Sensor's Node reporting Temp, Humidity and Vsial light in Luxes to a smarthome controller. 
To create the home automation network you need smarthome controller and at least two Nodes one as a Sensor, a Relay or an Actuator Node and the other one as the “Gateway Serial” connected to a smarthome controller. I personally love [Domoticz](https://domoticz.com/) as a smarthome controller. Please check this [HowTo](https://github.com/EasySensors/ButtonSizeNode/blob/master/DomoticzInstallMySensors.md) to install Domoticz.

- **No Controller setup.** 
However, for no-controller setup, as example, you can use 3 nodes - first node as the “Gateway Serial”, second node as a Relay and last one as a Switch for that Relay. No controller needed then, keep the Switch and the Relay on the same address and the switch will operate the relay. 


Things worth mentioning about the  [MySensors](https://www.mysensors.org/) Arduino sketch: 


Arduino Pins|	Description
------------|--------------
#define MY_RADIO_RFM69<br>#define MY_RFM69_FREQUENCY   RF69_433MHZ<br>#define MY_IS_RFM69HW|	Define which radio we use – here is RFM 69<br>with frequency 433 MHZ and it is HW<br>type – one of the most powerful RFM 69 radios.<br>If your radio is RFM69CW - comment out line<br>with // #define MY_IS_RFM69HW 
#define MY_NODE_ID 0xE0 | Define Node address (0xE0 here). I prefer to use static addresses<br> and in Hexadecimal since it is easier to identify the node<br> address in  [Domoticz](https://domoticz.com/) devices list after it<br> will be discovered by controller ( [Domoticz](https://domoticz.com/)).<br> However, you can use AUTO instead of the hardcoded number<br> (like 0xE0) though.  [Domoticz](https://domoticz.com/) will automatically assign node ID then.
#define MY_OTA_FIRMWARE_FEATURE<br>#define MY_OTA_FLASH_JDECID 0x2020 | Define OTA feature. OTA stands for “Over The Air firmware updates”.<br> If your node does not utilize Sleep mode you can send new “firmware”<br> (compiled sketch binary) by air. [Here is the link on how to do it.](https://www.mysensors.org/about/ota)<br> Skip to the step "How to upload a new sketch just with OTA" as all initial <br> steps have been completed in the ButtonSizeNode. <br>For OTA we use JDEC Flash chip where the node stores<br> new firmware and once it has been  received and checksum (CRC)<br> is correct it reboots and flashes your new<br> code into the node controller.  0x2020 "Erase type"  <br>defined here for JDEC Flash chip . 
#define MY_SIGNING_ATSHA204 <br>#define  MY_SIGNING_REQUEST_SIGNATURES | Define if you like to use Crypto Authentication to secure your nodes<br> from intruders or interference. After that, you have to “personalize”<br> all the nodes, which have those, defines enabled.<br> [**How to “personalize” nodes with encryption key**](https://github.com/EasySensors/ButtonSizeNode/blob/master/SecurityPersonalizationHowTo.md).<br> You need both defines in the nodes you need to protect.<br> The Gateway Serial could be with only one of those<br> defines enabled - #define MY_SIGNING_ATSHA204

Connect the Node to FTDI USB adaptor, Select Pro Mini 8MHz board in Arduino IDE and upload the ButtonSizeNode.ino sketch.

**Done**


The board is created by  [Koresh](https://www.openhardware.io/user/143/projects/Koresh)

![enter image description here](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/bs1.jpg?raw=true)
![enter image description here](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/bs2.jpg?raw=true)


>For schematics lovers:

![enter image description here](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/schematic.jpg?raw=true)

[PDF]https://github.com/EasySensors/ButtonSizeNode/blob/master/pdf/ButtonSizeNode_ext1.pdf


P.S. Always mind! if your Arduino code fails you need some backup plan. Put some candles and matches in your bathroom )))))... hope you understand. Start your automation wisely. Like auotomate your doghouse first. Then checkenshed and so on!!!
