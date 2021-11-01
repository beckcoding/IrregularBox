#include <SPI.h>
#include <SD.h>
char timedatetempbuf[65];//time and date string buffer
File myFile;
const int chipSelect = 4;
#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_ADDR   0x3C
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
DS3231 Clock;
bool Century=false,h12,pm;

char mon[12][4] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
Adafruit_SSD1306 display(-1);

#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

double temp_amb;
double temp_obj;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
double average;
int i = 0;

void initial(){
  digitalWrite(7, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(8, LOW);
}
void distcalc(){
 // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm"); 
}
void avtempcalc(){
for (int i=0; i < 10; i++) {
 average = average + temp_obj;
 }
 average = average/10;
 }
void normalfeverind(){
if (average >= 30) // Turns LED RED fever
  { digitalWrite(7, HIGH); }
  else {
  digitalWrite(7, LOW); } 

if (average > 24 && average < 29) // Turns LED Normal temp Green
  { digitalWrite(6, HIGH); }
  else {
  digitalWrite(6, LOW); }
 }
void savetemp(){
  myFile = SD.open("temp.txt",FILE_WRITE);
  if (myFile){
  Serial.print(average);
  
  myFile.println(average);
  myFile.print("average temp");
  myFile.close();}
  
}


void setup() {
   Serial.begin(9600);
   pinMode(10,OUTPUT);
   Wire.begin();// inicializa bus I2C
   Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  
  
  delay(3000);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
  mlx.begin(); 
  pinMode(5, OUTPUT);// sets Blue LED as an OUTPUT
  pinMode(6, OUTPUT);// sets Green LED as an OUTPUT
  pinMode(7, OUTPUT);// sets Red LED as an OUTPUT
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode (8, OUTPUT) ; // sets the pin 4 laser as an OUTPUT
}

void loop() {
  
  initial();
  distcalc();
 if (distance > 4 && distance < 8) // Turns LED on between 6cm and 12 cm
  { digitalWrite(8, HIGH); }
  else {
  digitalWrite(8, LOW); }
  delay (3000);
  if (distance > 4 && distance < 8)//starts measuring temperature
  {
  temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempC(); 
  avtempcalc();
  
  normalfeverind();
  
  delay (2000);
}
 else {
  temp_amb = 0;
  temp_obj = 0;  
  average = 0;
  }
   /*savetemp();*/
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(30,0);
 
  display.print("  ");
  display.print(mon[Clock.getMonth(Century)-1]);
  display.print(" ");
  display.print(Clock.getDate());
  display.print("  20");
  display.println(Clock.getYear());
  display.print("    ");
  display.print(Clock.getHour(h12, pm));
  display.print(" : ");
  display.print(Clock.getMinute());
  display.print(" : ");
  display.print(Clock.getSecond());
  display.println(" HRS");
  display.print("Amb Temp: ");
  display.print(temp_amb);
  display.println("  C");
  display.print("Skin temp: ");
  display.print(average);
  display.print("  C");
  display.display();
  
  Serial.print("  ");
  Serial.print(mon[Clock.getMonth(Century)-1]);
  Serial.print(" ");
  Serial.print(Clock.getDate());
  Serial.print("  20");
  Serial.println(Clock.getYear());
  Serial.print("    ");
  Serial.print(Clock.getHour(h12, pm));
  Serial.print(" : ");
  Serial.print(Clock.getMinute());
  Serial.print(" : ");
  Serial.print(Clock.getSecond());
  Serial.println("Amb Temp: ");
  Serial.print(temp_amb);
  Serial.print("    ");
  Serial.println("  C");
  Serial.println("Skin temp: ");
  Serial.print(average);
  Serial.print("  C");
 
  delay(1000);
  
}
