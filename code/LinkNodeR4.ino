#include "FS.h"
#include <ESP8266WiFi.h>
#include <WString.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>    

String data;
String deviceID;
String FactoryKey;
String apikey;
bool config_flag = true;
WiFiClient client;
const char* server = "www.linksprite.io";

void config_wifi();
void query();
void updatetime(String a);
int RegisterDevice();
void config_gpio();

void setup()
{
  Serial.begin(115200); 
  /*Read the factory_key and deviceID first*/
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);
  /*this opens the file "data.txt" in read-mode*/
  File f = SPIFFS.open("/data.txt", "r");
  if (!f) {
    Serial.println("File doesn't exist yet. Creating it");
  /* Configure the factory_key and deviceID via serial
   *  and save it to data.txt
   * example data:
   * 0080000003b124b62d-03a4-4087-bb30-fd118ad10ff
   */  
    while(config_flag){   
      Serial.println("Start to configure...");
      if(Serial.available() > 0)
      {
          data = Serial.readStringUntil('\n');
      }
      /*check the '-' character, if get '-', then configuration is done!*/
      if(data.lastIndexOf('-')!=-1){
        config_flag = false;
        Serial.println("Configuration is finished!");
      }
      delay(1000);
    }
    /* open the file in write mode*/
    File f = SPIFFS.open("/data.txt", "w");
    if (!f) {
      Serial.println("file creation failed");
    } 
    /* now write two lines in key/value style with  end-of-line characters*/
    f.println(data);
  } else {
    // we could open the file
    while(f.available()) {
      //Lets read line by line from the file
      data = f.readStringUntil('\n');      
    }
  }  
 /*parse the device ID and factory from data.txt file*/
 deviceID = data.substring(0,10);
 String va = data.substring(10);
 FactoryKey = va.substring(0,36);
 Serial.println("deviceID:");
 Serial.println(deviceID);
 Serial.println("FactoryKey:");
 Serial.println(FactoryKey);
 f.close();
 config_wifi();
 config_gpio();
 while(RegisterDevice()!=0);
}

void loop() 
{
    query(); 
    delay(1000);
}

void config_gpio()
{
   pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
   pinMode(14, OUTPUT);
   pinMode(16, OUTPUT);  
}

void config_wifi()
{
   WiFiManager wifiManager;
   wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
   wifiManager.autoConnect("LinkNodeAP");
   Serial.print("WiFi Connected ...\n");
   Serial.println("WiFi connected");
}

void query()
{
 if (client.connect(server,80)) 
   {  
   String  postStr ="{";
         postStr +="\"action\":\"query\",";
         postStr +="\"apikey\":\"";
         postStr += apikey;
         postStr +="\",";
         postStr +="\"deviceid\":\"";
         postStr += deviceID;
         postStr +="\",";
         postStr += "\"params\":";
         postStr += "[";
         postStr += "\"relays\"";
         postStr +="]";
         postStr +="}";
    client.print("POST /api/http HTTP/1.1\n");
    client.print("Host: ");
    client.print(server);
    client.print("\nContent-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);    
  }
 delay(200);
 Serial.println("Store response...");
 String request = "";
 while (client.available()) 
 {
   char c = client.read();
   request +=c;
 } 
 if (request!= NULL)
 {
   int index1 = request.indexOf(":{");
   int index2 = request.indexOf("},");
   int index3 = request.indexOf("GMT");
   String tim = request.substring(index3-9,index3);
   String param = request.substring(index1, index2 + 1);
   Serial.print("The param is:");
   Serial.println(param);
   Serial.print("The time is:");
   Serial.println(tim);
   if(param[12]=='1')
      digitalWrite(12,HIGH);
   else
      digitalWrite(12,LOW);
   if(param[13]=='1')
      digitalWrite(13,HIGH);
   else
      digitalWrite(13,LOW);
   if(param[14]=='1')
      digitalWrite(14,HIGH);
   else
      digitalWrite(14,LOW);
   if(param[15]=='1')
      digitalWrite(16,HIGH);
   else
      digitalWrite(16,LOW);
   Serial.println(param[12]);
   Serial.println(param[13]);
   Serial.println(param[14]);
   Serial.println(param[15]);
   client.stop();
   Serial.println("Waiting...");    
   updatetime(tim);
   delay(500);  
 }
}

void updatetime(String a)
{
  if (client.connect(server,80)) 
   {  
     String  postStr1 ="{";
           postStr1 +="\"action\":\"update\",";
           postStr1 +="\"apikey\":\"";
           postStr1 += apikey;
           postStr1 +="\",";
           postStr1 +="\"deviceid\":\"";
           postStr1 += deviceID;
           postStr1 +="\",";
           postStr1 +="\"params\":";
           postStr1 +="{";
           postStr1 +="\"time\":\"";
           postStr1 +=a;
           postStr1 +="\"\r\n";
           postStr1 +="}";
           postStr1 +="}";
    client.print("POST /api/http HTTP/1.1\n");
    client.print("Host: ");
    client.print(server);
    client.print("\nContent-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(postStr1.length());
    client.print("\n\n");
    client.print(postStr1);  
  }
 delay(200);
 Serial.println("Store response...");
 String request = "";
 while (client.available()) 
 {
   char c = client.read();
   request +=c;
 } 
 if (request!= NULL)
 {
   int index1 = request.indexOf(":{");
   int index2 = request.indexOf("},");
   String param = request.substring(index1, index2 + 1);
   Serial.print("The update param is:");
   Serial.println(param);
   client.stop();   
 } 
}

int RegisterDevice()
{
    if (client.connect(server,80)) 
   {  
     String  postStr2 ="{";
           postStr2 +="\"action\":\"register\",";
           postStr2 +="\"deviceid\":\"";
           postStr2 += deviceID;
           postStr2 +="\",";
           postStr2 +="\"apikey\":\"";
           postStr2 += FactoryKey;
           postStr2 +="\"}";
    client.print("POST /api/http HTTP/1.1\n");
    client.print("Host: ");
    client.print(server);
    client.print("\nContent-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(postStr2.length());
    client.print("\n\n");
    client.print(postStr2);  
    Serial.print(postStr2);
  }
 delay(200);
 Serial.println("Store response...");
 String request = "";
 while (client.available()) 
 {
   char c = client.read();
   request +=c;
 } 
 if (request!= NULL)
 {
   int index1 = request.indexOf(":{");
   int index2 = request.indexOf("},");
   String param = request.substring(index1, index2 + 1);
   Serial.println(param);
   int index3 = param.indexOf("apikey");
   int index4 = param.indexOf("deviceid");
   apikey = param.substring(index3+9, index3+45);
   String id = param.substring(index4+11, index4 +21);
   Serial.print("KEY:");
   Serial.println(apikey);
   Serial.print("ID:");
   Serial.println(id);
   client.stop();   
   int len=param.length();
   if(len>=250)
   {
    Serial.println("RegisterDevice OK!\n");
    return 0;
   }
 } 
 return -1;
}
