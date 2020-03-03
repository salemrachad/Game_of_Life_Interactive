import processing.video.*;
import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

Capture cam;


int[][] ProcessingledMatrix;
int BWrez =250;
int ledxnumber=32;
int ledynumber=8;
String str;

void setup() {  
  size(640, 360);

  String portName = Serial.list()[0];
  print(portName);
  myPort = new Serial(this, portName, 9600);

  initializeCam();
}

void draw() {
}

//Get the average RGB from pixels

color extractColorFromImage(PImage img) {

  img.loadPixels();
  int r = 0, g = 0, b = 0;
  for (int i=0; i<img.pixels.length; i++) {
    color c = img.pixels[i];
    r += c>>16&0xFF;
    g += c>>8&0xFF;
    b += c&0xFF;
  }
  r /= img.pixels.length;
  g /= img.pixels.length;
  b /= img.pixels.length;

  //sums the rgb to calculate brightness
  //closer to 0 is black |||||| closer to 255 is white
  //implement if loop to make colors binary- Calibrate

  int sum = r +g + b;
  for (int i=0; i<ledxnumber; i++) {
    for (int j=0; j<ledynumber; j++) {    
      if (sum <BWrez) {
        r=0;
        g=0;
        b=0;
        ProcessingledMatrix[i][j] = 1;
      } else if (sum>BWrez) {
        r=255;
        g=255;
        b=255;
        ProcessingledMatrix[i][j] = 0;
      }
    }
  }
  return color(r, g, b);
}

void initializeCam() {
  String[] cameras = Capture.list();

  if (cameras.length == 0) {
    println("There are no cameras available for capture.");
    exit();
  } else {
    println("Available cameras:");
    for (int i = 0; i < cameras.length; i++) {
      println(cameras[i]);
    }
    // The camera can be initialized directly using an 
    // element from the array returned by list():
    cam = new Capture(this, cameras[11]);
    cam.start();
  }
}
void processImage() {
  PImage img = loadImage("C:/Users/racha/desktop/image/silh_1.jpg");

  int xled = width/ledxnumber;
  int yled = height/ledynumber;

  ProcessingledMatrix = new int[ledxnumber][ledynumber];

  //Draw LED MATRIX GRID

  for (int i=0; i<width; i+=xled) {
    for (int j=0; j<height; j+=yled) {
      PImage newImg = img.get(i, j, xled, yled);
      fill(extractColorFromImage(newImg));
      rect(i, j, xled, yled);
    }
  }
  save("C:/Users/racha/desktop/image/silh_1_frag.jpg");
}
void keyPressed() {

  if (key == 'r') {
    if (cam.available() == true) {
      cam.read();
      cam.filter(GRAY);
      image (cam, 0, 0);
      saveFrame("C:/Users/racha/desktop/image/silh_1.jpg");
      processImage();
      //myPort.write('r');
      //sendtoArduino();
    }
  }
}

//  }
//}

//void sendtoArduino(int[][] _ProcessingledMatrix) {
//  byte out[] = new byte[ledxnumber*ledynumber];
//////void sendtoArduino() {
//  for (int i=0; i<ledxnumber; i++) {
//    for (int j=0; j<ledynumber; j++) {
//      out[i+j] = (byte)_ProcessingledMatrix[i][j];
//      //print(ProcessingledMatrix[i][j]);
//      //myPort.write(ProcessingledMatrix[i][j]);
//      //parse data from int to byte 
//    }
//  }
// //print(out);
//myPort.write(out);
//}  

void sendtoArduino() {
  for (int i=0; i<ledxnumber; i++) {
    for (int j=0; j<ledynumber; j++) {
      if (i > 0) {
        str += ",";//We add a comma before each value, except the first value
      }
      str += ProcessingledMatrix[i][j];//We concatenate each number in the string.
    }
    myPort.write(str);
  }
}

/*
for(int i = 0; i < 16; i++){
 if(i > 0){
 str += ",";//We add a comma before each value, except the first value
 }
 str += frame[i];//We concatenate each number in the string.
 }
 serial.write(str);
 }*/
