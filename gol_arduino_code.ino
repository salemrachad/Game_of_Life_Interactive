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

#include <SPI.h>

int trigPin = 12;
int echoPin = 13;

// function prototype to define default timeout value
static unsigned int newPulseIn(const byte pin, const byte state, const unsigned long timeout = 1000000L);
// using a macro to avoid function call overhead
#define WAIT_FOR_PIN_STATE(state) \
  while (digitalRead(pin) != (state)) { \
    if (micros() - timestamp > timeout) { \
      return 0; \
    } \
  }
static unsigned int newPulseIn(const byte pin, const byte state, const unsigned long timeout) {
  unsigned long timestamp = micros();
  WAIT_FOR_PIN_STATE(!state);
  WAIT_FOR_PIN_STATE(state);
  timestamp = micros();
  WAIT_FOR_PIN_STATE(!state);
  return micros() - timestamp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//Pause Button Setup

const int buttonPin = 2;
const int buttonPin10 = 10;

int buttonState;
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonState10;
int lastButtonState10 = LOW;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//LED Setup

const int ledPin9 = 9;
const int ledPin3 = 3;
const int ledPin5 = 5;

int ledState9 = LOW;
int ledState3 = LOW;
int ledState5 = LOW;

//brightness of the led

float bright;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//2d array

boolean array1[WIDTH][HEIGHT] ;
boolean cellbuffer[WIDTH][HEIGHT];

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Probability

float Probability = 45;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variables for timer

boolean pause = false;
int interval = 200;
int lastRecordedTime = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//Variable for State
//implementing 2 states
//state 0 is waiting to receive from processing
//state 1 is playing

int state = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  //Starting Dot Matrix
  pixels.begin();
  pixels.show();
  initializeCells();
}

void loop() {

  pauseButton();
  resetButton();
  offonPauseG();
  pauseGame();

}


void initializeCells() {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      float state = random (100);
      if (state > Probability) {
        state = 0;
      }
      else {
        state = 1;
      }
      array1[i][j] = int(state); // Save state of each cell
    }
  }
}
void GameofLife_ram() {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      cellbuffer[i][j] = array1[i][j];
    }
  }
  // Visit each cell:
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      int neighbours = 0; //  count the neighbours
      for (int ii = i - 1; ii <= i + 1; ii++) {
        for (int jj = j - 1; jj <= j + 1; jj++) {
          if (((ii >= 0) && (ii < WIDTH)) && ((jj >= 0) && (jj < HEIGHT))) { //check if out of bounds
            if (!((ii == i) && (jj == j))) { // Make sure to to check against self
              if (cellbuffer[ii][jj] == 1) {
                neighbours ++; // Check alive neighbours and count them
              }
            } // End of if
          } // End of if
        } // End of jj loop
      } //End of ii loop

      // We've checked the neigbours - apply rules
      if (cellbuffer[i][j] == 1) { // The cell is alive: kill it if
        if (neighbours < 2 || neighbours > 3) {
          array1[i][j] = 0; // Die unless it has 2 or 3 neighbours
        }
      }
      else { // The cell is dead: make it live if necessary
        if (neighbours == 3 ) {
          array1[i][j] = 1; // Only if it has 3 neighbours
        }
      } // End of if
    } // End of j loop
  } //End of i loop
} //End of function

void statePlot() {
  int alive;
  //int bright1 = proxibrightness();
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
  pixels.show();
  if (alive <= int(NUMPIXELS * 0.038)) {
    initializeCells();
  }
}

void pauseButton() {
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  int reading = digitalRead(buttonPin);
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle Pause if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState3 = !ledState3;
        pause = !pause;
        ledState5 = LOW;
        digitalWrite(ledPin5, ledState5);
        //Serial.print("pushed");
      }
    }
  }
  lastButtonState = reading;
  digitalWrite(ledPin3, ledState3);
}

void resetButton() {

  int reading1 = digitalRead(buttonPin10);
  // If the switch changed, due to noise or pressing:
  if (reading1 != lastButtonState10) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading1 != buttonState10) {
      buttonState10 = reading1;

      // only toggle Pause if the new button state is HIGH
      if (buttonState10 == HIGH) {
        initializeCells();
        //Serial.print("Reset");
      }
    }
  }
  lastButtonState10 = reading1;
}

void offonPauseG() {
  if (pause == false) {
    ledState5 = HIGH;
    digitalWrite(ledPin5, ledState5);
  } else if (pause == true) {
    ledState5 = LOW;
    digitalWrite(ledPin5, ledState5);
  }
}
void pauseGame() {
  if (millis() - lastRecordedTime > interval) {
    if (!pause) {
      GameofLife_ram();
      statePlot();
      lastRecordedTime = millis();
    }
  }
}




//void portTest() {       test1
//
//  pinMode(ledPin9, OUTPUT);
//
//  //  RECEIVE DATA FROM PROCESSING CODE TO USE
//  // Wiring/Arduino code:
//
//  while (Serial.available()) { // If data is available to read,
//    serVal += Serial.read();
//  }
//  for (int i = 0; i < cols; i++) {
//    for (int j = 0; j < rows; j++) {
//      int index = serVal.indexOf(","); //We find the next comma
//      array1[i][j] = atol(serVal.substring(0, index).c_str());
//      serVal = serVal.substring(index + 1); //Remove the number from the string
//    }
//  }
////    if (serVal == 'r') {
////      digitalWrite(ledPin9, HIGH);
////      delay(150);
////      digitalWrite(ledPin9, LOW);
////      serVal = "";
////    }
////    else {
////      digitalWrite(ledPin9, LOW);
////    }
//}

//void establishContact() {
//  while (Serial.available() <= 0) {
//    Serial.write('A');   // send a capital A
//    delay(300);
//  }
//}

//int proxibrightness() {
//  long duration, distance;
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//  duration = newPulseIn(echoPin, HIGH);
//  distance = (duration / 1) / 29.1;
//  duration = newPulseIn(echoPin, LOW);
//  delay(10);
//
////  int average=0;
////  int bright;
////  for (int i = 0; i < 20; i++) {
////    average = average + distance;
////  }
////  average = average / 20;
////  if (average < 0) {
////    average = 0;
////  } else if (average > 400) {
////    average = 400;
////  }
////  if (average < 100) {
////    bright = bright + 2;
////    if (bright > 100) {
////      bright = 100;
////    }
////  }
////  if (average > 100) {
////    bright = bright - 2;
////    if (bright < 0) {
////      bright = 0;
////    }
////  }
//  //mapping reverse range for bright to be at max when close and minimum at far.
//  bright = map(distance,0,400,40,0);
//  return bright;
//}
