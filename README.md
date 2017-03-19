

![enter image description here](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/bttnsz.jpg?raw=true)

## The Button Size Node is a low cost wireless Arduino IDE compatible (the Atmel ATMega328P) microcontroller with RFM 69 HW(CW) radio on board and few other nice additions. Check Specs below. It replaces this setup:
![](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/replce.jpg?raw=true)

## Specification: ##
 - Dimensions 42.2mm x 20.9mm
 - Temperature and humidity sensor Si7021 
 - High Accuracy Temperature Sensor ±0.4 °C (max), –10 to 85 °C
 - Precision Relative Humidity Sensor ± 3% RH (max), 0–80% RH
 - Light sensor BH1750,  spectral responsibility is approximately human eye response.
 - Authentication security - Atmel ATSHA204A Crypto Authentication Chip
 - External JDEC EPROM
 - RFM69-HW (high power version) or CW (low power consumption version) 433 MHz Radio transceiver
 - Battery voltage sensor (via divider)
 - Supply voltage up to 6.5 Volts
 - The Digital and Analog pins are 3.3 volts
 - Battery connector CR2032 240mAh


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
Scissors line | you cat cut sensors and battery holder if you need just controller and radio


**Arduino IDE Settings**

![Arduino IDE Settings](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/IDEsettings.jpg?raw=true)



How to use it as home automation (IOT) node controller
------------------------------------------------------


ButtonSizeNode.ino is the Arduino example sketch using [MySensors](https://www.mysensors.org/) API. 

To turn these nodes into home automation network you need at least two Nodes one as a node and the other one as “Gateway Serial” and connect them to a controller (I personally love [Domoticz](https://domoticz.com/)). However, for no-controller setup, as example, you can use 3 nodes - one node as “Gateway Serial”, node as, say relay and one as switch for that relay. No controller needed then, keep the switch and the relay on the same address and the switch will operate the relay.

Things worth mentioning about the  [MySensors](https://www.mysensors.org/) Arduino sketch: 


Arduino Pins|	Description
------------|--------------
#define MY_IS_RFM69HW <br> #define MY_RADIO_RFM69 <br> #define MY_RFM69_FREQUENCY   RF69_433MHZ <br> #define MY_IS_RFM69HW |	Define which radio we use – here is RFM 69 with frequency 433 MHZ and it is HW type – one of the most powerful RFM 69 radios.  If your radio is RFM69CW - comment out line with // #define MY_IS_RFM69HW 

Define which radio we use – here is RFM 69 with frequency 433 MHZ and it is HW type – one of the most powerful RFM 69 radios.  If your radio is RFM69CW - comment out line with // #define MY_IS_RFM69HW 

```c++
// Enable and select radio type attached 
#define MY_IS_RFM69HW
#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY   RF69_433MHZ
#define MY_IS_RFM69HW
```  

Define Node address. I prefer to use static addresses and in Hexadecimal since it is easier to identify the node address in  [Domoticz](https://domoticz.com/) devices list after it will be discovered by controller ( [Domoticz](https://domoticz.com/)).
```c++
#define MY_NODE_ID 0xE0
```
However, you can use AUTO instead of the hardcoded number (like 0xE0) though.  [Domoticz](https://domoticz.com/) will automatically assign node ID then.

Define OTA feature. OTA stands for “Over The Air firmware updates”. If your node does not utilize Sleep mode you can send new “firmware” (compiled sketch binary) by air. **Here is the link on how to do it.**
For OTA we use JDEC Flash chip where the node stores new firmware and once it received and control sum (CRC) is correct it reboots and flashes your new code into the node controller. So we define it is "erase type" as 0x2020 here. This define should be as below: 
```c++
#define MY_OTA_FIRMWARE_FEATURE
#define MY_OTA_FLASH_JDECID 0x2020
```
If you would like to use Crypto Authentication to secure your nodes from intruders or interference, you need to enable:

```c++
#define MY_SIGNING_ATSHA204
```
After that, you have to “personalize” all the nodes, which have those, defines enabled. [**How to “personalize” nodes with encryption key**](https://github.com/EasySensors/ButtonSizeNode/blob/master/SecurityPersonalizationHowTo.md)

Connect the Node to FTDI USB adaptor, Select Pro Mini 8MHz board and burn the ButtonSizeNode.ino sketch.

**Done**

>For schematics lovers:

![enter image description here](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/schematic.jpg?raw=true)

The board is created by  [Koresh](https://www.openhardware.io/user/143/projects/Koresh)
