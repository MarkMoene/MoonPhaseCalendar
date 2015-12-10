// Arduino test support

#ifndef ARDUINO_HPP_INCLUDED
#define ARDUINO_HPP_INCLUDED

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

void mock_setup()
{
    g_pin_direction = INPUT;
    g_pin_value     = 0;

    extern void setup();
    setup();
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

#ifdef _MSC_VER

# include <chrono>
# include <thread>

void delayMicroseconds( int us )
{
    std::this_thread::sleep_for( std::chrono::microseconds( us ) );
}
#else

// gcc requires posix threading for chrono, we'll use usleep():
# include <unistd.h>

void delayMicroseconds( int us )
{
    usleep( us );
}
#endif // _MSC_VER

void delay( int ms )
{
    delayMicroseconds( 1000 * ms );
}

#endif // ARDUINO_HPP_INCLUDED

// end of file
