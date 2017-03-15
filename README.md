The code is example of MySensors way of operating controller nodes as home network
To use it you need at least two controllers one as a node the other one as “Gateway Serial” usually connected to some controller (I love Domoticz). However, for very simple setup you can use 3 nodes - node as gateway, node as relay and last one as switch. 
Things worth mentioning about the MySensors Arduino sketch:
First, we need to define which radio we use – RFM 69 with frequency 433 MHZ and it is HW type – one of the most powerful RFM 69 radios. 
```c++
// Enable and select radio type attached
#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY   RF69_433MHZ
#define MY_IS_RFM69HW
```
I use static addresses and in Hexadecimal since it is easier to identify the node number in Domoticz devices list after it will b1e discovered by controller (Domoticz).
```c++
#define MY_NODE_ID 0xE0
```

However, you can use AUTO instead of the hardcoded number (like 0xE0) though. Domoticz will automatically assign node ID then.
Here we go and define our OTA feature. OTA is stands for “Over The Air firmware updates”. If your node does not utilize Sleep mode you can send new “firmware” (complied sketch) by air. **Here is the link on how to do it.**
For OTA we use JDEC Flash chip where the node stores new firmware and once it received and control Sum is correct it reboots and flashes your new code into the node controller. So we define it is erase type 0x2020 here. This define should be as below: 

```c++
#define MY_OTA_FIRMWARE_FEATURE
#define MY_OTA_FLASH_JDECID 0x2020
```
If you would like to Crypto authentication to secure your nodes from intrudes you need to enable it:

```c++
#define MY_SIGNING_ATSHA204
#define  MY_SIGNING_REQUEST_SIGNATURES
```

After that, you have to “personalize” all the nodes, which have those, defines enabled. **How to “personalize” nodes with encryption key**
Connect to FTDI USB adaptor, Select Pro Mini 8MHz board and burn the sketch.
