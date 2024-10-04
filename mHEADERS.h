/* 
 * File:   mHEADERS.h
 * Author: User
 *
 * Created on September 6, 2024, 7:50 PM
 */

#ifndef MHEADERS_H
#define	MHEADERS_H

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

#include "DIO.h"
#include "LEDs.h"
#include "buttons.h"
#include "outdev.h"
//#include "mLCD.h"
#include "mLCD4.h"
#include "keypad.h"

#include "intr.h"
#include "mADC.h"
#include "mtimer.h"

#include "mUART.h"
#include "mSPI.h"
#include "mI2C.h"
#include "mEEPROM.h"
#include "SevenSegment.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/FreeRTOSConfig.h"
#include "FreeRTOS/include/task.h"

#endif	/* MHEADERS_H */

