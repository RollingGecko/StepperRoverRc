/*
* Config.h 
*
* is part of StepperRoverRc.ino
* Version: 0.1
* Created: 23 / 3 / 2017
* Author : Andreas Chaitidis

Copyright 2017 - 2020 Andreas Chaitidis Andreas.Chaitidis@gmail.com
*/

#pragma once

#ifndef config_h
#define config_h

#define TEENSY

//Uncomment to enable debugging Serial prints
 #define DEBUGING

//Nano
//Pin settings RC
//#define FWRWRCCHANNELPIN	6
//#define STEARRCCHANNELPIN	7
//#define SENSRCCHANNELPIN	8
//
////Pin setting Stepper
//#define STEP1PIN	2
//#define DIR1PIN		3
//#define STEP2PIN	9
//#define DIR2PIN		10


//TEENSY32
//Pin settings RC
#define FWRWRCCHANNELPIN	13
#define STEARRCCHANNELPIN	14
#define SENSRCCHANNELPIN	15

//Pin setting Stepper
#define STEP1PIN	16
#define DIR1PIN		17
#define STEP2PIN	18
#define DIR2PIN		19


//Invert steppers

#define INVSTEPPER1	-1
#define INVSTEPPER2	1

//definition of Max Speed in steps
#define MAXSTEPSPEED 100

//Stearing speed in percent of max speed
//reduces the max FwRw Speed by this percentage to allow acceleration for stearing
#define STEARINGPERCENTAGE 0.2

//Zerodamping
#define ZERODAMPING 0.05
//Ramping settings
#define INTERVALSTEPSIZE	5000
#define RAMP				0.1

#endif