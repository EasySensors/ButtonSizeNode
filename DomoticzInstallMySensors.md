## MySensors Nodes use [Domoticz](https://domoticz.com/) as Controller running Raspberry PI. How To###

In this How To I will use [ButtonSizeNode](https://github.com/EasySensors/ButtonSizeNode) as Gateway Serial for MySensors network

**Step 1.**
****
Complete: **Preparation (for both methods)** and  **Domoticz - The easy way** steps from this guide:
[ Installing and running Domoticz on a Raspberry PI](https://www.domoticz.com/wiki/Installing_and_running_Domoticz_on_a_Raspberry_PI)

It may look a bit complicated but in fact it will be very easy

**Step 2.**
****

Load GatewaySerial.ino sketch into ButtonSizeNode or any Arduino + radio transiever (RFM69 or NRF24) attached. You can use use this **guide** or this **MySensors guide** in case you are not very famaliar with the matter.
Connect USB FTDI to the Node to turn it into Serial Gateway.

**Step 3.**
****
Connect Gateway Serial into any RaspberryPi USB.

**Step 4.**
****

Add the Gateway Serial as new [hardware device](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/domotizHrdwre.jpg?raw=true) 