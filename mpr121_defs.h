/*******************************************************************************

 Bare Conductive MPR121 library
 ------------------------------
 
 MPR121_defs.h - MPR121 register definitions
 
 Based on code by Jim Lindblom and plenty of inspiration from the Freescale 
 Semiconductor datasheets and application notes.
 
 Bare Conductive code written by Stefan Dzisiewski-Smith and Peter Krige.
 
 This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 
 Unported License (CC BY-SA 3.0) http://creativecommons.org/licenses/by-sa/3.0/
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*******************************************************************************/

#ifndef MPR121_DEFS_H
#define MPR121_DEFS_H

// MPR121 Register Defines

// touch and OOR statuses
static unsigned char const TS1 = 0x00;
static unsigned char const TS2 = 0x01;
static unsigned char const OORS1 = 0x02;
static unsigned char const OORS2 = 0x03;

// filtered data
static unsigned char const E0FDL = 0x04;
static unsigned char const E0FDH = 0x05;
static unsigned char const E1FDL = 0x06;
static unsigned char const E1FDH = 0x07;
static unsigned char const E2FDL = 0x08;
static unsigned char const E2FDH = 0x09;
static unsigned char const E3FDL = 0x0A;
static unsigned char const E3FDH = 0x0B;
static unsigned char const E4FDL = 0x0C;
static unsigned char const E4FDH = 0x0D;
static unsigned char const E5FDL = 0x0E;
static unsigned char const E5FDH = 0x0F;
static unsigned char const E6FDL = 0x10;
static unsigned char const E6FDH = 0x11;
static unsigned char const E7FDL = 0x12;
static unsigned char const E7FDH = 0x13;
static unsigned char const E8FDL = 0x14;
static unsigned char const E8FDH = 0x15;
static unsigned char const E9FDL = 0x16;
static unsigned char const E9FDH = 0x17;
static unsigned char const E10FDL = 0x18;
static unsigned char const E10FDH = 0x19;
static unsigned char const E11FDL = 0x1A;
static unsigned char const E11FDH = 0x1B;
static unsigned char const E12FDL = 0x1C;
static unsigned char const E12FDH = 0x1D;

// baseline values
static unsigned char const E0BV = 0x1E;
static unsigned char const E1BV = 0x1F;
static unsigned char const E2BV = 0x20;
static unsigned char const E3BV = 0x21;
static unsigned char const E4BV = 0x22;
static unsigned char const E5BV = 0x23;
static unsigned char const E6BV = 0x24;
static unsigned char const E7BV = 0x25;
static unsigned char const E8BV = 0x26;
static unsigned char const E9BV = 0x27;
static unsigned char const E10BV = 0x28;
static unsigned char const E11BV = 0x29;
static unsigned char const E12BV = 0x2A;

// general electrode touch sense baseline filters
// rising filter
static unsigned char const MHDR = 0x2B;
static unsigned char const NHDR = 0x2C;
static unsigned char const NCLR = 0x2D;
static unsigned char const FDLR = 0x2E;

// falling filter
static unsigned char const MHDF = 0x2F;
static unsigned char const NHDF = 0x30;
static unsigned char const NCLF = 0x31;
static unsigned char const FDLF = 0x32;

// touched filter
static unsigned char const NHDT = 0x33;
static unsigned char const NCLT = 0x34;
static unsigned char const FDLT = 0x35;

// proximity electrode touch sense baseline filters
// rising filter
static unsigned char const MHDPROXR = 0x36;
static unsigned char const NHDPROXR = 0x37;
static unsigned char const NCLPROXR = 0x38;
static unsigned char const FDLPROXR = 0x39;

// falling filter
static unsigned char const MHDPROXF = 0x3A;
static unsigned char const NHDPROXF = 0x3B;
static unsigned char const NCLPROXF = 0x3C;
static unsigned char const FDLPROXF = 0x3D;

// touched filter
static unsigned char const NHDPROXT = 0x3E;
static unsigned char const NCLPROXT = 0x3F;
static unsigned char const FDLPROXT = 0x40;

// electrode touch and release thresholds
static unsigned char const E0TTH = 0x41;
static unsigned char const E0RTH = 0x42;
static unsigned char const E1TTH = 0x43;
static unsigned char const E1RTH = 0x44;
static unsigned char const E2TTH = 0x45;
static unsigned char const E2RTH = 0x46;
static unsigned char const E3TTH = 0x47;
static unsigned char const E3RTH = 0x48;
static unsigned char const E4TTH = 0x49;
static unsigned char const E4RTH = 0x4A;
static unsigned char const E5TTH = 0x4B;
static unsigned char const E5RTH = 0x4C;
static unsigned char const E6TTH = 0x4D;
static unsigned char const E6RTH = 0x4E;
static unsigned char const E7TTH = 0x4F;
static unsigned char const E7RTH = 0x50;
static unsigned char const E8TTH = 0x51;
static unsigned char const E8RTH = 0x52;
static unsigned char const E9TTH = 0x53;
static unsigned char const E9RTH = 0x54;
static unsigned char const E10TTH = 0x55;
static unsigned char const E10RTH = 0x56;
static unsigned char const E11TTH = 0x57;
static unsigned char const E11RTH = 0x58;
static unsigned char const E12TTH = 0x59;
static unsigned char const E12RTH = 0x5A;

// debounce settings
static unsigned char const DTR = 0x5B;

// configuration registers
static unsigned char const AFE1 = 0x5C;
static unsigned char const AFE2 = 0x5D;
static unsigned char const ECR = 0x5E;

// electrode currents
static unsigned char const CDC0 = 0x5F;
static unsigned char const CDC1 = 0x60;
static unsigned char const CDC2 = 0x61;
static unsigned char const CDC3 = 0x62;
static unsigned char const CDC4 = 0x63;
static unsigned char const CDC5 = 0x64;
static unsigned char const CDC6 = 0x65;
static unsigned char const CDC7 = 0x66;
static unsigned char const CDC8 = 0x67;
static unsigned char const CDC9 = 0x68;
static unsigned char const CDC10 = 0x69;
static unsigned char const CDC11 = 0x6A;
static unsigned char const CDC12 = 0x6B;

// electrode charge times
static unsigned char const CDT01 = 0x6C;
static unsigned char const CDT23 = 0x6D;
static unsigned char const CDT45 = 0x6E;
static unsigned char const CDT67 = 0x6F;
static unsigned char const CDT89 = 0x70;
static unsigned char const CDT1011 = 0x71;
static unsigned char const CDT11 = 0x72;

// GPIO
static unsigned char const CTL0 = 0x73;
static unsigned char const CTL1 = 0x74;
static unsigned char const DAT = 0x75;
static unsigned char const DIR = 0x76;
static unsigned char const EN = 0x77;
static unsigned char const SET = 0x78;
static unsigned char const CLR = 0x79;
static unsigned char const TOG = 0x7A;

// auto-config 
static unsigned char const ACCR0 = 0x7B;
static unsigned char const ACCR1 = 0x7C;
static unsigned char const USL = 0x7D;
static unsigned char const LSL = 0x7E;
static unsigned char const TL = 0x7F;

// soft reset
static unsigned char const SRST = 0x80;

// PWM
static unsigned char const PWM0 = 0x81;
static unsigned char const PWM1 = 0x82;
static unsigned char const PWM2 = 0x83;
static unsigned char const PWM3 = 0x84;

#endif // MPR121_DEFS_H
