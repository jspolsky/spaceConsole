
#include <Arduino.h>
#include <OctoWS2811.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "Teensy4Controller.h"
#include "Util.h"
#include "led.h"

namespace Led
{
  static bool power = true;
  static int buttonStatus = 0;

  uint8_t brightnessOptions[15] = {2, 4, 8, 12, 16, 24, 32, 48, 56, 64, 72, 96, 128, 168, 255};
  static uint8_t brightnessIndex = 14;

  enum mode_t
  {
    mode_singleColor,
    mode_genetic,
    mode_testPattern
  };

  static mode_t mode = mode_genetic; // TODO change back to mode_testPattern;

  static CRGB singleColorMode = CRGB::Black;
  //
  // Which of the physical LEDs that are present are going to
  // get patterns drawn on them?
  //

  const uint32_t NUM_LEDS = 1200; // LAB

  //
  // Physical arrangement of LEDs
  //

  // Pins 25-40 go out to the antenna. Each group of four sequential pins
  // represent one line from the bottom to the top, so, for example, pins 25 - 28
  // are a single line of 1200 LEDs, addressed as pixels[0..1199].
  //
  // Pin 14 is for five RGB pushbuttons on the control panel.

  const int numPins = 17;
  byte pinList[numPins] = {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 14};
  const int ledsPerStrip = 300;
  CRGB pixels[numPins * ledsPerStrip];

  // These buffers need to be large enough for all the pixels.
  // The total number of pixels is "ledsPerStrip * numPins".
  // Each pixel needs 3 bytes, so multiply by 3.  An "int" is
  // 4 bytes, so divide by 4.  The array is created using "int"
  // so the compiler will align it to 32 bit memory.
  DMAMEM int displayMemory[ledsPerStrip * numPins * 3 / 4];
  int drawingMemory[ledsPerStrip * numPins * 3 / 4];
  OctoWS2811 octo(ledsPerStrip, displayMemory, drawingMemory, WS2811_RGB | WS2811_800kHz, numPins, pinList);
  CTeensy4Controller<RGB, WS2811_800kHz> *pcontroller;

  void setup()
  {
    octo.begin();
    pcontroller = new CTeensy4Controller<RGB, WS2811_800kHz>(&octo);

    FastLED.setBrightness(brightnessOptions[brightnessIndex]);

    //FastLED.setCorrection(TypicalLEDStrip);
    //FastLED.setTemperature(DirectSunlight);
    FastLED.addLeds(pcontroller, pixels, numPins * ledsPerStrip);
  }

  void loop()
  {
    if (power)
    {
      if (buttonStatus == 0) {
        // prompt for vote with cylon
        int litbutton = (millis() / 250) % 5;
        for (int i = 0; i <= 4; i++)
          pixels[4800 + i] = litbutton == i ? CRGB(CHSV((i - 1) * 50, 255, 255)) : CRGB::Black;
      }
      else {
        for (int i = 1; i <= 5; i++)
          pixels[4799 + i] = buttonStatus >= i ? CRGB(CHSV((i - 1) * 50, 255, 255)) : CRGB::Black;
      }

      if (mode == mode_genetic)
      {
        // fnMondrian();
        // fnOneSecondSweep();
        fnNoise();
        // fnThrobBrightness();     // TODO this should be a part of the genetic code

        FastLED.show();
      }
      else if (mode == mode_singleColor)
      {
        FastLED.showColor(singleColorMode);
      }
      else if (mode == mode_testPattern)
      {
        fnTestPattern();
        FastLED.show();
      }
    }
    else
    {
      FastLED.showColor(0);
    }
  }

  uint16_t getFPS()
  {
    return FastLED.getFPS();
  }

  void All4Strips(uint32_t i, CRGB color)
  {
    pixels[i] =
        pixels[i + 1200] =
            pixels[i + 2400] =
                pixels[i + 3600] = color;
  }

  // copies the first strip to all four strips
  void Mirror4Strips()
  {
    for (uint16_t i = 1200; i <= 3600; i += 1200)
      for (uint16_t j = 0; j < 1200; j++)
        pixels[i + j] = pixels[j];
  }

  //
  // for now, all functions named "fn" are loop() implementations. Each one implements a different
  // pattern / style / gene.
  //

  DEFINE_GRADIENT_PALETTE(heatmap_gp){
      0, 0, 0, 0,
      128, 255, 0, 0,
      200, 255, 255, 0,
      255, 255, 255, 255};

  DEFINE_GRADIENT_PALETTE(ocean_gp){
      0, 0, 128, 0,
      96, 0, 128, 64,
      160, 0, 64, 128,
      255, 0, 0, 128};

  // Gradient palette "bhw1_sunset2_gp", originally from
  // http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_sunset2.png.index.html
  // converted for FastLED with gammas (2.6, 2.2, 2.5)
  // Size: 20 bytes of program space.

  DEFINE_GRADIENT_PALETTE(sunset_gp){
      0, 255, 175, 8,
      81, 237, 29, 10,
      137, 148, 57, 42,
      165, 68, 54, 54,
      255, 18, 23, 29};

  // Gradient palette "radial_eyeball_brown_gp", originally from
  // http://soliton.vm.bytemark.co.uk/pub/cpt-city/ocal/tn/radial-eyeball-brown.png.index.html
  // converted for FastLED with gammas (2.6, 2.2, 2.5)
  // Size: 44 bytes of program space.

  DEFINE_GRADIENT_PALETTE(eyeball_brown_gp){
      0, 0, 0, 0,
      38, 0, 0, 0,
      51, 255, 97, 0,
      102, 82, 4, 2,
      109, 82, 4, 2,
      114, 255, 97, 0,
      121, 255, 255, 255,
      229, 255, 255, 255,
      249, 255, 191, 184,
      255, 255, 0, 0};

  // Gradient palette "Caribbean_gp", originally from
  // http://soliton.vm.bytemark.co.uk/pub/cpt-city/vh/tn/Caribbean.png.index.html
  // converted for FastLED with gammas (2.6, 2.2, 2.5)
  // Size: 144 bytes of program space.

  DEFINE_GRADIENT_PALETTE(caribbean_gp){
      0, 0, 0, 43,
      36, 0, 10, 106,
      60, 1, 32, 147,
      84, 1, 79, 138,
      109, 3, 104, 156,
      133, 17, 118, 176,
      145, 35, 118, 176,
      170, 82, 133, 156,
      194, 95, 175, 207,
      213, 173, 203, 242,
      223, 75, 73, 24,
      233, 144, 111, 27,
      237, 184, 125, 27,
      245, 242, 166, 24,
      255, 242, 223, 197};

  CRGBPalette16 heatmap = heatmap_gp;
  CRGBPalette16 ocean = ocean_gp;
  CRGBPalette16 sunset = sunset_gp;
  CRGBPalette16 eyeball_brown = eyeball_brown_gp;
  CRGBPalette16 caribbean = caribbean_gp;

  // delightful bright noise function
  void fnNoise()
  {

    static uint16_t x = 0;
    int scale = 6; // lower numbers: bigger blobs of color. Higher numbers: smaller blobs.
    static uint16_t t = 0;

    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
      uint8_t noise = inoise8(i * scale + x, t);
      uint8_t hue = map(noise, 50, 190, 0, 255); // spread results out into 0-255 hue range.
                                                 // try other ranges, like 0-64 for orange/yellow or 96-180 for bluegreen https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors

      // OPTION ONE - just use hue to get a full-rainbow
      pixels[i] = CHSV(hue, 255, 255); // adjust the second param of beatsin. 164 makes a pronounced throb. 192 is gently throb. 64 is rock and roll
      // OPTION TWO - pick color from a palette
      // pixels[i] = ColorFromPalette(caribbean, hue);
    }

    // actually seriously consider adding beatsin8() to the global brightness as a completely independent genome, for
    // all patterns.

    EVERY_N_MILLISECONDS(10)
    {
      // adjusting x slides the whole pattern up and down
      // subtracting from x slides the pattern up the antenna
      // adding to x slides the pattern down the antenna
      x -= 7; // lower numbers: slower. Higher numbers: faster. 10 is kinda average.

      // adjusting t morphs the whole pattern smoothly
      t += 3; // 1 is probably too slow. 10 is about as fast as you can see!
    }

    Mirror4Strips();
  }

  void fnThrobBrightness()
  {
    // adjusts the global brightness using a beat

    // first param is BPM
    // second param is min
    // third param is max

    FastLED.setBrightness(beatsin8(120, 0, brightnessOptions[brightnessIndex]));
  }

  // zap a nice color from bottom to top every second.
  void fnOneSecondSweep()
  {

    static uint32_t split = 0;
    static uint32_t last_split = 0;
    static uint8_t hue = random8();

    last_split = split;
    split = (millis() % 1000) * NUM_LEDS / 1000;

    if (last_split > split)
    {
      // finish the top
      for (uint32_t i = last_split; i < NUM_LEDS; i++)
      {
        All4Strips(i, CHSV(hue, 255, 128));
      }

      hue += random(27, 102);
    }

    for (uint32_t i = 0; i < split; i++)
    {
      All4Strips(i, CHSV(hue, 255, 128));
    }

    EVERY_N_MILLIS(10)
    {
      // optional variant adds a gentle little fade at the top
      fadeToBlackBy(pixels, 16 * ledsPerStrip, 8);
    }

    for (uint16_t i = 4800; i < 4805; i++)
      pixels[i] = CHSV(hue, 255, 128); // 4800 is the first button ... for now
  }

  // show little pulses of nice colors
  void fnMondrian()
  {
    static uint8_t hue = 0;

    // 200, 500, 2500, 5000 are interesting options
    EVERY_N_MILLIS(200)
    {
      hue = random8();
    }

    EVERY_N_MILLIS(200)
    {

      uint16_t runlength = random16(NUM_LEDS / 10, NUM_LEDS / 5);
      uint16_t start = random16(0, NUM_LEDS - 1 - runlength);

      CRGB color = CHSV(hue, 255, 192);
      for (uint32_t i = start; i < start + runlength; i++)
      {
        All4Strips(i, color);
      }
    }

    EVERY_N_MILLIS(10)
    {
      // so
      // the last param of fadeToBlackBy can be 255 for "crisp" mondrian
      // or any power of 2 below that, to get different effects.
      fadeToBlackBy(pixels, 16 * ledsPerStrip, 4);
    }
  }

  void fnTestPattern()
  {
    static uint8_t hue = 0;

    EVERY_N_MILLISECONDS(100)
    {
      hue = (hue + 1) & 0xFF;
    }

    for (int i = 0; i < 1200; i++)
    {
      pixels[i] = CHSV(32, 255, 196);
      pixels[i + 1200] = CHSV(96, 255, 196);
      pixels[i + 2400] = CHSV(160, 255, 196);
      pixels[i + 3600] = CHSV(224, 255, 196);
    }

    for (uint8_t level = 0; level < 4; level++)
    {

      uint8_t block_size = 6 * (level + 1);
      uint16_t bounce_pos = cubicwave8((millis() / 32) & 0xFF) * (300 - block_size) / 256;

      for (uint16_t i = 0; i < block_size; i++)
      {
        for (uint16_t j = 0; j < 4; j++)
        {

          if (bounce_pos + i < 300)
          {
            pixels[j * 1200 + level * 300 + bounce_pos + i] = (i % 6) ? CRGB::White : CRGB::Black;
          }
        }
      }

      for (int i = 0; i < 4; i++)
      {
        pixels[i * 1200 + level * 300] = pixels[i * 1200 + level * 300 + 299] = CHSV(hue, 255, 255);
      }
    }
  }

  bool togglePower(void)
  {
    return (power = !power);
  }

  uint8_t brighter()
  {
    uint8_t result;

    brightnessIndex = min(14, brightnessIndex + 1);
    FastLED.setBrightness(result = brightnessOptions[brightnessIndex]);
    return result;
  }

  uint8_t dimmer()
  {
    uint8_t result;

    if (brightnessIndex > 0)
    {
      brightnessIndex--;
    }

    FastLED.setBrightness(result = brightnessOptions[brightnessIndex]);
    return result;
  }

  void geneticAlgorithm()
  {
    mode = mode_genetic;
  }

  void testPattern()
  {
    mode = mode_testPattern;
  }

  void setSolidColor(CRGB rgb)
  {
    singleColorMode = rgb;
    mode = mode_singleColor;
  }

  void setButtonStatus(int buttonStatusNew)
  {
    buttonStatus = buttonStatusNew;
  }

}; // namespace Led