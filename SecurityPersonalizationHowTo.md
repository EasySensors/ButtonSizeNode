Security "personalization" for MySensors SecurityPersonalizer.ino sketch How To
------------------------------------------------------------------------


Very detailed explanation you can find here : [Signing support created by Patrick "Anticimex" Fallberg](https://www.mysensors.org/about/signing)

If you do not want to dive deep into it, just follow these **3 steps**. Assume you know what is the [Arduiono IDE](https://www.arduino.cc/en/main/software) and the Serial Monitor from the Arduiono IDE:

**Step 1 - generate random MY_HMAC_KEY**


> You can skip **Step 1** by filling some numbers instead of zeroes in SecurityPersonalizer.ino ```#define MY_HMAC_KEY 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00....... ```


Open SecurityPersonalizer.ino from MySensors examples.
 Comment out lines:
```c++
//#define USER_KEY
//#define SKIP_UART_CONFIRMATION
```
Upload sketch to the node and open the serial monitor. Copy from the serial monitor #define MY_HMAC_KEY 0x blah blah blah

![open serial monitor and copy #define MY_HMAC_KEY 0x blah blah blah](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/personalization.jpg?raw=true)



**Step 2 - save MY_HMAC_KEY**


Put #define MY_HMAC_KEY  into here 
![Put it here ](https://github.com/EasySensors/ButtonSizeNode/blob/master/pics/personalization1.jpg?raw=true)

**Step 3 - write MY_HMAC_KEY into the Node**


Unconment
```c++
#define LOCK_CONFIGURATION
#define USER_KEY
#define SKIP_UART_CONFIRMATION
```
Upload sketch into the node 

Now you have CryptoAuthentication key written into Atsha204 chip. You can rewrite another one later if you want to change it.

**Done!**

Please note:

Your **Node sketch** you want to protect should have: 
```c++
#define MY_SIGNING_ATSHA204 
#define MY_SIGNING_REQUEST_SIGNATURES
```
your **Gateway Serial** sketch need basically only this one:
```c++
#define MY_SIGNING_ATSHA204
```



