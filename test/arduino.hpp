// Arduino test support

#ifndef ARDUINO_HPP_INCLUDED
#define ARDUINO_HPP_INCLUDED

#include <unistd.h>

const int INPUT  = 0;
const int OUTPUT = 1;

int bit( int const pos )
{
    return 1 << pos;
}

template< typename T>
void bitClear( T & x, int const pos )
{
   x &= ~bit( pos ); 
}

template< typename T>
void bitSet( T & x, int const pos )
{
   x |= bit( pos ); 
}

template< typename T>
void bitWrite( T & x, int const pos, bool const on ) 
{
    on ? x |=  bit( pos )
       : x &= ~bit( pos );
}

template< typename T>
bool bitRead( T const x, int const pos )
{
    return ( x & bit( pos ) ) != 0;
}

int g_pin_direction = INPUT;
int g_pin_value     = 0;

void init_board()
{
    g_pin_direction = INPUT;
    g_pin_value     = 0;
}

void pinMode( int pin, bool out )
{
    bitWrite( g_pin_direction, pin, out );
}

void digitalWrite( int pin, bool on )
{
    bitWrite( g_pin_value, pin, on );
}

bool digitalRead( int pin )
{
    return bitRead( g_pin_value, pin );
}

void delayMicroseconds( int us ) 
{
    usleep( 1000 );
}
    
#endif // ARDUINO_HPP_INCLUDED

// end of file
