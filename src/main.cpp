
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
void RouteIRCode(unsigned int code);

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
  static unsigned int lastIrCode = 0;

  Led::loop();
  if (Remote::loop())
  {
    if (Remote::lastResult == 0xFFFFFFFF)
    {
      // repeat command! -- if it's a repeatable command
      // like brightness up/down.
      if (lastIrCode == 0xFF3AC5 ||
          lastIrCode == 0xFFBA45)
      {
        RouteIRCode(lastIrCode);
      }
    }
    else
    {
      lastIrCode = Remote::lastResult;
      RouteIRCode(lastIrCode);
    }
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
    OLED::status(3,"");
  }

  Alnum::loop();
  Genetics::loop();

  int buttonStatusNew = Buttons::loop();
  if (buttonStatusNew)
  {
    Genetics::record_vote(buttonStatusNew);
  }
}

void RouteIRCode(unsigned int code)
{
  // IR codes: https://gist.github.com/Derpidoo/e3042055e0f5c3708f9b98b75fe4d59e
  // or just try hitting a button to see what you get :)
  OLED::status(3, "");
  char rgchBuf[CB_DISPLAY_LINE];

  switch (code)
  {
  case 0xFF02FD: // POWER
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

  case 0xFFF00F: // AUTO - unimplemented
    break;

  case 0xFF3AC5: // Brightness up

    sprintf(rgchBuf,
            "BRIGHTNESS %d",
            Led::brighter());
    // OLED::status(3, rgchBuf);
    Alnum::showOneTime(rgchBuf);
    break;

  case 0xFFBA45: // Brightness down

    sprintf(rgchBuf,
            "BRIGHTNESS %d",
            Led::dimmer());
    //OLED::status(3, rgchBuf);
    Alnum::showOneTime(rgchBuf);
    break;

  case 0xFF30CF: // DIY1 - test pattern
    Led::testPattern();
    break;

  case 0xFFB04F: // DIY2
  case 0xFF827D: // PLAY
    Led::geneticAlgorithm();
    break;

    //
    // Here are all the colors:
    //

  case 0xFF1AE5: // RED
    Led::setSolidColor(CRGB::Red);
    break;

  case 0xFF2AD5: // RED ROW 2
    Led::setSolidColor(CHSV(16, 255, 255));
    break;

  case 0xFF0AF5: // RED ROW 3
    Led::setSolidColor(CHSV(32, 255, 255));
    break;

  case 0xFF38C7: // RED ROW 4
    Led::setSolidColor(CHSV(48, 255, 255));
    break;

  case 0xFF18E7: // RED ROW 5
    Led::setSolidColor(CHSV(64, 255, 255));
    break;

  case 0xFF9A65: // GREEN
    Led::setSolidColor(CRGB::Green);
    break;

  case 0xFFAA55: // GREEN ROW 2
    Led::setSolidColor(CHSV(108, 255, 255));
    break;

  case 0xFF8A75: // GREEN ROW 3
    Led::setSolidColor(CHSV(121, 255, 255));
    break;

  case 0xFFB847: // GREEN ROW 4
    Led::setSolidColor(CHSV(134, 255, 255));
    break;

  case 0xFF9867: // GREEN ROW 5
    Led::setSolidColor(CHSV(147, 255, 255));
    break;

  case 0xFFA25D: // BLUE
    Led::setSolidColor(CRGB::Blue);
    break;

  case 0xFF926D: // BLUE ROW 2
    Led::setSolidColor(CHSV(179, 255, 255));
    break;

  case 0xFFB24D: // BLUE ROW 3
    Led::setSolidColor(CHSV(198, 255, 255));
    break;

  case 0xFF7887: // BLUE ROW 4
    Led::setSolidColor(CHSV(217, 255, 255));
    break;

  case 0xFF58A7: // BLUE ROW 5
    Led::setSolidColor(CHSV(236, 255, 255));
    break;

  case 0xFF22DD: // WHITE
    Led::setSolidColor(CRGB::White);
    break;

  case 0xFF12ED: // WHITE ROW 2
    Led::setSolidColor(CRGB(192, 192, 192));
    break;

  case 0xFF32CD: // WHITE ROW 3
    Led::setSolidColor(CRGB(128, 128, 128));
    break;

  case 0xFFF807: // WHITE ROW 4
    Led::setSolidColor(CRGB(64, 64, 64));
    break;

  case 0xFFD827: // WHITE ROW 5
    Led::setSolidColor(CRGB(32, 32, 32));
    break;

  default:
    sprintf(rgchBuf, "IR %X ??", code);
    OLED::status(3, rgchBuf);
  }
}