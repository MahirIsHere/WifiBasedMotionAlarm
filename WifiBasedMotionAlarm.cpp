#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "orchi joy bijoy"
#define STAPSK  "OJBI7396"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

boolean PIRstate ; //variable to store PIR state
boolean lastPIRstate = HIGH;
int PIR = 0; //PIR connected to GPIO 0
int OUTPUTPIN = 2; //For LED Output

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //A Loop until it is Connected to the Wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); //A delay is set for Serial Output, so that it doesn't hang
    Serial.print(".");
  }

  //Wifi Is Connected, Output is Printed on Serial Monitor
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //PIR Pin is Setup for Input
  pinMode(PIR, INPUT); 
  //Last Input is taken
  lastPIRstate = digitalRead(PIR);
  //An Output Pin is Setup for LED
  pinMode(OUTPUTPIN, OUTPUT);

  delay(2000);
  //A notification is sent for test
  sendNotification();

  //A Delay is set so that the Sensor gets time to Calibrate
  delay(3000);
}

void loop() {
  PIRstate = digitalRead(PIR);  //HIGH when motion detected, else LOW
  
  if (PIRstate != lastPIRstate)  //Checking if there is any motion
  {
    Serial.println("Motion Detected");
    //Once Motion is Detected,
    //LED will light up
    digitalWrite(OUTPUTPIN, HIGH);
    //Notification is Sent
    sendNotification();
    
    delay(100);

    //After a Delay, LED is Turned off
    digitalWrite(OUTPUTPIN, LOW);
    //Last State of PIR is Stored
    lastPIRstate = PIRstate;
  }
}

//A Function for Noticiation Sending
void sendNotification(){
  //If Wifi is Connected
  if (WiFi.status() == WL_CONNECTED)  //Check WiFi connection status
  {
      HTTPClient http; //Declare an object of class HTTPClient
      if(http.begin("http://maker.ifttt.com/trigger/iot_Motion_Sensor/with/key/b5dYCmIwYQBHBFD6myBdrIDmpvwOFzmbKSUpk_LT41Y")){  //Specify request destination
         http.GET();   //Send the request
         http.end();   //Close connection
         Serial.println("Notification Sent");
       }else{
         Serial.println("Maker.ifttt.com Connection failed, Can't send notificaition");
       }
  }
}