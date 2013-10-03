/*
    MPR121_defs.h
	April 8, 2010
	by: Jim Lindblom

        updated June 22, 2012
        by: Stefan Dzisiewski-Smith
*/

// MPR121 Register Defines

// touch and OOR statuses
#define _TS1       0x00
#define _TS2       0x01
#define _OORS1     0x02
#define _OORS2     0x03

// filtered data
#define _E0FDL     0x04
#define _E0FDH     0x05
#define _E1FDL     0x06
#define _E1FDH     0x07
#define _E2FDL     0x08
#define _E2FDH     0x09
#define _E3FDL     0x0A
#define _E3FDH     0x0B
#define _E4FDL     0x0C
#define _E4FDH     0x0D
#define _E5FDL     0x0E
#define _E5FDH     0x0F
#define _E6FDL     0x10
#define _E6FDH     0x11
#define _E7FDL     0x12
#define _E7FDH     0x13
#define _E8FDL     0x14
#define _E8FDH     0x15
#define _E9FDL     0x16
#define _E9FDH     0x17
#define _E10FDL    0x18
#define _E10FDH    0x19
#define _E11FDL    0x1A
#define _E11FDH    0x1B
#define _E12FDL    0x1C
#define _E12FDH    0x1D

// baseline values
#define _E0BV      0x1E
#define _E1BV      0x1F
#define _E2BV      0x20
#define _E3BV      0x21
#define _E4BV      0x22
#define _E5BV      0x23
#define _E6BV      0x24
#define _E7BV      0x25
#define _E8BV      0x26
#define _E9BV      0x27
#define _E10BV     0x28
#define _E11BV     0x29
#define _E12BV     0x2A

// general electrode touch sense baseline filters
// rising filter
#define _MHDR	  0x2B
#define _NHDR	  0x2C
#define	_NCLR 	  0x2D
#define	_FDLR	  0x2E

// falling filter
#define	_MHDF	  0x2F
#define	_NHDF	  0x30
#define	_NCLF	  0x31
#define	_FDLF	  0x32

// touched filter
#define _NHDT      0x33
#define _NCLT      0x34
#define _FDLT      0x35

// proximity electrode touch sense baseline filters
// rising filter
#define _MHDPROXR  0x36
#define _NHDPROXR  0x37
#define _NCLPROXR  0x38
#define _FDLPROXR  0x39

// falling filter
#define _MHDPROXF  0x3A
#define _NHDPROXF  0x3B
#define _NCLPROXF  0x3C
#define _FDLPROXF  0x3D

// touched filter
#define _NHDPROXT  0x3E
#define _NCLPROXT  0x3F
#define _FDLPROXT  0x40

// electrode touch and release thresholds
#define	_E0TTH	  0x41
#define	_E0RTH	  0x42
#define	_E1TTH	  0x43
#define	_E1RTH	  0x44
#define	_E2TTH	  0x45
#define	_E2RTH	  0x46
#define	_E3TTH	  0x47
#define	_E3RTH	  0x48
#define	_E4TTH	  0x49
#define	_E4RTH	  0x4A
#define	_E5TTH	  0x4B
#define	_E5RTH	  0x4C
#define	_E6TTH	  0x4D
#define	_E6RTH	  0x4E
#define	_E7TTH	  0x4F
#define	_E7RTH	  0x50
#define	_E8TTH	  0x51
#define	_E8RTH	  0x52
#define	_E9TTH	  0x53
#define	_E9RTH	  0x54
#define	_E10TTH	  0x55
#define	_E10RTH	  0x56
#define	_E11TTH	  0x57
#define	_E11RTH	  0x58
#define	_E12TTH	  0x59
#define	_E12RTH	  0x5A

// debounce settings
#define _DTR       0x5B

// configuration registers
#define _AFE1      0x5C
#define _AFE2      0x5D
#define _ECR       0x5E

// electrode currents
#define _CDC0      0x5F
#define _CDC1      0x60
#define _CDC2      0x61
#define _CDC3      0x62
#define _CDC4      0x63
#define _CDC5      0x64
#define _CDC6      0x65
#define _CDC7      0x66
#define _CDC8      0x67
#define _CDC9      0x68
#define _CDC10     0x69
#define _CDC11     0x6A
#define _CDC12     0x6B

// electrode charge times
#define _CDT01     0x6C
#define _CDT23     0x6D
#define _CDT45     0x6E
#define _CDT67     0x6F
#define _CDT89     0x70
#define _CDT1011   0x71
#define _CDT11     0x72

// GPIO
#define vCTL0	  0x73
#define	_CTL1	  0x74
#define _DAT	  0x75
#define	_DIR	  0x76
#define	_EN        0x77
#define	_SET	  0x78
#define	_CLR	  0x79
#define	_TOG	  0x7A

// auto-config 
#define	_ACCR0	  0x7B
#define	_ACCR1	  0x7C
#define _USL       0x7D
#define _LSL       0x7E
#define _TL        0x7F

// soft reset
#define _SRST      0x80

// PWM
#define _PWM0      0x81
#define _PWM1      0x82
#define _PWM2      0x83
#define _PWM3      0x84
