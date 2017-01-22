/**************************************************************************/
/*!
    @file     TCA9548A.cpp
    @author   Ethan Mak
    @license  BSD (see license.txt)

    Driver for the TCA9548A digital color sensors.

    Lol Adafruit sucks.

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "TCA9548A.h"

/*========================================================================*/
/*                          PRIVATE FUNCTIONS                             */
/*========================================================================*/

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
TCA9548A::TCA9548A(uint8_t it) 
{
  address = it;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

void TCA9548A::select(uint8_t pin)
{
	if (pin > 7) return;
	Wire.beginTransmission(address);
	Wire.write(1 << pin);
	Wire.endTransmission();  
}

void TCA9548::disable()
{
	Wire.beginTransmission(address);
	Wire.write(0);
	Wire.endTransmission();
}
