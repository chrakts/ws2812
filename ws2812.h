//****************************************************************************
//  ws2812 - Serial driver for ws2812 RGB leds using fastpwm
//
//  Include
//
//  2013 by M. Marquardt (adrock0905@alice.de)
//
//  Please see README.TXT for more information
//

#ifndef WS2812_H_
#define WS2812_H_

#include <avr/io.h>
#include "gammacorrection.h"
#include "ws2812Hardware.h"

//----- Defaults for configurable stuff

//  If undefined, try to find out arch

#if ! ( defined WS_ARCH_TINY || defined WS_ARCH_MEGA || defined WS_ARCH_XMEGA )
    #if __AVR_ARCH__ == 25              // ATTINY
        #define WS_ARCH_TINY
    #elif __AVR_ARCH__ == 5             // ATMEGA
        #define WS_ARCH_MEGA
    #elif __AVR_ARCH__ >= 100           // XMEGA
        #define WS_ARCH_XMEGA
    #else
        #error "Unrecognized __AVR_ARCH__ - please define WS_ARCH_..."
    #endif
#endif

//  Constants for timing (common for all arch)

#ifndef WS_F_OUT
    #define     WS_F_OUT        800000                  // Output frequency Original: 800000
#endif
#ifndef WS_CNT_TOTAL
    #define     WS_CNT_TOTAL    44 // 22 ((F_CPU/WS_F_OUT)/2-1)    // # cycles for one bit
#endif
#ifndef WS_CNT_LOW
    #define     WS_CNT_LOW      12 // 6 war 7 (((F_CPU/WS_F_OUT)/2)/3-1)  // # cycles for T0H
   // #define     WS_CNT_LOW      (((F_CPU/WS_F_OUT)*1)/3-1)  // # cycles for T0H original
#endif
#ifndef WS_CNT_HIGH
    #define     WS_CNT_HIGH     32 // 16 (((F_CPU/WS_F_OUT)*1)/3-1) // # cycles for T1H
#endif




//----- Functions

#ifndef __ASSEMBLER__

//****************************************************************************
//  void WS_init(void)
//
//  DESCR:
//      Initialize hardware
//
extern "C" {
void WS_init(void);
}

//****************************************************************************
//  TINY+ATMEGA:
//      void WS_out(uint8_t *data, uint16_t count, const uint8_t *gamma);
//
//  XMEGA:
//      uint8_t WS_out(uint8_t *data, uint16_t count, const uint8_t *gamma);
//
//  DESCR:
//      Output bytes to ws2812 leds using fastpwm with gamma correction using
//      the lookup table specified.
//
//      ATTINY+ATMEGA: When gamma is non-null, due to implementation
//      constraints, there is ONE MORE byte read at end of data from memory,
//      but not output.
//
//      XMEGA: If output is already in progress (DMA), this function will
//      return an error.
//
//  IN:
//      r24: uint8_t *data      Ptr. to bytes to output
//      r22: uint16_t count     Number of bytes
//      r20: uint8_t *gamma     Ptr. to GammaTable in FLASH, NULL = none
//
//  OUT: (XMega only)
//      r24: uint8_t status      0=OK, 1=DMA busy
//
#if defined WS_ARCH_TINY || defined WS_ARCH_MEGA
void WS_out(uint8_t *data, uint16_t count, const uint8_t *gamma);
#else
extern "C" {
uint8_t WS_out(uint8_t *data, uint16_t count, const uint8_t *gamma);
}
#endif


//****************************************************************************
//  XMEGA:
//      uint8_t WS_outCheck(void)
//
//  DESCR
//      Check if output of last data is finished.
//
//  IN:
//
//  OUT:
//      r24: uint8_t status      0=OK, 1=DMA busy
//
#ifdef WS_ARCH_XMEGA
extern "C" {
	uint8_t WS_outCheck(void);
}
#endif


#endif // __ASSEMBLER__


#endif // WS2812_H_
