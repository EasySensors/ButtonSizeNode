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
#define MY_RFM69_FREQUENCY   RF69_433MHZ

// Comment it out for CW  version radio.
#define MY_IS_RFM69HW

// Comment it out for Auto Node ID #
#define MY_NODE_ID 0xAA

//Enable OTA feature
#define MY_OTA_FIRMWARE_FEATURE
#define MY_OTA_FLASH_JDECID 0x2020

//Enable Crypto Authentication to secure the node
//#define MY_SIGNING_ATSHA204
//#define  MY_SIGNING_REQUEST_SIGNATURES

#include <Wire.h>

// Written by Christopher Laws, March, 2013.
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

#define RELAY_pin 7 // Digital pin connected to relay


// Assign numbers for all sensors we will report to gateway\controller (they will be created as child devices)
#define HUM_sensor 2
#define TEMP_sensor 3
#define VIS_sensor 4
#define UV_sensor 5

// Create MyMessage Instance for sending readins from sensors to gateway\controller (they will be created as child devices)
MyMessage msg_hum(HUM_sensor, V_HUM);
MyMessage msg_temp(TEMP_sensor, V_TEMP);
MyMessage msg_vis(VIS_sensor, V_LIGHT_LEVEL);
MyMessage msg_uv(UV_sensor, V_UV);

unsigned long wdiDelay2  = 0;

void swarm_report()
{
  char temp_txt[10];
  char humiditySi7021[10];
  char tempSi7021[10];
  char VIS_LIGHT[10];
  uint16_t lux = lightMeter.readLightLevel();// Get Lux value

  // Measure Relative Humidity from the HTU21D or Si7021
  // dtostrf(); converts float into string
  dtostrf(sensor.getRH(),0,2,humiditySi7021);  

  // Measure Temperature from the HTU21D or Si7021
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
  dtostrf(sensor.getTemp(),0,2,tempSi7021);
 
  send(msg_temp.set(tempSi7021), true); // Send tempSi7021 temp sensor readings
  wait(50);
  send(msg_hum.set(humiditySi7021), true); // Send humiditySi7021     sensor readings
  wait(50);
  dtostrf(lux,5,0,VIS_LIGHT);
  send(msg_vis.set(VIS_LIGHT), true); // Send LIGHT BH1750     sensor readings
  wait(50);
}

void before() {
    //No need watch dog enabled in case of battery power.
    //wdt_enable(WDTO_4S);
    wdt_disable();
    lightMeter.begin();
}

void setup() {
}

void presentation() 
{  
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("ButtonSize node", "111.0");

  // Register all sensors to gw (they will be created as child devices)
  present(HUM_sensor, S_HUM);
  present(TEMP_sensor, S_TEMP);
  present(VIS_sensor, S_LIGHT_LEVEL);
}

unsigned long wdiDelay  = 0;

void loop()
{
  //No need watch dog in case of battery power.
  //wdt_reset();
  
  swarm_report();      
  // Go sleep for some milliseconds
  sleep(60000);
}

