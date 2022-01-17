#include "ThingSpeak.h"
#include "WiFi.h"
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"


#define pin2 32
const int YL69Pin = 34;

const char* ssid = "Central";                       
const char* password = "AYDTB72JNTQ";               
unsigned long channelID = 625087;                
const char* WriteAPIKey = "D5ILPJV2UPB3X5GB";     

WiFiClient cliente;

DHT dht2(pin2, DHT22); 
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(115200);
  Serial.println("Test de sensores:");

  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conectado!");

  ThingSpeak.begin(cliente);

  dht2.begin();

  bmp.begin();
}

void loop() {
 int lectura = analogRead(A0);
  delay(5000);
  leerdht2();

  delay(5000);
  leerbmp();

  delay(5000);
  leerhumedad();

  ThingSpeak.writeFields(channelID,WriteAPIKey);
  Serial.println("Datos enviados a ThingSpeak!");
  delay(3400);
}


void leerdht2() {
  
  float t2 = dht2.readTemperature();
  float h2 = dht2.readHumidity();

  while (isnan(t2) || isnan(h2)){
    Serial.println("Lectura fallida en el sensor DHT22, repitiendo lectura...");
    delay(2000);
    t2 = dht2.readTemperature();
    h2 = dht2.readHumidity();
  }

  Serial.print("Temperatura DHT22: ");
  Serial.print(t2);
  Serial.println(" ºC.");

  Serial.print("Humedad DHT22: ");
  Serial.print(h2);
  Serial.println(" %."); 

  Serial.println("-----------------------");

  ThingSpeak.setField (1,t2);
  ThingSpeak.setField (2,h2);
}
void leerbmp() {

  float presion = bmp.readPressure()/100;
  float altitud = bmp.readAltitude();

  Serial.print("Presión bmp: ");
  Serial.print(presion);
  Serial.println(" Pa.");

  Serial.print("Altitud bmp: ");
  Serial.print(altitud);
  Serial.println(" metros.");

  Serial.println("-----------------------");  

  ThingSpeak.setField (3,presion);
  ThingSpeak.setField (4,altitud);
}

void leerhumedad(){
  // ** YL-69 moisture ***
  int const readYL69value = analogRead(YL69Pin);
  // map inversely to 0..10%
  int const convertedPercentage = map(readYL69value, 4095, 1200, 0, 100);
  Serial.print("Moisture (YL-69): ");
  Serial.print(convertedPercentage);
  Serial.print("%\n");
    ThingSpeak.setField (5,readYL69value);
} 

//BMP280
//Conectar a 3V
//El sensor BMP180 conecta el SCK al puerto D22
//El sensor BMP180 conecta el SDA al puerto D21



//DHT22
//Conectar a 5V
//El sensor DHT22 conecta al puertoD32

//Sensor humedad del suelo
//Conectar a 5v
//El sensor de humedad del suelo al puertoD32
