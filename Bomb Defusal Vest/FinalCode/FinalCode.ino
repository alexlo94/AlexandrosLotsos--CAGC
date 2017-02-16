#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pins 2 and 3 need to be connected to another flora
#define PIXEL_PIN             12
#define BUTTON_B              0
#define BUTTON_W              9
#define BUTTON_R              10
#define BUTTON_G              1

#define NUMPIXELS      4

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int buttonstate_b = 1;
int buttonstate_w = 1;
int buttonstate_r = 1;
int buttonstate_g = 1;

//Game Mechanic Code below
int correct_answer[4];
int user_input[4];
bool pressed = false;
int stage = 0; //value that goes from 0,3 to signify the stage the user is in while submitting input
int game_length = 45000; //time of game, in millis
int previous_seconds_left = game_length / 1000; //keep track of when seconds change to not constantly send time to other flora
int seconds_left = game_length / 1000;
void setup() {
  alpha4.begin(0x70);
  
  randomSeed(analogRead(2));
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(50);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_W, INPUT_PULLUP);
  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(BUTTON_G, INPUT_PULLUP);
  
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }

  for(int i = 0; i < 4; i++){
    randomSeed(i);
    int next = random(0,100) % 4;
    correct_answer[i] = next;
  }
}

void button_pressed(Adafruit_NeoPixel &pixels, bool &pressed, int &stage) {
  pixels.setPixelColor(stage, pixels.Color(155,155,155));
  pixels.show();
  pressed = true;
  stage = stage + 1; 
}

void loop() {
  // send game in progress to other flora
  seconds_left = seconds_left - (millis()/1000);
  if (seconds_left != previous_seconds_left) {
    String timeDisplay = "  " + seconds_left;
    for (int i = 0; i < timeDisplay.length(); i++){
      alpha4.writeDigitAscii(i, timeDisplay.charAt(i));
    }
    alpha4.writeDisplay();
    previous_seconds_left = seconds_left;

    //game over
    if (seconds_left == 0) {
      alpha4.writeDigitAscii(0, 'B');
      alpha4.writeDigitAscii(1, 'O');
      alpha4.writeDigitAscii(2, 'O');
      alpha4.writeDigitAscii(3, 'M');
      alpha4.writeDisplay();
      digitalWrite(motorPin, LOW);
      for (int i = 0; i < 50; i++) {
        for(int i = 0; i < NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(155,0,0));
          pixels.show();
        }
        delay(2000);
        for(int i = 0; i < NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(0,0,0));
          pixels.show();
        }
        delay(2000);
      }
    }
  }
  
  
  Serial.println(stage);
   
  //read buttonStates
  buttonstate_b = digitalRead(BUTTON_B) ;
  buttonstate_w = digitalRead(BUTTON_W) ;
  buttonstate_r = digitalRead(BUTTON_R) ;
  buttonstate_g = digitalRead(BUTTON_G) ;


  //execute appropriately according to previous reads
  if (pressed == false && stage < 4) {
    if (buttonstate_b == LOW){
        user_input[stage] = 0;
        button_pressed(pixels, pressed, stage);
    }
    if (buttonstate_g == LOW){
        user_input[stage] = 1;
        button_pressed(pixels, pressed, stage);
    }
    if (buttonstate_r == LOW){
        user_input[stage] = 2;
        button_pressed(pixels, pressed, stage);
    }
    if (buttonstate_w == LOW){
        user_input[stage] = 3;
        button_pressed(pixels, pressed, stage);
    }
  }
  if(buttonstate_b == HIGH && buttonstate_g == HIGH && buttonstate_r == HIGH && buttonstate_w == HIGH){
    pressed = false;
  }

  //round done
  if(stage >= 4){
    int correct = 0;
    for(int i = 0; i < 4; i++){
      if(user_input[i] == correct_answer[i]){
        pixels.setPixelColor(i, pixels.Color(0,155,0));
        correct = correct + 1;
        Serial.println(user_input[i]);
        Serial.println("correct");
      }
      else {
        pixels.setPixelColor(i, pixels.Color(155,0,0));
        Serial.println(user_input[i]);
        Serial.println("incorrect");
      }
    }
    pixels.show();

    //game won
    if (correct == 4) {
      for (int i = 0; i < 50; i++) {
        for(int i = 0; i < NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(0,155,0));
        }
        pixels.show();
        delay(2000);
        for(int i = 0; i < NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(0,0,0));
        }
        pixels.show();
        delay(2000);
      }
    }

    //reset pixel display
    delay(3000);
    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    pixels.show();
    
    stage = 0;
  }
}

  

