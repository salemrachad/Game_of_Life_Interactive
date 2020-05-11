![main-page-sketch](https://user-images.githubusercontent.com/61739179/79780058-a4d5b700-833b-11ea-9721-27b420642cf1.gif)

                                          Interactive Painting

**I am building an interactive painting for my digital fabrication masters @ IED Madrid**  
Please the check complete documentation on my wiki page : *https://github.com/salemrachad/Interactive_Painting/wiki*

* The IoT interactive painting can hang by your front door or a hallway...
When the painting senses motion, i.e someone exiting/entering the apartment it
will display the Weather Information or snap a picture and use it as the initial
state for Conway's Game of life.  Where the idea is that life doesn't only happen to you.
At some level you create it. So your presence is essence in that exchange.
  * *https://github.com/salemrachad/Interactive_Painting/wiki/0.0-Concept*

* I am going to be using an Arduino nano 33 IoT to control WS2812b LED displays. It has a 
good price bends for creative shaping can connect multiple matrixes together and easily controlled
by the fastLED or Neopixel Library. Every LED on that requires 5 mA so make sure to do the correct calculations
and chose the correct power supply.
  * *https://github.com/salemrachad/Interactive_Painting/wiki/0.1-Setup*

* I am using openWeather API call to get the weather information needed.
  * *https://github.com/salemrachad/Interactive_Painting/wiki/1.0-Weather-Sprites*
  * *https://github.com/salemrachad/Interactive_Painting/wiki/1.2-WIFI---API---JSON*

* The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician 
John Horton Conway in 1970. It is a zero-player game, meaning that its evolution is determined by its
initial state, requiring no further input.  
  * *https://github.com/salemrachad/Interactive_Painting/wiki/2.0-GOL-Rules*

* I am using Processing.exe to analyze color information and translate the image taken by the webcam to a 32 x 32 2D Matrix then pass it through serial to the Arduino. Sending a 1024 byte size array is a bit tricky and I am still working on it.
  * *https://github.com/salemrachad/Interactive_Painting/wiki/3.0-Interactive-display*  
