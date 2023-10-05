/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(EXPERIMENTAL_I2CBUS)

#include "../../gcode.h"

#include "../../../feature/twibus.h"

#define ABRANTIX_I2C_MAX_DATA_LENGTH 28
#define ABRANTIX_I2C_WAIT_TIMEOUT 5000

struct AbrantixI2cFrame
{
  char Command;
	unsigned char DataLength;
	char Data[ABRANTIX_I2C_MAX_DATA_LENGTH];
};

/**
 * M270: Send Abrantix I2C frame to a I2C slave device
 *
 *  M270 A<slave device address base 10> C<command base 10> D"<data as quoted ASCII string>"
 *
 */
void GcodeSuite::M270() {
  AbrantixI2cFrame i2cFrame;
  bool addressPresent = false;
  bool commandPresent = false;
  
  // Reset and rewind the buffer
  i2c.reset();

  // Set the target address
  if (parser.seenval('A')) {
    i2c.address(parser.value_byte());
    addressPresent = true;
  }  

  if(parser.seenval('C')) {
    i2cFrame.Command = parser.value_byte();
    commandPresent = true;
  }

  if(parser.seenval('D')) {
    const char* data = parser.value_string();
    strncpy(i2cFrame.Data, data, sizeof(i2cFrame.Data));
    i2cFrame.DataLength = min(sizeof(i2cFrame.Data), strlen(data));
  }

  // Flush the buffer to the bus
  if (addressPresent && commandPresent) {
    i2c.addbytes((char*)(&i2cFrame), i2cFrame.DataLength + 2);
    i2c.send();
  } 
  else {
    SERIAL_ERROR_MSG("Bad i2c request");
  }
}

/**
 * M271: Send Abrantix I2C frame and request X bytes from I2C slave device
 *
 * Usage: M271 A<slave device address base 10> C<command base 10> D<data as (quoted) ASCII string - optional> X<wait for expected answer (optional) [S<silent - optional>]>
 */
void GcodeSuite::M271() {
  AbrantixI2cFrame i2cResponseFrame;
  bool waitForAnswer = false;
  bool silent = false;
  const char* expectedAnswer = ""; 
  bool loop = false;
  millis_t startMillis = millis();

  if(parser.seenval('X'))
  {
    expectedAnswer = parser.value_string();
    waitForAnswer = true;
  }

  if(waitForAnswer) {
    loop = true;
  }

  silent = parser.seen('S');

  do {
    //first, we're going to send data
    M270();

    if (parser.seenval('A')) i2c.address(parser.value_byte());

    const uint8_t bytes = ABRANTIX_I2C_MAX_DATA_LENGTH + 2;

    if (i2c.addr) {
      uint8_t bytesReceived = i2c.capture((char*)(&i2cResponseFrame), bytes);
      
      //response sanity checks
      if(bytesReceived >= 2) {
        //place trailing zero in data field
        i2cResponseFrame.Data[i2cResponseFrame.DataLength] = 0;
        
        if(!silent) {
          SERIAL_ECHO(i2cResponseFrame.Data);
        }
        if(waitForAnswer) {
          if(0 == strncmp(expectedAnswer, i2cResponseFrame.Data, ABRANTIX_I2C_MAX_DATA_LENGTH)) {
            loop = false;
          }
          else {
            if(millis() - startMillis > ABRANTIX_I2C_WAIT_TIMEOUT)
            {
              SERIAL_ERROR_MSG("I2C wait response Timeout");      
              loop = false;
            } else {
              safe_delay(50);
            }
          }
        }       
      }
      else {
        SERIAL_ERROR_MSG("Bad i2c response");
        i2cResponseFrame.DataLength = 0;
        i2cResponseFrame.Data[0] = 0;
        loop = false;
      }
    }
    else {
      SERIAL_ERROR_MSG("Bad i2c request");
      loop = false;
    }
  } while (loop);
}

#endif
