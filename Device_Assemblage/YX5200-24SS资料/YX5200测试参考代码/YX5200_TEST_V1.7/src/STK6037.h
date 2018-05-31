#ifndef __STK6037_H__
#define __STK6037_H__

sfr     CHIPCON       = 0xBF;   /* {RW}[---10000] CHIP CONfiguration register */
sfr     P1_OPT        = 0xD9;   /* {RW}[---00000] PWM Pad Option register */
sfr     PWM_EA0      = 0xD2;   /* {RW}[10000000] PWM0 data register */
sfr     PWM_EA1      = 0xD3;   /* {RW}[10000000] PWM1 data register */
sfr     PWM_EA2      = 0xD4;   /* {RW}[10000000] PWM2 data register */

sfr     PCLKSEL      = 0XB7;

sfr     PWM0H      = 0xF9;   /* {RW}[10000000] PWM0 data register */
sfr     PWM0L      = 0xFA;   /* {RW}[10000000] PWM0 data register */
sfr     PWMDATA1H      = 0xFB;   /* {RW}[10000000] PWM1 data register */
sfr     PWMDATA1L      = 0xFC;   /* {RW}[10000000] PWM1 data register */
sfr     PWMDATA2H      = 0xFD;   /* {RW}[10000000] PWM2 data register */
sfr     PWMDATA2L      = 0xFE;   /* {RW}[10000000] PWM2 data register */
sfr     PWMDATA3H      = 0xF1;   /* {RW}[10000000] PWM3 data register */
sfr     PWMDATA3L      = 0xF2;   /* {RW}[10000000] PWM3 data register */
sfr     PWMDATA4H      = 0xF3;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA4L      = 0xF4;   /* {RW}[10000000] PWM4 data register */

sfr     PWMDATA5H      = 0xF5;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA5L      = 0xF6;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA6H      = 0xE9;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA6L      = 0xEA;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA7H      = 0xEB;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA7L      = 0xEC;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA8H      = 0xED;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA8L      = 0xEE;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA9H      = 0xB9;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA9L      = 0xBA;   /* {RW}[10000000] PWM4 data register */

sfr     PWMDATA10H      = 0xBB;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA10L      = 0xBC;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA11H      = 0xBD;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA11L      = 0xBE;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA12H      = 0xB1;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA12L      = 0xB2;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA13H      = 0xB3;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA13L      = 0xB4;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA14H      = 0xB5;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA14L      = 0xB6;   /* {RW}[10000000] PWM4 data register */

sfr     PWMDATA15H      = 0xA9;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA15L      = 0xAA;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA16H      = 0xAB;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA16L      = 0xAC;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA17H      = 0xAD;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA17L      = 0xAE;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA18H      = 0xA1;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA18L      = 0xA2;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA19H      = 0xA3;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA19L      = 0xA4;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA20H      = 0xA5;   /* {RW}[10000000] PWM4 data register */
sfr     PWMDATA20L      = 0xA6;   /* {RW}[10000000] PWM4 data register */


sfr     ADCSEL       = 0xDA;   /* {RW}[0---0000] A/D converter control register */

sfr     ADCVALH       = 0xDB;   /* {RW}[--000000] A/D converter data register */
sfr     ADCVALL       = 0xDC;   /* {RW}[--000000] A/D converter data register */

sfr     WDTCTRL       = 0xE1;   /* {RW}[00---000] Watchdog Timer control register */

sfr     ISPSLV        = 0xE2;   /* {RW}[01010101] ISP Slave Address register*/
sfr     ISPEN         = 0xE3;   /* {RW}[00000000] ISP enable register       */

sfr DPL1        = 0x84;  //{??}[00000000] Data Pointer 1 Low byte
sfr DPH1        = 0x85;  //{??}[00000000] Data Pointer 1 High byte

sfr DPS         = 0x86;  /*{??}[00000000] DPTR Select (LSB) S E L  */

sfr CKCON       = 0x8E;  /*{??}[00000001] ClocK CONtrol register TTTMMM 210DDD MMM210  */

sfr SPC_FNC     = 0x8F;  /*{??}[00000000] SPeCial function register W R S  */

sfr EXIF        = 0x91;  /*{??}[00001000] EXtended Interrupt Flag IIII EEEE 5432  */

sfr MPAGE       = 0x92;  //{??}[00000000] Memory PAGE register

sfr EICON       = 0xD8;  /*{??}[01000000] Extended Interrupt CONtrol S EPW M PFD O FIT D I I 1     */

sfr EIE         = 0xE8;  /*{??}[11100000] Extended Interrupt Enable EEEEE WXXXX D5432 */

sfr EIP         = 0xF8;  /*{??}[11100000] Extended Interrupt Priority PPPPP WXXXX D5432 */
// 8052 Extensions
sfr T2CON    = 0xC8;
sfr T2MOD    = 0xC9;
sfr RCAP2L   = 0xCA;
sfr RCAP2H   = 0xCB;
sfr TL2      = 0xCC;
sfr TH2      = 0xCD;

// T2CON
sbit CPRL2   = T2CON ^ 0;
sbit CT2     = T2CON ^ 1;
sbit TR2     = T2CON ^ 2;
sbit EXEN2   = T2CON ^ 3;
sbit TCLK    = T2CON ^ 4;
sbit RCLK    = T2CON ^ 5;
sbit EXF2    = T2CON ^ 6;
sbit TF2     = T2CON ^ 7;

/*========================================================================
 * generic 80C51 SFR
--------------------------------------------------------------------------*/
/*  BYTE Register  */
sfr P0   = 0x80;
sfr P1   = 0x90;
sfr P2   = 0xA0;
sfr P3   = 0xB0;
sfr SCON1 = 0xC0;
sfr SBUF1 = 0xC1;
sfr PSW  = 0xD0;
sfr ACC  = 0xE0;
sfr B    = 0xF0;
sfr SP   = 0x81;
sfr DPL  = 0x82;
sfr DPH  = 0x83;
sfr PCON = 0x87;
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
sfr IE   = 0xA8;
sfr IP   = 0xB8;
sfr SCON = 0x98;
sfr SBUF = 0x99;
sfr TH3  = 0x9B;
sfr TL3  = 0x9A;
 /*  IAP Control  */
sfr IAPEN = 0xE4; // /* IAP Enable */
sfr IAP_ADRL = 0xE5; // /* IAP ADDR */
sfr IAP_ADRH = 0xE6; // /* IAP ADDR */
/*  BIT Register  */
/*  PSW   */
sbit CY   = 0xD7;
sbit AC   = 0xD6;
sbit F0   = 0xD5;
sbit RS1  = 0xD4;
sbit RS0  = 0xD3;
sbit OV   = 0xD2;
sbit P    = 0xD0;

/*  TCON  */
sbit TF1  = 0x8F;
sbit TR1  = 0x8E;
sbit TF0  = 0x8D;
sbit TR0  = 0x8C;
sbit IE1  = 0x8B;
sbit IT1  = 0x8A;
sbit IE0  = 0x89;
sbit IT0  = 0x88;

/*  IE   */
sbit EA   = 0xAF;
sbit ES1   = 0xAE;
sbit ES   = 0xAC;
sbit ET1  = 0xAB;
sbit EX1  = 0xAA;
sbit ET0  = 0xA9;
sbit EX0  = 0xA8;
sbit ET2  = 0xAD;
/*  IP   */
sbit PS   = 0xBC;
sbit PT1  = 0xBB;
sbit PX1  = 0xBA;
sbit PT0  = 0xB9;
sbit PX0  = 0xB8;

/*  P3  */
sbit RD   = 0xB7;
sbit WR   = 0xB6;
sbit T1   = 0xB5;
sbit T0   = 0xB4;
sbit INT1 = 0xB3;
sbit INT0 = 0xB2;
sbit TXD  = 0xB1;
sbit RXD  = 0xB0;

/*  SCON  */
sbit SM0  = 0x9F;
sbit SM1  = 0x9E;
sbit SM2  = 0x9D;
sbit REN  = 0x9C;
sbit TB8  = 0x9B;
sbit RB8  = 0x9A;
sbit TI   = 0x99;
sbit RI   = 0x98;

/*  SCON1  */
sbit SM0_1  = 0xC7;
sbit SM1_1  = 0xC6;
sbit SM2_1  = 0xC5;
sbit REN_1  = 0xC4;
sbit TB8_1  = 0xC3;
sbit RB8_1  = 0xC2;
sbit TI_1   = 0xC1;
sbit RI_1   = 0xC0;
// IO Port
sbit P0_0     = P0 ^ 0;
sbit P0_1     = P0 ^ 1;
sbit P0_2     = P0 ^ 2;
sbit P0_3     = P0 ^ 3;
sbit P0_4     = P0 ^ 4;
sbit P0_5     = P0 ^ 5;
sbit P0_6     = P0 ^ 6;
sbit P0_7     = P0 ^ 7;

sbit P1_0     = P1 ^ 0;
sbit P1_1     = P1 ^ 1;
sbit P1_2     = P1 ^ 2;
sbit P1_3     = P1 ^ 3;
sbit P1_4     = P1 ^ 4;
sbit P1_5     = P1 ^ 5;
sbit P1_6     = P1 ^ 6;
sbit P1_7     = P1 ^ 7;

sbit P2_0     = P2 ^ 0;
sbit P2_1     = P2 ^ 1;
sbit P2_2     = P2 ^ 2;
sbit P2_3     = P2 ^ 3;
sbit P2_4     = P2 ^ 4;
sbit P2_5     = P2 ^ 5;
sbit P2_6     = P2 ^ 6;
sbit P2_7     = P2 ^ 7;

sbit P3_0     = P3 ^ 0;
sbit P3_1     = P3 ^ 1;
sbit P3_2     = P3 ^ 2;
sbit P3_3     = P3 ^ 3;
sbit P3_4     = P3 ^ 4;
sbit P3_5     = P3 ^ 5;
sbit P3_6     = P3 ^ 6;
sbit P3_7     = P3 ^ 7;


typedef	unsigned char	BYTE;
typedef	unsigned int	WORD;

typedef  unsigned char   BOOL ;
typedef  unsigned char   INT8U ;
typedef  signed char     INT8S ;
typedef  unsigned int    INT16U ;
typedef  signed int      INT16S ;
typedef  unsigned long   INT32U ;
typedef  signed long     INT32S ;

#endif
