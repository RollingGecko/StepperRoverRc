/*
* StepperRoverRc.ino
* Version: 0.1
* Created: 23 / 3 / 2017 
* Author : Andreas Chaitidis

Copyright 2017 - 2020 Andreas Chaitidis Andreas.Chaitidis@gmail.com
*/

//Required libraries
#include <AccelStepper.h>	//http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
#include <stdlib.h>			
#include "Config.h"

AccelStepper Stepper1(1,STEP1PIN,DIR1PIN);
AccelStepper Stepper2(1, STEP2PIN, DIR2PIN);

#define NUM_RC_CHANNELS 3 //You need to specify how many pins you want to use
#include <PinChangeInt.h>  //https://github.com/GreyGnome/PinChangeInt
const uint8_t RC_Channel_Pin[NUM_RC_CHANNELS] = {
	FWRWRCCHANNELPIN,STEARRCCHANNELPIN, SENSRCCHANNELPIN};//Here I specify I want to listen to pins A8 to A13 of my mega

uint16_t RC_Channel_Value[NUM_RC_CHANNELS]; //This variable will contain the values read from the RC signal
#include "RCLib.h" //https://github.com/jantje/ArduinoLibraries/tree/master/RCLib ; This include needs all declarations above. Do not try to move it up or it won't compile

float speedStepper1 = 0;
float speedStepper2 = 0;
float fwRwChannel = 0.0;
float stearChannel = 0.0;
float sensChannel = 0.0;
float fwRwChannelramping = 0.0;
int intervalCounter = 0;
float fwRwSensAdj = 0.0;
void setup()
{
	SetRCInterrupts(); //This method will do all the config foe you.
	int flag;
#ifdef DEBUGING
	Serial.begin(57600);
#endif
	SetRCInterrupts(); //This method will do all the config foe you.
					   //Note some problems will be reported on the serial monitor

	
	Stepper1.setMaxSpeed(MAXSTEPSPEED);
//	Stepper1.setAcceleration(1000);
//	Stepper1.setSpeed(100);
	Stepper2.setMaxSpeed(MAXSTEPSPEED);
//	Stepper2.setAcceleration(2000);
}

void loop()
{
//Rc channel reading	
	int flag;
	if (flag = getChannelsReceiveInfo()) //Here you read the RC flag contains all the channels that have a response
										 // see duane's excellent articles on how this works
	{
		fwRwChannel = map(RC_Channel_Value[0], 1100, 1900, -100, 100) / 100.00;
		stearChannel = map(RC_Channel_Value[1], 1100, 1900, -100, 100) / 100.00;
		sensChannel = map(RC_Channel_Value[2], 1100, 1900, 0, 100) / 100.00;
		
//Debugging Stuff	
#ifdef DEBUGING
		Serial.print(RC_Channel_Value[0]);
		Serial.print(",");
		Serial.print(fwRwChannel);
		Serial.print(" ");
		Serial.print(RC_Channel_Value[1]);
		Serial.print(",");
		Serial.print(stearChannel);
		Serial.print(" ");
		Serial.print(RC_Channel_Value[2]);
		Serial.print(",");
		Serial.print(sensChannel);
#endif
		fwRwSensAdj = fwRwChannel*sensChannel;
//Zero-Damping
		if (fwRwSensAdj < ZERODAMPING && fwRwSensAdj > -(ZERODAMPING))
		{
			fwRwSensAdj = 0;
		}

		if (stearChannel < ZERODAMPING && stearChannel > -(ZERODAMPING))
		{
			stearChannel = 0;
		}
	}

//Ramping
	if (intervalCounter == INTERVALSTEPSIZE)
	{
		if (abs(fwRwSensAdj - fwRwChannelramping) > RAMP)
		{
			if ((fwRwSensAdj - fwRwChannelramping) > 0)
			{
				fwRwChannelramping = fwRwChannelramping + RAMP;
			}			
			if ((fwRwSensAdj - fwRwChannelramping) < 0)
			{
				fwRwChannelramping = fwRwChannelramping - RAMP;
			}
		}
		else
		{
			fwRwChannelramping = fwRwSensAdj;
		}
		
		intervalCounter = 0;
	}
	Serial.print("FwRwRamp: ");
	Serial.print(fwRwChannelramping);

	intervalCounter++;
	
	float maxFwRwSpeed = 1.0 - STEARINGPERCENTAGE;
		
		speedStepper1 = ((maxFwRwSpeed * fwRwChannelramping) - (STEARINGPERCENTAGE * stearChannel))*MAXSTEPSPEED;
		speedStepper2 = ((maxFwRwSpeed * fwRwChannelramping) + (STEARINGPERCENTAGE * stearChannel))*MAXSTEPSPEED;

		Serial.print("SpeedStepper1: ");
		Serial.println(speedStepper1);

		Stepper1.setSpeed(speedStepper1);
		Stepper2.setSpeed(speedStepper2);
		
	Stepper1.runSpeed();
	Stepper2.runSpeed();

	

}

