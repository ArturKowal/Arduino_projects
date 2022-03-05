#include<LiquidCrystal.h>  // biblioteka wyswietlacza
#include <DHTesp.h> //czujnik temp / wilgoci
// czujnik zewnetrzny
#include <OneWire.h>
#include <DallasTemperature.h>
// czujnik zanieczyszczen
#include <DS18B20.h>
//czujnik cisnienia
#include <Wire.h>
#include <BMP180.h>


//czujnik cisnienia
//BMP180 myBMP(BMP180_ULTRAHIGHRES);

#define DHT11_PIN 12    //przypisanie pinu 12 Arduino jako odczyt z sensora czujnik temp / wilgoci
const int buzzer = 13; //buzzer to arduino pin 13

//Wyświeltacz
const int rs =7, en =6, d4=5,d5=4,d6=3,d7=2; // piny wyświetlacza 
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

byte celcjusz[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// ludek
byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};
byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

// czujnik wewnętrzny temp i hum
DHTesp dht ;

// czujnik powietrza
int a0 = A0; // pin A0

// czujnik zewnętrzny
OneWire oneWire(8); // pin 8
DallasTemperature sensors(&oneWire);


void setup(){
  // czujnik cISnienia
  //delay(5000);
  //myBMP.begin();
  
  // czujnik zewnętrzny
  sensors.begin();

  // czujnik powietrza
  pinMode (a0, INPUT);
  
  //Wyświetlacz:
  lcd.begin(16,2);
  //lcd.home();
  //lcd.print("Dziala...");
  //ludek
  lcd.createChar(3, armsDown);
  lcd.createChar(4, armsUp);

  // celcjusz
  lcd.createChar(5, celcjusz);

  //czujnik wewnętrzny 
  int dhtPin = 12;
  dht.setup(dhtPin, DHTesp::DHT11);
  
  //buzzer
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  
}

void loop(){
 
  // czujnik zewnetrzny
  sensors.requestTemperatures(); // czujnik zewnętrzny

  //czujnik jakości powietrza
  int zanieczyszczenie = analogRead(a0);
  
  // czujnik wewnątrz
  int temperatura = dht.getTemperature();
  int wilgotnosc = dht.getHumidity();

  //buzzer przerywa gra
  //tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec

  // wyswietlacz
  lcd.setCursor(0,0);
  lcd.print("in:");
  lcd.print(temperatura);
  lcd.write(5);
  lcd.print("C hum:");
  lcd.print(wilgotnosc);
  lcd.print("%");
  
  lcd.setCursor(0,1);       
  lcd.print("out:");
  lcd.print(round(sensors.getTempCByIndex(0))); // czujnik zewnętrzny
  lcd.write(5);
  lcd.print("C PM10:");
  lcd.print(zanieczyszczenie/10);

  //ludek
  int sensorReading = analogRead(A0);
  int delayTime = map(sensorReading, 0, 1023, 200, 1000);
  lcd.setCursor(15, 1);
  lcd.write(3);
  delay(delayTime);
  lcd.setCursor(15, 1);
  // draw him arms up:
  lcd.write(4);
  delay(delayTime);
}
  
