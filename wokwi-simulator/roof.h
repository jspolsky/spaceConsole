#pragma once

namespace Roof {

  //
  // The LEDs are arranged in 6 "rings" (actually rectangles). From a bird's eye view,
  // the rectangles look like this:
  //
  // +---+---+---+
  // | 0 | 1 | 2 |
  // +---+---+---+
  // | 3 | 4 | 5 |
  // +---+---+---+
  // camp entrance
  //
  // Ring 0 and 2 are above the camp bars. Ring 1 is above the DJ booth. Rings 3, 4, and 5 are above
  // the front of the public area.
  //
  typedef uint8_t ring_t;   // Ring number. There are 6 rings, numbered from 0 to 5.

  // These utility functions can be used by animations to set
  // the color of LEDs. Animations should not access the leds[] array
  // directly, as this array will be a different size in production vs. simulator.

  void SetRingColor(ring_t ring, CRGB color);   // sets an entire ring to the same color.
  void SetGlobalColor(CRGB color);              // set the entire roof to the same color.


  void setup();
  void loop();
};