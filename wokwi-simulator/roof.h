#pragma once

namespace Roof
{

  //
  // The LEDs are arranged in 6 "rings" (actually rectangles) as follows:
  //
  // +---+---+---+
  // | 0 | 1 | 2 |
  // +---+---+---+
  // | 3 | 4 | 5 |
  // +---+---+---+
  //
  // Ring 0 and 2 are above the bars. Ring 1 is above the DJ booth. Rings 3, 4, and 5 are above
  // the front public area.
  //
  typedef uint8_t ring_t;   // Ring number. There are 6 rings, numbered from 0 to 5.

  // These utility functions can be used by animations to set
  // the color of LEDs.

  void SetRingColor(ring_t ring, CRGB color);   // sets a given ring to the same color.


  void setup();
  void loop();
};