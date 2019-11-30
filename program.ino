#define SensorPin A1            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 12
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

float ttt;



int sensor_pin = A0;
#include "DHT.h"
  float h=0;
   float t=0;
#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);




const int relay=7;
int output_value ;
#include <SoftwareSerial.h>         // including the library for the software serial
#define DEBUG true
SoftwareSerial esp8266(10,11);

void setup() {
  pinMode(relay,OUTPUT);

  // put your setup code here, to run once:
Serial.begin(9600);         // Setting the baudrate at 9600
  esp8266.begin(115200);        // Set the baudrate according to you esp's baudrate. your esp's baudrate might be different from mine
  sendData("AT+RST\r\n",2000,DEBUG);            // command to reset the module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG);       // This will configure the mode as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG);          // This command will get the ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG);       // This will configure the esp for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG);

   Serial.println("Reading From the Sensor ...");
     Serial.println("BlueCore Tech Temperature and Humidity");

  dht.begin();
pinMode(LED,OUTPUT);
 
  Serial.println("pH meter experiment!");
 

   }

void loop() {
 tt();
  mm();
  ph();
if(esp8266.available())         // This command will that check if the esp is sending a message
  {    
 
    if(esp8266.find("+IPD,"))
    {
     delay(1000);
     mm();
     tt();
     int connectionId = esp8266.read()-48; /* We are subtracting 48 from the output because the read() function returns
         mm();
     tt();                                     the ASCII decimal value and the first decimal number which is 0 starts at 48*/


       mm();
     tt();
     ph();
     String webpage = "<h1>Smart plant monitoring system </h1>";
      webpage += "<p>Moisture</p>";
      webpage += output_value ;
       webpage += "<p><br>Temperature</p>";
      webpage += t;
        webpage += "<p><br>Humidity</p>";
      webpage += h;
      webpage += "<p><br>Ph</p>";
      webpage += ttt;
 // webpage+= "FOLLOW GT ONE";
 
       
       
 
       
       webpage += "</h2></p></body>";  
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);    
     String closeCommand = "AT+CIPCLOSE=";
     closeCommand+=connectionId;
     closeCommand+="\r\n";
     sendData(closeCommand,3000,DEBUG);
    }
  }
}

    String sendData(String command, const int timeout, boolean debug)
{
    String response = "";  
    esp8266.print(command);
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        char c = esp8266.read();
        response+=c;
      }  
    }
    if(debug)
    {
      Serial.print(response);
    }
    return response;
}





 

 

   


void mm(){
   output_value= analogRead(sensor_pin);

   output_value = map(output_value,550,0,0,100);

   Serial.print("Mositure : ");

   Serial.print(output_value);

   Serial.println("%");
   if(output_value>20)
   {
   digitalWrite(relay,HIGH);
   
   }
   else
   {
   digitalWrite(relay,LOW);
   }


}
void tt(){
  // Give it time to calibrate
  delay(20000);

 
  h = dht.readHumidity();
  // Read Celsius
  t = dht.readTemperature();
  // Read Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check errors
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.println("Humidity: ");
  Serial.print(h);
  Serial.println(" %.");
  Serial.println("  ");
  Serial.println("Temperature: ");
  Serial.print(t);
  Serial.println(" Degrees ");
 
}




double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}



void ph()
{
   static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
   \illis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    S
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
        ttt=pHValue;
  }
}
