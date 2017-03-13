// Enable debug prints to serial monitor
#define MY_DEBUG

#include <MemoryFree.h>
#include <avr/wdt.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Enable and select radio type attached
//#define MY_RADIO_NRF24
#define MY_RADIO_RFM69

#define MY_RFM69_FREQUENCY   RF69_433MHZ
//#define MY_RFM69_FREQUENCY   RF69_868MHZ
//#define MY_RFM69_FREQUENCY   RF69_315MHZ
//#define MY_RFM69_FREQUENCY   RF69_915MHZ 

#define MY_IS_RFM69HW
#define MY_NODE_ID 0xE0
#define MY_OTA_FIRMWARE_FEATURE
#define MY_OTA_FLASH_JDECID 0x2020
//#define MY_OTA_FLASH_JDECID 0x1C30

#define MY_SIGNING_ATSHA204
#define  MY_SIGNING_REQUEST_SIGNATURES

// Enable repeater functionality for this node
// #define MY_REPEATER_FEATURE

#include <Wire.h>

//#include "Adafruit_SI1145.h"
//Adafruit_SI1145 uv = Adafruit_SI1145();

#include <BH1750.h>
BH1750 lightMeter;


#include "SparkFun_Si7021_Breakout_Library.h"
//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather sensor;

//#include <SPI.h>
#include <MySensors.h>

// Redefining write codes for JDEC FLASH used in the node
//#define SPIFLASH_BLOCKERASE_32K   0x52
#define SPIFLASH_BLOCKERASE_32K   0xD8
#define SPIFLASH_CHIPERASE        0x60

//#include <SimpleTimer.h>

#include <stdlib.h>
//#include <SPIFlash.h>

//--------------------- https://github.com/JonHub/Filters

#define RELAY_pin 7 // Digital pin connected to relay

#define RELAY_sensor 1
#define HUM_sensor 2
#define TEMP_sensor 3
#define VIS_sensor 4
#define UV_sensor 5

#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay


/*
RFM69::virtual bool sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize,
                             uint8_t retries=2, uint8_t retryWaitTime=
                                 10); //!< sendWithRetry (40ms roundtrip req for 61byte packets)
*/

MyMessage msg(RELAY_sensor, V_LIGHT);
MyMessage msg_hum(HUM_sensor, V_HUM);
MyMessage msg_temp(TEMP_sensor, V_TEMP);
MyMessage msg_vis(VIS_sensor, V_LIGHT_LEVEL);
MyMessage msg_uv(UV_sensor, V_UV);

//SimpleTimer timer;

RFM69 radio;

unsigned long wdiDelay2  = 0;

void swarm_report()
{
  char temp_txt[10];
  char humiditySi7021[10];
  char tempSi7021[10];
  char VIS_LIGHT[10];
  // char UV_SI1145[10];
  uint16_t lux = lightMeter.readLightLevel();// Get Lux value
  int temp_int = (int)radio.readTemperature(0);
  
  Serial.print( "\t  radio.readTemperature: " ); Serial.println( temp_int );
  
  //wait(50);
  //send(msg_temp.set(temp_int), true); // Send RFM module temp sensor readings

  // Measure Relative Humidity from the HTU21D or Si7021
  dtostrf(sensor.getRH(),0,2,humiditySi7021);  

  // Measure Temperature from the HTU21D or Si7021
  dtostrf(sensor.getTemp(),0,2,tempSi7021);
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
 
  dtostrf(lux,5,0,VIS_LIGHT);
//  dtostrf(uv.readVisible(),5,0,VIS_LIGHT);
  //wait(200); 
  //dtostrf((uv.readUV()/100),5,0,UV_SI1145);
  //wait(200); 

  //Serial.println(VIS_LIGHT_SI11450[0],VIS_LIGHT_SI11450[1],VIS_LIGHT_SI11450[2],VIS_LIGHT_SI11450[3]);
  send(msg_temp.set(tempSi7021), true); // Send tempSi7021 temp sensor readings
  wait(30);
  send(msg_hum.set(humiditySi7021), true); // Send humiditySi7021     sensor readings
  wait(30);
  send(msg_vis.set(VIS_LIGHT), true); // Send LIGHT BH1750     sensor readings
  wait(30);
//  send(msg_uv.set(UV_SI1145), true); // Send humiditySi7021     sensor readings
//  wait(30);
}

void before() {
    lightMeter.begin();
      /*
     Set the address for this sensor 
     you can use 2 different address
     Device_Address_H "0x5C"
     Device_Address_L "0x23"
     you must connect Addr pin to A3 .
     */
    //LightSensor.SetAddress(Device_Address_L); //Address 0x23
     /* Set the Working Mode for this sensor 
       Select the following Mode:
        Continuous_H_resolution_Mode
        Continuous_H_resolution_Mode2
        Continuous_L_resolution_Mode
        OneTime_H_resolution_Mode
        OneTime_H_resolution_Mode2
        OneTime_L_resolution_Mode
        The data sheet recommanded To use Continuous_H_resolution_Mode */
    //LightSensor.SetMode(Continuous_H_resolution_Mode);

    /*
    //Initialize the  Si1145 I2C sensors  and ping them
    sensor.begin();
    //uv.begin();
    if (! uv.begin()) {
      Serial.println("Didn't find Si1145");
    //while (1);   
    }
    */
   
    //wdt_enable(WDTO_4S);
    wdt_disable();
 
    // Call swarm_report to update sensor swarm readings.
    //timer.setInterval(10000, swarm_report);

    SPIFlash _flash(MY_OTA_FLASH_SS, MY_OTA_FLASH_JDECID);
    Serial.println(_flash.readDeviceId());
}

void setup() {
}

void presentation() 
{  
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("ButtonSize node", "111.0");

  // Register all sensors to gw (they will be created as child devices)
  present(RELAY_sensor, S_LIGHT);
  present(HUM_sensor, S_HUM);
  present(TEMP_sensor, S_TEMP);
  present(VIS_sensor, S_LIGHT_LEVEL);
  //present(UV_sensor, S_UV);

  //Serial.println("setPowerLevel 31");
  //radio.setPowerLevel(31);
}

unsigned long wdiDelay  = 0;

void loop()
{
  //wdt_reset();
  //Serial.print("freeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeMemory()=");
  //Serial.println(freeMemory());
  //  timer.run();
  //  sleep(1, RISING, 0);
  swarm_report();      
  sleep(60000);
}

