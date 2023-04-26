
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "Util.h"
#include "led.h"
#include "oled.h"
#include "alnum.h"
#include "remote.h"
#include "buttons.h"
#include "genetics.h"

char *itoa(int value, char *str, int base);
void RouteIRCode(unsigned int code, bool fRepeat);

void setup()
{
  OLED::setup();
  OLED::status(3, "Booting");
  Alnum::setup();
  Alnum::setMainMessage("BOOTING");
  Util::setup();
  Genetics::setup();
  Led::setup();
  Remote::setup();
  Buttons::setup();
}

void loop()
{
  Led::loop();
  if (Remote::loop())
  {
        RouteIRCode(Remote::lastResult, Remote::fRepeat);
  }

  // uncomment this to check frame rate
  // EVERY_N_MILLISECONDS(1000)
  // {
  //
  //   char s[16];
  //
  //   sprintf(s, "%d fps", Led::getFPS());
  //   OLED::status(3, s);
  // }

  EVERY_N_MILLISECONDS(15000)
  {
    OLED::status(3, "");
  }

  Alnum::loop();
  Genetics::loop();

  int buttonStatusNew = Buttons::loop();
  if (buttonStatusNew)
  {
    Genetics::record_vote(buttonStatusNew);
  }
}

void RouteIRCode(unsigned int code, bool fRepeat)
{
  // IR codes: https://gist.github.com/Derpidoo/e3042055e0f5c3708f9b98b75fe4d59e
  // or just try hitting a button to see what you get :)
  OLED::status(3, "");
  char rgchBuf[CB_DISPLAY_LINE];

  switch (code)
  {
  case 0x40: // POWER
    
    if (fRepeat) break;
    if (Led::togglePower())
    {
      // OLED::status(3, "Power On");
      Alnum::showOneTime("POWER ON");
    }
    else
    {
      // OLED::status(3, "Power Off");
      Alnum::showOneTime("POWER OFF");
    }
    break;

  case 0xF: // AUTO - unimplemented
    break;

  case 0x5C: // Brightness up

    sprintf(rgchBuf,
            "BRIGHTNESS %d",
            Led::brighter());
    // OLED::status(3, rgchBuf);
    Alnum::showOneTime(rgchBuf);
    break;

  case 0x5D: // Brightness down

    sprintf(rgchBuf,
            "BRIGHTNESS %d",
            Led::dimmer());
    //OLED::status(3, rgchBuf);
    Alnum::showOneTime(rgchBuf);
    break;

  case 0xC: // DIY1 - test pattern
    Led::testPattern();
    break;

  case 0xD: // DIY2
  case 0x41: // PLAY
    Led::geneticAlgorithm();
    break;

  case 0xE: // DIY3 - pride
    Led::pride();
    break;

    //
    // Here are all the colors:
    //

  case 0x58: // RED
    Led::setSolidColor(CRGB::Red);
    break;

  case 0x54: // RED ROW 2
    Led::setSolidColor(CHSV(16, 255, 255));
    break;

  case 0x50: // RED ROW 3
    Led::setSolidColor(CHSV(32, 255, 255));
    break;

  case 0x1C: // RED ROW 4
    Led::setSolidColor(CHSV(48, 255, 255));
    break;

  case 0x18: // RED ROW 5
    Led::setSolidColor(CHSV(64, 255, 255));
    break;

  case 0x59: // GREEN
    Led::setSolidColor(CRGB::Green);
    break;

  case 0x55: // GREEN ROW 2
    Led::setSolidColor(CHSV(108, 255, 255));
    break;

  case 0x51: // GREEN ROW 3
    Led::setSolidColor(CHSV(121, 255, 255));
    break;

  case 0x1D: // GREEN ROW 4
    Led::setSolidColor(CHSV(134, 255, 255));
    break;

  case 0x19: // GREEN ROW 5
    Led::setSolidColor(CHSV(147, 255, 255));
    break;

  case 0x45: // BLUE
    Led::setSolidColor(CRGB::Blue);
    break;

  case 0x49: // BLUE ROW 2
    Led::setSolidColor(CHSV(179, 255, 255));
    break;

  case 0x4D: // BLUE ROW 3
    Led::setSolidColor(CHSV(198, 255, 255));
    break;

  case 0x1E: // BLUE ROW 4
    Led::setSolidColor(CHSV(217, 255, 255));
    break;

  case 0x1A: // BLUE ROW 5
    Led::setSolidColor(CHSV(236, 255, 255));
    break;

  case 0x44: // WHITE
    Led::setSolidColor(CRGB::White);
    break;

  case 0x48: // WHITE ROW 2
    Led::setSolidColor(CRGB(192, 192, 192));
    break;

  case 0x4C: // WHITE ROW 3
    Led::setSolidColor(CRGB(128, 128, 128));
    break;

  case 0x1F: // WHITE ROW 4
    Led::setSolidColor(CRGB(64, 64, 64));
    break;

  case 0x1B: // WHITE ROW 5
    Led::setSolidColor(CRGB(32, 32, 32));
    break;

  default:

    sprintf(rgchBuf,
            "IR %X ??",
            code);
    Alnum::showOneTime(rgchBuf);
  }
}