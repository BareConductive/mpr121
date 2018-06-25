/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------
 
 MPR121_defs.h - MPR121 register definitions
 
 Based on code by Jim Lindblom and plenty of inspiration from the Freescale 
 Semiconductor datasheets and application notes.
 
 Bare Conductive code written by Stefan Dzisiewski-Smith and Peter Krige.
 
 This work is licensed under a MIT license https://opensource.org/licenses/MIT
 
 Copyright (c) 2016, Bare Conductive
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*******************************************************************************/

#ifndef MPR121_DEFS_H
#define MPR121_DEFS_H

// MPR121 Register Defines

// touch and OOR statuses
static uint8_t const MPR121_TS1 = 0x00;
static uint8_t const MPR121_TS2 = 0x01;
static uint8_t const MPR121_OORS1 = 0x02;
static uint8_t const MPR121_OORS2 = 0x03;

// filtered data
static uint8_t const MPR121_E0FDL = 0x04;
static uint8_t const MPR121_E0FDH = 0x05;
static uint8_t const MPR121_E1FDL = 0x06;
static uint8_t const MPR121_E1FDH = 0x07;
static uint8_t const MPR121_E2FDL = 0x08;
static uint8_t const MPR121_E2FDH = 0x09;
static uint8_t const MPR121_E3FDL = 0x0A;
static uint8_t const MPR121_E3FDH = 0x0B;
static uint8_t const MPR121_E4FDL = 0x0C;
static uint8_t const MPR121_E4FDH = 0x0D;
static uint8_t const MPR121_E5FDL = 0x0E;
static uint8_t const MPR121_E5FDH = 0x0F;
static uint8_t const MPR121_E6FDL = 0x10;
static uint8_t const MPR121_E6FDH = 0x11;
static uint8_t const MPR121_E7FDL = 0x12;
static uint8_t const MPR121_E7FDH = 0x13;
static uint8_t const MPR121_E8FDL = 0x14;
static uint8_t const MPR121_E8FDH = 0x15;
static uint8_t const MPR121_E9FDL = 0x16;
static uint8_t const MPR121_E9FDH = 0x17;
static uint8_t const MPR121_E10FDL = 0x18;
static uint8_t const MPR121_E10FDH = 0x19;
static uint8_t const MPR121_E11FDL = 0x1A;
static uint8_t const MPR121_E11FDH = 0x1B;
static uint8_t const MPR121_E12FDL = 0x1C;
static uint8_t const MPR121_E12FDH = 0x1D;

// baseline values
static uint8_t const MPR121_E0BV = 0x1E;
static uint8_t const MPR121_E1BV = 0x1F;
static uint8_t const MPR121_E2BV = 0x20;
static uint8_t const MPR121_E3BV = 0x21;
static uint8_t const MPR121_E4BV = 0x22;
static uint8_t const MPR121_E5BV = 0x23;
static uint8_t const MPR121_E6BV = 0x24;
static uint8_t const MPR121_E7BV = 0x25;
static uint8_t const MPR121_E8BV = 0x26;
static uint8_t const MPR121_E9BV = 0x27;
static uint8_t const MPR121_E10BV = 0x28;
static uint8_t const MPR121_E11BV = 0x29;
static uint8_t const MPR121_E12BV = 0x2A;

// general electrode touch sense baseline filters
// rising filter
static uint8_t const MPR121_MHDR = 0x2B;
static uint8_t const MPR121_NHDR = 0x2C;
static uint8_t const MPR121_NCLR = 0x2D;
static uint8_t const MPR121_FDLR = 0x2E;

// falling filter
static uint8_t const MPR121_MHDF = 0x2F;
static uint8_t const MPR121_NHDF = 0x30;
static uint8_t const MPR121_NCLF = 0x31;
static uint8_t const MPR121_FDLF = 0x32;

// touched filter
static uint8_t const MPR121_NHDT = 0x33;
static uint8_t const MPR121_NCLT = 0x34;
static uint8_t const MPR121_FDLT = 0x35;

// proximity electrode touch sense baseline filters
// rising filter
static uint8_t const MPR121_MHDPROXR = 0x36;
static uint8_t const MPR121_NHDPROXR = 0x37;
static uint8_t const MPR121_NCLPROXR = 0x38;
static uint8_t const MPR121_FDLPROXR = 0x39;

// falling filter
static uint8_t const MPR121_MHDPROXF = 0x3A;
static uint8_t const MPR121_NHDPROXF = 0x3B;
static uint8_t const MPR121_NCLPROXF = 0x3C;
static uint8_t const MPR121_FDLPROXF = 0x3D;

// touched filter
static uint8_t const MPR121_NHDPROXT = 0x3E;
static uint8_t const MPR121_NCLPROXT = 0x3F;
static uint8_t const MPR121_FDLPROXT = 0x40;

// electrode touch and release thresholds
static uint8_t const MPR121_E0TTH = 0x41;
static uint8_t const MPR121_E0RTH = 0x42;
static uint8_t const MPR121_E1TTH = 0x43;
static uint8_t const MPR121_E1RTH = 0x44;
static uint8_t const MPR121_E2TTH = 0x45;
static uint8_t const MPR121_E2RTH = 0x46;
static uint8_t const MPR121_E3TTH = 0x47;
static uint8_t const MPR121_E3RTH = 0x48;
static uint8_t const MPR121_E4TTH = 0x49;
static uint8_t const MPR121_E4RTH = 0x4A;
static uint8_t const MPR121_E5TTH = 0x4B;
static uint8_t const MPR121_E5RTH = 0x4C;
static uint8_t const MPR121_E6TTH = 0x4D;
static uint8_t const MPR121_E6RTH = 0x4E;
static uint8_t const MPR121_E7TTH = 0x4F;
static uint8_t const MPR121_E7RTH = 0x50;
static uint8_t const MPR121_E8TTH = 0x51;
static uint8_t const MPR121_E8RTH = 0x52;
static uint8_t const MPR121_E9TTH = 0x53;
static uint8_t const MPR121_E9RTH = 0x54;
static uint8_t const MPR121_E10TTH = 0x55;
static uint8_t const MPR121_E10RTH = 0x56;
static uint8_t const MPR121_E11TTH = 0x57;
static uint8_t const MPR121_E11RTH = 0x58;
static uint8_t const MPR121_E12TTH = 0x59;
static uint8_t const MPR121_E12RTH = 0x5A;

// debounce settings
static uint8_t const MPR121_DTR = 0x5B;

// configuration registers
static uint8_t const MPR121_AFE1 = 0x5C;
static uint8_t const MPR121_AFE2 = 0x5D;
static uint8_t const MPR121_ECR = 0x5E;

// electrode currents
static uint8_t const MPR121_CDC0 = 0x5F;
static uint8_t const MPR121_CDC1 = 0x60;
static uint8_t const MPR121_CDC2 = 0x61;
static uint8_t const MPR121_CDC3 = 0x62;
static uint8_t const MPR121_CDC4 = 0x63;
static uint8_t const MPR121_CDC5 = 0x64;
static uint8_t const MPR121_CDC6 = 0x65;
static uint8_t const MPR121_CDC7 = 0x66;
static uint8_t const MPR121_CDC8 = 0x67;
static uint8_t const MPR121_CDC9 = 0x68;
static uint8_t const MPR121_CDC10 = 0x69;
static uint8_t const MPR121_CDC11 = 0x6A;
static uint8_t const MPR121_CDCPROX = 0x6B;

// electrode charge times
static uint8_t const MPR121_CDT01 = 0x6C;
static uint8_t const MPR121_CDT23 = 0x6D;
static uint8_t const MPR121_CDT45 = 0x6E;
static uint8_t const MPR121_CDT67 = 0x6F;
static uint8_t const MPR121_CDT89 = 0x70;
static uint8_t const MPR121_CDT1011 = 0x71;
static uint8_t const MPR121_CDTPROX = 0x72;

// GPIO
static uint8_t const MPR121_CTL0 = 0x73;
static uint8_t const MPR121_CTL1 = 0x74;
static uint8_t const MPR121_DAT = 0x75;
static uint8_t const MPR121_DIR = 0x76;
static uint8_t const MPR121_EN = 0x77;
static uint8_t const MPR121_SET = 0x78;
static uint8_t const MPR121_CLR = 0x79;
static uint8_t const MPR121_TOG = 0x7A;

// auto-config 
static uint8_t const MPR121_ACCR0 = 0x7B;
static uint8_t const MPR121_ACCR1 = 0x7C;
static uint8_t const MPR121_USL = 0x7D;
static uint8_t const MPR121_LSL = 0x7E;
static uint8_t const MPR121_TL = 0x7F;

// soft reset
static uint8_t const MPR121_SRST = 0x80;

// PWM
static uint8_t const MPR121_PWM0 = 0x81;
static uint8_t const MPR121_PWM1 = 0x82;
static uint8_t const MPR121_PWM2 = 0x83;
static uint8_t const MPR121_PWM3 = 0x84;

#endif // MPR121_DEFS_H
