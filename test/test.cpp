// Moon Phase Calendar for Arduino, using Pro Trinket 3V/12MHz

// Test support
#include "arduino.hpp"

// Arduino application
#include "../arduino/MoonPhaseCalendar/MoonPhaseCalendar.ino"

// Test program

#include "lest.hpp"
#include <iomanip>
#include <ostream>

using pause_ms = int;

template< typename T >
T setbit( T const x, int const pos, bool const on )
{
    return on ? x |  bit( pos )
              : x & ~bit( pos );
}

bool operator==( Date a, Date b )
{
    return a.year == b.year && a.month == b.month && a.day == b.day;
}

std::ostream & operator<< ( std::ostream & os, Date const & d )
{
    return os << "(" << d.year << "," << std::setw(2) << d.month << "," << std::setw(2) << d.day << ")" ;
}

#define CASE( name ) lest_CASE( specification(), name )

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

// Test specification

namespace {

CASE( "Convenience: bit sets proper bit" )
{
    EXPECT( bit( 0 ) == 0x01 );
    EXPECT( bit( 1 ) == 0x02 );
    EXPECT( bit( 2 ) == 0x04 );
    EXPECT( bit( 3 ) == 0x08 );
    EXPECT( bit( 4 ) == 0x10 );
    EXPECT( bit( 5 ) == 0x20 );
    EXPECT( bit( 6 ) == 0x40 );
    EXPECT( bit( 7 ) == 0x80 );
}

CASE( "Convenience: bit tests true for set bits" )
{
    for ( int i = 0; i < 8; ++i )
        EXPECT( bitRead( bit(i), i ) );
}

CASE( "Convenience: bit tests false for unset bits" )
{
    for ( int i = 0; i < 8; ++i )
        for ( int k = i+1; k < 8; ++k )
            EXPECT_NOT( bitRead( bit(i), k ) );
}

CASE( "Convenience: setbit manipulates proper bit" )
{
    for ( int i = 0; i < 16 ; ++i )
    {
        EXPECT( setbit( 0x0000, i, true  ) == ( bit(i)          ) );
        EXPECT( setbit( 0xFFFF, i, false ) == ( bit(i) ^ 0xFFFF ) );
    }
}

CASE( "Electronics: Rotary encoder: ... [.rotary]" )
{
    mock_setup();
    EXPECT( !"Implement" );
}

CASE( "Electronics: Button: ... [.button]" )
{
    mock_setup();
    EXPECT( !"Implement" );
}

CASE( "Electronics: Date display reports 31 Dec as 3112 [date]" )
{
    mock_setup();

    display_date( {2015, 12, 31}, pause_ms(0) );

    EXPECT( display.m_value == 3112 );
}

CASE( "Electronics: Date display reports 1 Jan as 0101 [date]" )
{
    mock_setup();

    display_date( {2016,  1,  1}, pause_ms(0) );

    // no leading '0' here as that signifies octal
    EXPECT( display.m_value == /*0*/101 );
}

CASE( "Electronics: Moon phase display sets pins properly [moon]" )
{
    static int pattern[] =
    {
        0b0000,
        0b0001,
        0b0011,
        0b0111,
        0b1111,
        0b1110,
        0b1100,
        0b1000,
    };
    static_assert( dimension_of(pattern) == phase_count, "expecting #pattern == pin_moon_count" );

    mock_setup();

    for ( int i = 0; i < 8; ++i )
    {
        display_moon_phase( i );

        EXPECT( pattern[i] == g_pin_value >> pin_moon_first );
    }
}

CASE( "Algorithm: A leap year if divisible by 400 [leap]" )
{
    EXPECT( is_leap_year( 1600 ) );
    EXPECT( is_leap_year( 2000 ) );
    EXPECT( is_leap_year( 2400 ) );
}

CASE( "Algorithm: A leap year if divisible by 4 but not by 100 [leap]" )
{
    EXPECT( is_leap_year( 1996 ) );
    EXPECT( is_leap_year( 2008 ) );
    EXPECT( is_leap_year( 2016 ) );
}

CASE( "Algorithm: Not a leap year if not divisible by 4 [leap]" )
{
    EXPECT_NOT( is_leap_year( 1998 ) );
    EXPECT_NOT( is_leap_year( 1999 ) );
    EXPECT_NOT( is_leap_year( 2010 ) );
}

CASE( "Algorithm: Not a leap year if divisible by 100 but not by 400 [leap]" )
{
    EXPECT_NOT( is_leap_year( 1800 ) );
    EXPECT_NOT( is_leap_year( 1900 ) );
    EXPECT_NOT( is_leap_year( 2100 ) );
}

CASE( "Algorithm: 1 Jan 2000 is adjacent to 2 Jan 2000 [date]" )
{
    EXPECT( next_date( Date{2000, 1, 1} ) ==          ( Date{2000, 1, 2} ) );
    EXPECT(          ( Date{2000, 1, 1} ) == prev_date( Date{2000, 1, 2} ) );
}

CASE( "Algorithm: 28 Feb 2000 is adjacent to 29 Feb 2000 (leap year) [date]" )
{
    EXPECT( next_date( Date{2000, 2, 28} ) ==          ( Date{2000, 2, 29} ) );
    EXPECT(          ( Date{2000, 2, 28} ) == prev_date( Date{2000, 2, 29} ) );
}

CASE( "Algorithm: 28 Feb 2015 is adjacent to 1 Mar 2015 (non leap year) [date]" )
{
    EXPECT( next_date( Date{2015, 2, 28} ) ==          ( Date{2015, 3, 1} ) );
    EXPECT(          ( Date{2015, 2, 28} ) == prev_date( Date{2015, 3, 1} ) );
}

CASE( "Algorithm: 31 Dec 2015 is adjacent to 1 Jan 2016 [date]" )
{
    EXPECT( next_date( Date{2015, 12, 31} ) ==          ( Date{2016, 1, 1} ) );
    EXPECT(          ( Date{2015, 12, 31} ) == prev_date( Date{2016, 1, 1} ) );
}

CASE( "Algorithm: New moon on 13 Oct 2015 [moon]" )
{
    EXPECT( moon_phase( {2015, 10, 13} ) == New );
}

CASE( "Algorithm: First quarter on 20 Oct 2015 [moon]" )
{
    EXPECT( moon_phase( {2015, 10, 20} ) == FirstQuarter );
}

CASE( "Algorithm: Full moon on 27 Oct 2015 [moon]" )
{
    EXPECT( moon_phase( {2015, 10, 27} ) == Full );
}

CASE( "Algorithm: Last quarter on 3 Nov 2015 [moon]" )
{
    EXPECT( moon_phase( {2015, 11, 3} ) == ThirdQuarter );
}

CASE( "Acceptance: New moon on 13 Oct 2015 [.accept]" )
{
    mock_setup();

    Date next = once( {2015, 10, 13}, pause_ms(0) );

    EXPECT(   next == ( Date{2015, 10, 14} ) );
    EXPECT(   1310 == display.m_value );
    EXPECT( 0b0000 == g_pin_value >> pin_moon_first );
}

CASE( "Acceptance: First quarter on 20 Oct 2015 [.accept]" )
{
    mock_setup();

    Date next = once( {2015, 10, 20}, pause_ms(0) );

    EXPECT(   next == ( Date{2015, 10, 21} ) );
    EXPECT(   2010 == display.m_value );
    EXPECT( 0b0011 == g_pin_value >> pin_moon_first );
}

CASE( "Acceptance: Full moon on 27 Oct 2015 [.accept]" )
{
    mock_setup();

    Date next = once( {2015, 10, 27}, pause_ms(0) );

    EXPECT(   next == ( Date{2015, 10, 28} ) );
    EXPECT(   2710 == display.m_value );
    EXPECT( 0b1111 == g_pin_value >> pin_moon_first );
}

CASE( "Acceptance: Last quarter on 3 Nov 2015 [.accept]" )
{
    mock_setup();

    Date next = once( {2015, 11, 3}, pause_ms(0) );

    EXPECT(   next == ( Date{2015, 11, 4} ) );
    EXPECT(    311 == display.m_value );
    EXPECT( 0b1100 == g_pin_value >> pin_moon_first );
}

CASE( "App: date-moon phase [.app]" )
{
    char const * const phase[] =
    {
        "....",
        "...)",
        "..|)",
        ".||)",
        "(||)",
        "(||.",
        "(|..",
        "(...",
    };

    Date day{2015, 11, 1};

    for (;;)
    {
        day = next_date( day );
        std::cout << day << " " << phase[ moon_phase( day ) ] << "\r";
        delay( 300 );
    }
}

}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv /*, std::cout */ );
}

#if 0
cl             -W3   -Dlest_FEATURE_AUTO_REGISTER=1 -I. -EHsc       test.cpp && test.exe
g++ -std=c++11 -Wall -Dlest_FEATURE_AUTO_REGISTER=1 -I. -o test.exe test.cpp && test.exe
#endif // 0
