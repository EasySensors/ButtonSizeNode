/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
**/

// Enable debug prints to serial monitor
#define MY_DEBUG

#include <MemoryFree.h>
#include <avr/wdt.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Enable and select radio type attached
#define MY_RADIO_RFM69


// if you use MySensors 2.0 use this style 
//#define MY_RFM69_FREQUENCY   RF69_433MHZ
//#define MY_RFM69_FREQUENCY   RF69_868MHZ
//#define MY_RFM69_FREQUENCY   RF69_915MHZ


#define MY_RFM69_FREQUENCY   RFM69_433MHZ
//#define MY_RFM69_FREQUENCY   RFM69_868MHZ


// Comment it out for CW  version radio.
//#define MY_IS_RFM69HW

// Comment it out for Auto Node ID #
#define MY_NODE_ID 0xA9

// Avoid battery drain if Gateway disconnected and the node sends more than MY_TRANSPORT_STATE_RETRIES times message.
#define MY_TRANSPORT_UPLINK_CHECK_DISABLED
#define MY_PARENT_NODE_IS_STATIC
#define MY_PARENT_NODE_ID 0


//Enable OTA feature
#define MY_OTA_FIRMWARE_FEATURE
#define MY_OTA_FLASH_JDECID 0  //0x2020

//Enable Crypto Authentication to secure the node
//#define MY_SIGNING_ATSHA204
//#define  MY_SIGNING_REQUEST_SIGNATURES

#include <Wire.h>

// Written by Christopher Laws, March, 2013.
// https://github.com/claws/BH1750

#include <BH1750.h>
BH1750 lightMeter;

#include "SparkFun_Si7021_Breakout_Library.h"
//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather sensor;

#include <MySensors.h>

// Redefining write codes for JDEC FLASH used in the node
// These two defines should always be after #include <MySensors.h> declaration
#define SPIFLASH_BLOCKERASE_32K   0xD8
#define SPIFLASH_CHIPERASE        0x60

#include <stdlib.h>


// Assign numbers for all sensors we will report to gateway\controller (they will be created as child devices)
#define HUM_sensor 2
#define TEMP_sensor 3
#define VIS_sensor 4
#define UV_sensor 5

// Create MyMessage Instance for sending readins from sensors to gateway\controller (they will be created as child devices)
MyMessage msg_hum(HUM_sensor, V_HUM);
MyMessage msg_temp(TEMP_sensor, V_TEMP);
MyMessage msg_vis(VIS_sensor, V_LIGHT_LEVEL);
//MyMessage msg_uv(UV_sensor, V_UV);

unsigned long wdiDelay2  = 0;

int BATTERY_SENSE_PIN = A6;  // select the input pin for the battery sense point

static int32_t oldLux = 0, lux;
static int16_t oldHumdty = 0, humdty;
static int16_t oldTemp = 0, temp;

void swarm_report()
{
  static int oldBatteryPcnt = 0;
  char humiditySi7021[10];
  char tempSi7021[10];
  char VIS_LIGHT[10];


  lightMeter.begin(BH1750::ONE_TIME_LOW_RES_MODE); // need for correct wake up
  lux = lightMeter.readLightLevel();// Get Lux value
  // dtostrf(); converts float into string
  long d = (long)(lux - oldLux);
  Serial.print("abs(lux - oldLux)="); Serial.print(abs(d)); Serial.print(" lux ="); Serial.print(lux); Serial.print(" oldLux ="); Serial.println(oldLux); 
  dtostrf(lux,5,0,VIS_LIGHT);
  if ( abs(d) > 50 ) send(msg_vis.set(VIS_LIGHT), true); // Send LIGHT BH1750     sensor readings
  oldLux = lux;
  sleep(100);

   
  // Measure Relative Humidity from the Si7021
  humdty = sensor.getRH();
  dtostrf(humdty,0,2,humiditySi7021);  
  if (humdty != oldHumdty) send(msg_hum.set(humiditySi7021), true); // Send humiditySi7021     sensor readings
  oldHumdty = humdty; 
  sleep(100);
  
  
  // Measure Temperature from the Si7021
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
  temp = sensor.getTemp();
  dtostrf(temp,0,2,tempSi7021);
  if (temp != oldTemp) send(msg_temp.set(tempSi7021), true); // Send tempSi7021 temp sensor readings
  oldTemp = temp;
  sleep(100);


  // Get the battery Voltage
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  // 1M, 470K divider across battery and using internal ADC ref of 1.1V1
  // ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
  /* The MySensors Lib uses internal ADC ref of 1.1V which means analogRead of the pin connected to 470kOhms Battery Devider reaches  
   * 1023 when voltage on the divider is around 3.44 Volts. 2.5 volts is equal to 750. 2 volts is equal to 600. 
   * RFM 69 CW works stable up to 2 volts. Assume 2.5 V is 0% and 1023 is 100% battery charge    
   * RFM 69 HCW works stable up to 2.5 volts (sometimes it can work up to 2.0V). Assume 2.5 V is 0% and 1023 is 100% battery charge  
   * 3.3V ~ 1023
   * 3.0V ~ 900
   * 2.5V ~ 750 
   * 2.0V ~ 600
   */

#ifdef  MY_IS_RFM69HW
  int batteryPcnt = (sensorValue - 750)  / 1.5;
#else
  int batteryPcnt = (sensorValue - 600)  / 3;
#endif
  
  batteryPcnt = batteryPcnt > 0 ? batteryPcnt:0; // Cut down negative values. Just in case the battery goes below 2V (2.5V) and the node still working. 
  batteryPcnt = batteryPcnt < 100 ? batteryPcnt:100; // Cut down more than "100%" values. In case of ADC fluctuations. 

  if (oldBatteryPcnt != batteryPcnt ) {
    sendBatteryLevel(batteryPcnt);
    oldBatteryPcnt = batteryPcnt;
  }
}

void before() {
  analogReference(INTERNAL); // using internal ADC ref of 1.1V
  //No need watch dog enabled in case of battery power.
  //wdt_enable(WDTO_4S);
  wdt_disable();
  lightMeter.begin(BH1750::ONE_TIME_LOW_RES_MODE);

/* Send JDEC to sleep 
 *  all of _flash.initialize(); _flash.sleep(); and _flash.wakeup();
 *  need to be wrapped with noInterrupts(); - interrupts();
 */
  noInterrupts();
  _flash.initialize();
  _flash.sleep();
  interrupts();
  
}

void setup() {
}

void presentation() 
{  
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("ButtonSize node", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(HUM_sensor, S_HUM);
  present(TEMP_sensor, S_TEMP);
  present(VIS_sensor, S_LIGHT_LEVEL);
}

unsigned long wdiDelay  = 0;

void loop(){
  //No need watch dog in case of battery power.
  //wdt_reset();
  _flash.wakeup();
  
  swarm_report();      

  
  // Go sleep for some milliseconds
  _flash.sleep();
  sleep(300000);
}

