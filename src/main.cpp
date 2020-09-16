#include <Arduino.h>
#include <OctoWS2811.h>


// 
// OctoWS2811 
//

// Any group of digital pins may be used
const int numPins = 16;
byte pinList[numPins] = {25,26,27,28,29,30,31,32,
                         33,34,35,36,37,38,39,40};

const int ledsPerStrip = 300;

// These buffers need to be large enough for all the pixels.
// The total number of pixels is "ledsPerStrip * numPins".
// Each pixel needs 3 bytes, so multiply by 3.  An "int" is
// 4 bytes, so divide by 4.  The array is created using "int"
// so the compiler will align it to 32 bit memory.
DMAMEM int displayMemory[ledsPerStrip * numPins * 3 / 4];
int drawingMemory[ledsPerStrip * numPins * 3 / 4];
const int config = WS2811_RGB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config, numPins, pinList);


void setup() {
  leds.begin();
  leds.show();
}


#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define BROWN  0x654321

void colorWipe(int color, int wait);
void loop() {
  int microsec = 2000000 / leds.numPixels();  // change them all in 2 seconds

  // uncomment for voltage controlled speed
  // millisec = analogRead(A9) / 40;

  colorWipe(RED, microsec);
  colorWipe(GREEN, microsec);
  colorWipe(BLUE, microsec);
  colorWipe(YELLOW, microsec);
  colorWipe(PINK, microsec);
  colorWipe(ORANGE, microsec);
  colorWipe(WHITE, microsec);
}

void colorWipe(int color, int wait)
{

  for (int i=0; i < leds.numPixels(); i+=3) {

    leds.setPixel(i, color);
    leds.setPixel(i+1, color);
    leds.setPixel(i+2, color);
    leds.show();
    delayMicroseconds(wait);
  }
}
