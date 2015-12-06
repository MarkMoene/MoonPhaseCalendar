// Moon Phase Calendar for Arduino, using Pro Trinket 3V/12MHz
// Copyright 2015 by Mark Moene

#define dimension_of( a ) ( sizeof(a) / sizeof(0[a]) )

// Configuration: start date

// Start date, must be date valid for moon phase algorithm:
// from 1-1-1900 to 31-12-2199 inclusive.
#define START_DATE { 2015, 11, 1 }

// Configuration: Pins

// Moon phase output (contiguous pins):
const int pin_moon_first = 9;
const int pin_moon_count = 4;
const int phase_count    = 8;

// Date 4-digit display:
const int pin_date_sda = 1;
const int pin_date_clk = 2;

// Rotary encoder:
// ....

// Push buttons:
// ....

// Date display:

void setup_date_display()
{
    pinMode( pin_date_sda, OUTPUT );
    pinMode( pin_date_clk, OUTPUT );
}

struct Date
{
    int year;
    int month;
    int day;
};

void display_date( Date const date )
{
    (void) date;
}

// Moon phase display:

int pin_moon( int const i )
{
    return pin_moon_first + i;
}

void setup_moon_phase_display()
{
    for ( int i = 0; i < pin_moon_count; ++i )
    {
        pinMode( pin_moon(i), OUTPUT );
    }
}

void display_moon_phase( int const phase )
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

    const int code = pattern[ phase % phase_count ];

    for ( int i = 0; i < pin_moon_count; ++i )
    {
        digitalWrite( pin_moon(i), bitRead( code, i ) );
    }
}

// ----------------------------------------------------------------------------
// Date algorithm.
// normalise() inspired on https://www.sourceware.org/git/gitweb.cgi?p=newlib-cygwin.git;a=blob_plain;f=newlib/libc/time/mktime.c

#include <stdlib.h>     // for div_t; cstdlib not standard available on Arduino

int is_leap_year( int const year )
{
    return ( 0 == year % 400 ) || (0 == year % 4 && 0 != year % 100 );
}

int days_in_month( int const year, int const month )
{
    enum { Jan, Feb, };

    const int days[12] =
    {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    };

    return days[ month ] + ( month == Feb && is_leap_year( year ) );
}

Date normalise( Date const date )
{
    Date d{ date.year, date.month - 1, date.day };

    if ( d.month < 0 || d.month > 11 )
    {
        div_t res = div( d.month, 12 );

        d.year += res.quot;

        if ( ( d.month = res.rem ) < 0 )
        {
            d.month += 12;
            --d.year;
        }
    }

    if ( d.day <= 0 )
    {
        while ( d.day <= 0 )
        {
            if ( --d.month == -1 )
            {
                d.year--;
                d.month = 11;
            }
            d.day += days_in_month( d.year, d.month );
        }
    }
    else
    {
        while ( d.day > days_in_month( d.year, d.month ) )
        {
            d.day -= days_in_month( d.year, d.month );

            if ( ++d.month == 12 )
            {
                d.year++;
                d.month = 0;
            }
        }
    }

    return { d.year, d.month + 1, d.day };
}

Date make_date( Date const date, int const add_days )
{
    return normalise( { date.year, date.month, date.day + add_days } );
}

Date prev_date( Date date ) { return make_date( date, -1 ); }
Date next_date( Date date ) { return make_date( date, +1 ); }

// ----------------------------------------------------------------------------
// Moon phase algorithm using integral numbers.
// Simplified to work from 1900 to 2199 inclusive.
// Based on Moon_phase(), http://www.voidware.com/moon_phase.htm

enum MoonPhase
{
    New , WaxingCrescent, FirstQuarter, WaxingGibbous,
    Full, WaningGibbous , ThirdQuarter, WaningCrescent,
};

int moon_phase( Date date )
{
    // Calculates the moon phase (0-7), accurate to 1 segment.
    // 0 = > new moon.
    // 4 => Full moon.

    // Transfer months worth of days to day:

    if ( date.month == 1 )
    {
        --date.day;
    }
    else if ( date.month == 2 )
    {
        date.day += 30;
    }
    else // m >= 3
    {
        date.day += 28 + ( date.month - 2 ) * 3059 / 100 + is_leap_year( date.year );
    }

    const int g = ( date.year - 1900 ) % 19 + 1;
    const int e = ( 11 * g + 18 ) % 30;

    const int adjust = ( (e == 25 && g > 11) || e == 24 );

    return ( ( ( ( e + date.day + adjust ) * 6 + 11 ) % 177 ) / 22 ) % 8;
}

// ----------------------------------------------------------------------------
// main program

void sleep_ms( int ms )
{
    for ( int i = 0; i < ms; ++i )
        delayMicroseconds( 1000 );
}

void setup()
{
    setup_date_display();
    setup_moon_phase_display();
}

Date once( Date date )
{
    display_date( date );
    display_moon_phase( moon_phase( date ) );

    return next_date( date );
}

void loop()
{
    static Date date = START_DATE;

    date = once( date );

    sleep_ms( 1000 );
}

// end of file
