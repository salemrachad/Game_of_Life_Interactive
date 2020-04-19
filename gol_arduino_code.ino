//////////////////////////////////////////////////////////////////////////////////////////////////////////
//115200 baud speed
#include <Adafruit_NeoPixel.h>

//DEFINE MATRIX 32x32
#define WIDTH 32
#define HEIGHT 32
#define PIN 6
#define NUMPIXELS 1024

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//color of the dots ({red, green, blue} max value 255 each]
int colorOfPattern[3];
//color of background
int colorOfBackground[3];

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//LED Setup

const int ledPin9 = 9;


int ledState9 = LOW;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//2d array

boolean array1[WIDTH][HEIGHT] ;
boolean cellbuffer[WIDTH][HEIGHT];
boolean incomingArray[NUMPIXELS];

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Probability

float Probability = 45;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variables for timer

boolean pause = false;
int interval = 200;
int lastRecordedTime = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//

const int buttonPin11 = 11;


int buttonState11;
int lastbuttonState11 = LOW;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

//////////////////////////////////////////////////////////////////////////////////////////////////////////

boolean firstContact = false;
int inByte = 0;

void setup() {

  Serial.begin(115200);

  //Starting Dot Matrix
  pixels.begin();
  pixels.show();

}

void loop() {

  buttonsendprocessingRequest();

  SerialRequest();

  Convert1dArray();

  statePlot();
}



void statePlot() {
  int alive;
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {

      if (j & 0x1) { //is odd
        if (array1[i][j] == 1) {
          pixels.setPixelColor(((j * WIDTH) + (WIDTH - 1 - i)), pixels.Color(25, 0, 25));
          alive++;
        }
        else {
          pixels.setPixelColor(((j * WIDTH) + (WIDTH - 1 - i)), pixels.Color(0, 0, 0));
        }
      }
      else {
        if (array1[i][j] == 1) {
          pixels.setPixelColor(i + (j * WIDTH) , pixels.Color(25, 0, 25));
          alive++;
        }
        else {
          pixels.setPixelColor(i + (j * WIDTH), pixels.Color(0, 0, 0));
        }
      }
    }
  }
}

void buttonsendprocessingRequest() {

  inByte = Serial.read();
  int reading = digitalRead(buttonPin11);
  // If the switch changed, due to noise or pressing:
  if (reading != lastbuttonState11) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState11) {
      buttonState11 = reading;

      // only toggle Pause if the new button state is HIGH

      if (buttonState11 == HIGH) {
        Serial.clear();
        Serial.write('A');   // send a capital A
        delay(500);
      }
    }
  }
  lastbuttonState11 = reading;
}

void SerialRequest() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
      incomingArray[i] = Serial.read();
    }
  }
}

void Convert1dArray(){
  int s =0;
  for (int i=0; i <WIDTH;i++){
    for (int j =0;j<HEIGHT;j++){
      incomingArray[s]=array1[i][j];
      s++;
    }
  }
  Serial.clear();
}
