#include <SPI.h>
#include <MaxMatrix.h>


//Proximity sensor setup
#include "SR04.h"
#define TRIG_PIN 12
#define ECHO_PIN 13

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;

//Pin Setup
int DIN = 7;
int CLK = 8;
int CS = 4;

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

//LED Setup
const int ledPin9 = 9;      // the number of the LED pin
const int ledPin3 = 3;      // the number of the LED pin
const int ledPin5 = 5;      // the number of the LED pin
const int ledPin11 = 11;      // the number of the LED pin
int ledState11 = LOW;
int ledState9 = LOW ;
int ledState3 = LOW;
int ledState5 = LOW;

//Value for Serial
String serVal;

//Number of displays in use
int maxInUse = 4;

//2d array
const int rows = 32;
const int cols = 8;

boolean array1[rows][cols] ;
boolean cellbuffer[rows][cols];

//brightness of the led
int bright;

// How likely for a cell to be alive at start (in percentage
float Probability = 40;

boolean pause = false;

// Variables for timer

int interval = 100;
int lastRecordedTime = 0;

MaxMatrix m(DIN, CS, CLK, maxInUse); // using MaxMatrix Library

// Test 1

void setup() {


  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin10, INPUT);
  pinMode(ledPin11, OUTPUT);

  m.init(); //Starting Dot Matrix

  while (!Serial) {
    digitalWrite(ledPin11, HIGH);
    delay(150);
    digitalWrite(ledPin11, LOW);
    delay(150);
  }
  if (Serial) {
    digitalWrite(ledPin11, HIGH);
  }
  //portTest();
  //initializeCells();
  //initializefromPort22();
}

void loop() {

  portTest();
  pauseButton();
  resetButton();
  offonPauseG();
  //onkeypressed();

  proxibrightness();
  if (millis() - lastRecordedTime > interval) {
    if (!pause) {
      iteration();
      statePlot();
      lastRecordedTime = millis();
    }
  }
}

void statePlot() {
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      if (array1[i][j] == 1) {
        // fill(alive); // If alive
        m.setDot(j, i, true);
      }
      else {
        // fill(dead); // If dead
        m.setDot(j, i, false);
      }
    }
  }
}

void iteration() {
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      cellbuffer[i][j] = array1[i][j];
    }
  }
  // Visit each cell:
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      int neighbours = 0; //  count the neighbours
      for (int ii = i - 1; ii <= i + 1; ii++) {
        for (int jj = j - 1; jj <= j + 1; jj++) {
          if (((ii >= 0) && (ii < cols)) && ((jj >= 0) && (jj < rows))) { //check if out of bounds
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

void proxibrightness() {

  a = sr04.Distance();
  int average = 0;

  for (int i = 0; i < 20; i++) {
    average = average + a;
  }
  average = average / 20;
  if (average < 0) {
    average = 0;
  } else if (average > 400) {
    average = 400;
  }

  //mapping reverse range for bright to be at max when close and minimum at far.
  //bright = map(a,0,400,12 ,0);

  if (average < 100) {
    bright = bright + 1;
    if (bright > 12) {
      bright = 12;
    }
  }
  if (average > 100) {
    bright = bright - 1;
    if (bright < 0) {
      bright = 0;
    }
  }
  m.setIntensity(bright);
}

void portTest() {

  pinMode(ledPin9, OUTPUT);

  while (Serial.available()) { // If data is available to read,
    serVal = Serial.read();
  }
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      int state=0;
      if (serVal == 1) {
        state = 1;
      } else if (serVal == 0) {
        state = 0;
      }
      array1[i][j] = int(state);
    }
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
        //initializeCells();
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


//void initializeCells() {
//
//  for (int i = 0; i < cols; i++) {
//    for (int j = 0; j < rows; j++) {
//      float state = random (100);
//      if (state > Probability) {
//        state = 0;
//      }
//      else {
//        state = 1;
//      }
//      array1[i][j] = int(state); // Save state of each cell
//    }
//  }
//}

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
/* TEMP
*/
