/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * Creality V24S1_301F4 (STM32F401RC) board pin assignments as found on Ender 3 S1.
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Abrantix Robot 2.0"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Abrantix Robot 2"
#endif

#define DISABLE_DEBUG false // DISABLE_(DEBUG|JTAG) is not supported for STM32F4.
#define ALLOW_STM32F4

#include "env_validate.h"

//
// SD Card
//
//#define ONBOARD_SPI_DEVICE                     1

//Serial (USART1): PA9(TX), PA10(RX)


// I2CI Definitions
#define I2C_SDA_PIN            PB7
#define I2C_SCL_PIN            PB6

//
// EEPROM

#if NO_EEPROM_SELECTED
  #define I2C_EEPROM                              // AT24C32
  #define MARLIN_EEPROM_SIZE             0x8000  // 32K (24C256)
  //#define MARLIN_EEPROM_SIZE               0x2000  // 8K (24C64)
#endif



#define KILL_PIN                          PC10  // (KILL) pin
#define KILL_PIN_STATE                    HIGH

#define SUICIDE_PIN                       PA0
#define SUICIDE_PIN_STATE                 HIGH                       
//
// Limit Switches
//
#define X_STOP_PIN                          PA5
#define Y_STOP_PIN                          PA6
#define Z_STOP_PIN                          PA7
#define I_MIN_PIN                           PC0
#define J_MIN_PIN                           PC1
#define K_MIN_PIN                           PC4
#define U_MIN_PIN                           PC5

//We will use those pins to detect if axis is present or not
//PA0  --> P0
//PA1  --> P1
//PA2  --> P2
//PA3  --> P3

//
// Steppers
//
#ifndef X_STEP_PIN
  #define X_STEP_PIN                        PC2
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN                         PB9
#endif
#define X_ENABLE_PIN                        -1   // Disabled (SOFTWARE_DRIVER_ENABLE), Shared

#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                        PB8
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                         PA11
#endif
#define Y_ENABLE_PIN                X_ENABLE_PIN

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB1
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PB5
#endif
#define Z_ENABLE_PIN                X_ENABLE_PIN


#if HAS_TMC_UART
  /**
   * Address for the UART Configuration of the TMC2209. Override in Configuration files.
   * To test TMC2209 Steppers enable TMC_DEBUG in Configuration_adv.h and test the M122 command with voltage on the steppers.
   */
  #define X_SLAVE_ADDRESS                 0b00
  #define Y_SLAVE_ADDRESS                 0b01
  #define Z_SLAVE_ADDRESS                 0b10
  
  #define I_SLAVE_ADDRESS                 0b00
  #define J_SLAVE_ADDRESS                 0b01
  #define K_SLAVE_ADDRESS                 0b10
  #define U_SLAVE_ADDRESS                 0b11

  // This is the stable default value after testing, but, higher UART rates could be configured, remeber to test the Steppers with the M122 command to check if everything works.
  #define TMC_BAUD_RATE 100000

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                      PC13
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN        X_SERIAL_TX_PIN
  #define Y_SERIAL_RX_PIN        X_SERIAL_RX_PIN

  #define Z_SERIAL_TX_PIN        X_SERIAL_TX_PIN
  #define Z_SERIAL_RX_PIN        X_SERIAL_RX_PIN

  #define I_SERIAL_TX_PIN                      PC14
  #define I_SERIAL_RX_PIN        I_SERIAL_TX_PIN

  #define J_SERIAL_TX_PIN        I_SERIAL_TX_PIN
  #define J_SERIAL_RX_PIN        I_SERIAL_RX_PIN

  #define K_SERIAL_TX_PIN        I_SERIAL_TX_PIN
  #define K_SERIAL_RX_PIN        I_SERIAL_RX_PIN

  #define U_SERIAL_TX_PIN        I_SERIAL_TX_PIN
  #define U_SERIAL_RX_PIN        I_SERIAL_RX_PIN

#endif

#define I_STEP_PIN                          PC6
#define I_DIR_PIN                           PB12
#define I_ENABLE_PIN                X_ENABLE_PIN

#define J_STEP_PIN                          PC7
#define J_DIR_PIN                           PB13
#define J_ENABLE_PIN                X_ENABLE_PIN

#define K_STEP_PIN                          PC8
#define K_DIR_PIN                           PB14
#define K_ENABLE_PIN                X_ENABLE_PIN

#define U_STEP_PIN                          PC9
#define U_DIR_PIN                           PB15
#define U_ENABLE_PIN                X_ENABLE_PIN
