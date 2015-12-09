// mock display

#ifndef TM1673DISPLAY_H_INCLUDED
#define TM1673DISPLAY_H_INCLUDED

#include <cstdint>

struct TM1637Display
{
  TM1637Display( uint8_t pinClk, uint8_t pinDIO ) {}

  void setBrightness( uint8_t brightness ) {}

  void setSegments( const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0 ) {}

  void showNumberDec( int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0 ) {}

  uint8_t encodeDigit( uint8_t digit ) { return 0; }
};

#endif // TM1673DISPLAY_H_INCLUDED
