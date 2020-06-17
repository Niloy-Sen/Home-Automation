#include<SoftwareSerial.h>     //Including the software serial library
SoftwareSerial espSerial(2,3);      //the connections to the ESP8266 WiFi Module
int Relay= 6;                      //connection of the relay with Arduino Uno

String output="";
String wifiname="Autohome";       //WiFi Name
String pass="wert234";            //WiFI Password

String sendData="GET/channels/1078485/fields/1.json?api_key=YAQ3JJABHSOM8VTC&results=1";  //Stores the API key to receive data from Thingspeak Cloud
void setup() {
  Serial.begin(9600);         //Starting the serial Monitor
  espSerial.begin(9600);      //Starting the WiFi Module
  pinMode(Relay,OUTPUT);      //The relay pin is to be used as output
  espCommand("AT+RST",1000);       //Reset the WiFi Module
  espCommand("AT+CWMODE = 1",1000); //Configuring it to be a client
  espCommand("AT+CWJAP=\""+wifiname+"\",\""+pass+"\"",1000);   //Setting up the WiFi name and password
  while(!espSerial.find("OK")) 
    {
       //Here it is waiting for proper connection
    }
  Serial.println("Wifi Connected");      //Once the WiFi is connected, an appropiate message is shown
  delay(1000);
  
  espCommand("AT+CIPMUX =0",1000);       //The module will only be connected to 1 device
  espCommand("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80",1000); //Connecting to the Thingspeak Cloud
  espCommand("AT+CIPSEND="+String(sendData.length()+4),1000);   //Length of the URL
  
  delay(100);
  output="";
  espCommand(sendData,1000);     //sending the URL
  delay(100);

  int l=output.length();   //stores the length of the incoming String
  char c=output.charAt(l-5); //The field value is always present at the (l-5) index position
  Serial.print("The Response is: ");    
  Serial.println(c);         //prints the field value
  if(c=='0')
  {
    digitalWrite(Relay,LOW);   //when the field va;ue is zero, the relay is not triggered
  }
  else if(c=='1')
  {
    digitalWrite(Relay,HIGH);  //when the field va;ue is 1, the relay is triggered to switch on the light
  }
}

void loop() {
                //the loop is kept empty. This is because, we don't want to continuosly read the data

}
void espCommand(String command,const int timeout) //this function sends all the ESP commands to the Modules
{
  Serial.print("AT Command ==>");
  Serial.print(command);
  Serial.println(" ");
  espSerial.println(command);


  //The next set of program is only used for the last command so as to read the data from the thinspeak clod
  long int time = millis();

  output=""; //clear the string
  
  while ( (time + timeout) > millis()) //Calculates the small timeout after each command
  {
    while (espSerial.available())
    {
      char i = espSerial.read(); // read one char at a time
      output += i; //Combine char to string 
    }
  }
  
  Serial.println("Received: ");  
  Serial.print(output);      //prints the input received from the thingspeak cloud in the serial monitor
  
}
