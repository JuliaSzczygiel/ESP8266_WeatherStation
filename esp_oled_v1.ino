#include "DHT.h"
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h> //librares

#define SCREEN_WIDTH 128 // OLED display width, in pixels 
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_DIN   D7
#define OLED_CLK   D5
#define OLED_DC    D4
#define OLED_CS    D8
#define OLED_RESET D0 // Declaration for SSD1306 display connected using software SPI

#define DHTPIN 4   // What digital pin the DHT22 is conected to
#define DHTTYPE DHT22 // Declaration type of DHT sensor 
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_DIN, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "MALCOME-PARTER"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Jmd19770430!"; // The password of the Wi-Fi network

void setup() {
  Serial.begin(9600); // Start the Serial communication to send messages to the computer
  Serial.setTimeout(2000);
  dht.begin();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC))
  {
   Serial.println(F("SSD1306 allocation failed"));
   for(;;); // Don't proceed, loop forever
  }
  
  while(!Serial) { }
    
  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");

  WiFi.begin(ssid, password); // Connect to the network            
  Serial.print("Connecting to ");
  
  while(WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print(".");
  }
  Serial.println('\n');
  
  Serial.print("Connected, IP address: ");  
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
  
  display.clearDisplay();
}

int timeSinceLastRead = 0;

void loop() {
  
  if(timeSinceLastRead > 5000) { // Report every 5 seconds.
    float h = dht.readHumidity();
    float t = dht.readTemperature();  //read temperature and humidity
    
  if(isnan(h) || isnan(t)) { 
    Serial.println("Failed to read from DHT sensor!"); 
    timeSinceLastRead = 0;
    return;
    } // Check if any reads failed and exit early (to try again).
    
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(25,0);
  display.setTextSize(1);
  display.println("Malcome Smart");
  display.setCursor(120,20);
  display.println("o");
  display.setCursor(0,24);
  display.setTextSize(2);
  display.println("Temp:");
  display.setCursor(58,24);
  display.setTextSize(2);
  display.println(t);
  display.setCursor(0,40);
  display.println("Wilg:");
  display.setCursor(120,38);
  display.setTextSize(1);
  display.println("%");
  display.setCursor(58,40);
  display.setTextSize(2);
  display.println(h);
  display.display();

  timeSinceLastRead = 0; 
  }
  delay(100);
  timeSinceLastRead += 100;
}
