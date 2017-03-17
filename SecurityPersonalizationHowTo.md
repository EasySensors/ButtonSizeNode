Security "personalization" How To for MySensors SecurityPersonalizer.ino sketch
------------------------------------------------------------------------


Very detailed explanation you can find here : [Signing support created by Patrick "Anticimex" Fallberg](https://www.mysensors.org/about/signing)
If you do not want to dive deep into it, just follow these **3 steps**:

**Step 1 - generate MY_HMAC_KEY**

> You can skip **Step 1** by filling some numbers instead of zeroes in ```#define MY_HMAC_KEY 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00....... ```


Open SecurityPersonalizer.ino from MySensors examples.
 Comment out lines:
```c++
//#define USER_KEY
//#define SKIP_UART_CONFIRMATION
```
Upload sketch to the node and open serial monitor. Copy #define MY_HMAC_KEY 0x blah blah blah

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
Upload sketch to the node 


Done!
