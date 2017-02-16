#include <Adafruit_NeoPixel.h>

//define the pin number, number of neopixels and initialize the neopixel controller object
#define PIN            10
#define NUMPIXELS      4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel onboard = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
int health = 50;
long total = 0;
int count = 0;
const int sampleRate = 500;
const int thresh = 50; //change this after playtesting

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(A9, INPUT_PULLUP);
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  onboard.begin();

  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0,255,0));
    pixels.show();
    onboard.setPixelColor(i, pixels.Color(0,255,0));
    onboard.show();
  }
}

// the loop routine runs over and over again forever:
void loop() {
  //------------Bend Sensor Readings------------------//
  int sensorValue = analogRead(A9);
  sensorValue += analogRead(A11);
  Serial.println(health);
  //delay(1);
  //------------Bend Sensor Readings------------------//

  //------------Health Calculations------------------//
  if(count == sampleRate){
    int avg = total/count;
    if(avg < thresh){
      int damage = map(avg, 1, thresh, 1, 10);
      health -= damage;
    }
    count = 0;
    total = 0;
  }
  else{
    total += sensorValue;
    count++;
  }
  //------------Health Calculations------------------//

  //------------Neopixel Output--------------------//
  if( health < 0){
     for(int i=0;i<NUMPIXELS;i++){
       pixels.setPixelColor(i, pixels.Color(255,0,0)); // red.
       pixels.show(); // This sends the updated pixel color to the hardware.
       onboard.setPixelColor(i, pixels.Color(255,0,0)); // red.
       onboard.show(); // This sends the updated pixel color to the hardware.
     }
  }
  //------------Neopixel Output--------------------//
  
}
