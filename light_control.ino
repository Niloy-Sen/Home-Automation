#include <ThingSpeak.h>   //importing the thingspeak library
#include <ESP8266WiFi.h>   //importing the ESP8266 WiFi library
int Relay=6;
const char* ssid="Autohome";    //WiFi Name
const char* password="werds4573";  //WiFI Password

char* server="api.thingspeak.com"; //URL for thingspeak cloud
unsigned long channel_id= 1078485;  //Channel ID
char* APIkey="LBGBKGEJNHGOM155";    //API key of the channel
unsigned field=1;             //Since we have to find the value of field 1

float Value=0.0;
WiFiClient client;       //Creates a client that can connect to a specified internet IP address 
void setup() {
  Serial.begin(9600);     //Begins the Serial Monitor
  Serial.print("Starting ");
  Wificonnect();     //calls the function for the connection of the WiFi
  pinMode(Relay,OUTPUT);  //Initialising the Relay Pin

  Value=readData(channel_id,field); //stores the value received from readData function
  Serial.println(Value);    //prints the value
  delay(1000);

  if(Value==0)
  {
    digitalWrite(Relay,LOW);      //If value=0, relay is not triggered
  }
  else if(Value==1)
  {
    digitalWrite(Relay,HIGH);   //If value=1, relay is triggered
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
int Wificonnect()
{
  WiFi.begin(ssid,password); //Initialises the WiFi Module
  while(WiFi.status() != WL_CONNECTED)   
  {
    Serial.print(".");      //waits for the proper WiFi Connection
    delay(100);
  }
  Serial.println("WiFi Connected"); //Prints an appropriate message when the connection is proper
  ThingSpeak.begin(client);    //Connection with ThingSpeak Cloud
}
float readData(long TS_channel,unsigned int TSfield)
{
  float Data=ThingSpeak.readFloatField( TS_channel,TSfield,APIkey); //reads the Field value from the ThingSpeak Cloud
  return Data;      //Returns the data
}
