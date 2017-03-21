#include <AccelStepper.h>
#include "Config.h"

AccelStepper Stepper1(1,STEP1PIN,DIR1PIN);
AccelStepper Stepper2(1, STEP2PIN, DIR2PIN);

#define NUM_RC_CHANNELS 2 //You need to specify how many pins you want to use
#include "PinChangeInt.h"  //If you need pinchangeint you need to include this header
const uint8_t RC_Channel_Pin[NUM_RC_CHANNELS] = {
	6,7 };//Here I specify I want to listen to pins A8 to A13 of my mega

uint16_t RC_Channel_Value[NUM_RC_CHANNELS]; //This variable will contain the values read from the RC signal
#include "RCLib.h" //This include needs all declarations above. Do not try to move it up or it won't compile
float speedStepper1 = 0;
float speedStepper2 = 0;
float rcChanel1 = 0.0;
float rcChanel2 = 0.0;
float rcChanel1ramping = 0.0;


void setup()
{
	SetRCInterrupts(); //This method will do all the config foe you.
	int flag;
	Serial.begin(57600);
	Serial.println(F("Rc serial oscilloscope demo"));
	SetRCInterrupts(); //This method will do all the config foe you.
					   //Note some problems will be reported on the serial monitor
	Serial.println(F("Interrupts Set; starting "));
	
	Stepper1.setMaxSpeed(MAXSTEPSPEED);
//	Stepper1.setAcceleration(1000);
//	Stepper1.setSpeed(100);
	Stepper2.setMaxSpeed(MAXSTEPSPEED);
//	Stepper2.setAcceleration(2000);
}

void loop()
{
	int flag;
	if (flag = getChannelsReceiveInfo()) //Here you read the RC flag contains all the channels that have a response
										 // see duane's excellent articles on how this works
	{
		rcChanel1 = map(RC_Channel_Value[0], 1100, 1900, -100, 100) / 100.00;
		rcChanel2 = map(RC_Channel_Value[1], 1100, 1900, -100, 100) / 100.00;
		Serial.print(RC_Channel_Value[0]);
		Serial.print(",");
		Serial.print(rcChanel1);
		Serial.print(" ");
		Serial.print(RC_Channel_Value[1]);
		Serial.print(",");
		Serial.println(rcChanel2);

		if (rcChanel1 < 0.05 & rcChanel1 > -0.05)
		{
			rcChanel1 = 0;
		}

		if (rcChanel2 < 0.05 & rcChanel2 > -0.05)
		{
			rcChanel2 = 0;
		}

		rcChanel1ramping = rcChanel1;
		
		float maxFwRwSpeed = 1.0 - STEARINGPERCENTAGE;
		
		speedStepper1 = ((maxFwRwSpeed * rcChanel1ramping) - (STEARINGPERCENTAGE * rcChanel2))*MAXSTEPSPEED;
		speedStepper2 = ((maxFwRwSpeed * rcChanel1ramping) + (STEARINGPERCENTAGE * rcChanel2))*MAXSTEPSPEED;

		Stepper1.setSpeed(speedStepper1);
		Stepper2.setSpeed(speedStepper2);
	
	}

	Stepper1.runSpeed();
	Stepper2.runSpeed();

}

