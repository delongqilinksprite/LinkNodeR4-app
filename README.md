# How to use Android APP to control LinkNode R4

LinkNode R4 is an open source 4-Channel relay controller which is powered by the ESP8266 WiFi SoC.

![](images/r4.png)

The following will introduce how to use Android APP to control the LinkNode R4.

## Steps

#### 1. Install Android APP
* Download the APK file from the [github](https://github.com/delongqilinksprite/LinkNodeR4-app.git) and install it.
* if your equipment is iphone/ipad,you can get the app form appstore and name is "LinkNode Relay".  


#### 2. Login LinkSpriteIO
* If you never register an account on LinkSpriteIO, please enter Email and your password, then click the **SIGN IN** button
* If you have an account on LinkSpriteIO, please enter Email and your password, then click the **REGISTER** button

![](images/2.png)

#### 3. Go to Relay device list
* Click the plus icon on the upper right quarter to add a new LinkNode R4.

![](images/3.png)

#### 4. Scan the QRcode
* Get the device information and register it to your account.
![](images/qar4.png)
![](images/4.png)

#### 5. Configure to accece WiFi AP
* Supply the power for LinkNodeR4, the LinkNode R4 will create a AP called **LinkNodeAP**
* Use your mobile phone to connect this AP
* Open a browser and enter the ip address **10.0.1.1** and you will see the following website:

![](images/ap.png)    
  
* Click the button **configure WiFi**
* Select your WiFi AP which you want to connect and enter your wifi password.
* If connecting failed, you can go to the same website to configure it again.
* After that, LinkNode R4 will connect to Linksprite IO via the internet.


#### 6. Control your 4-channel relay
* control the button on the right side to turn on or off the relays, and you can click the relay's name to change it.

![](images/5.png)
