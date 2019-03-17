/*
 * PID.c
 *
 *  Created on: Mar 16, 2019
 *      Author: kbisland
 */

#include "PID.h"

void init(PID_Config* config, float Kp, float Ki, float Kd, float grav, float cor, double interval){
	config->Kp = Kp;
	config->Ki = Ki;
	config->Kd = Kd;
	config->grav = grav;
	config->cor = cor;
	config->interval = interval;

}


void setPID(PID_Config* config, float Kp, float Ki, float Kd){
	config->Kp = Kp;
	config->Ki = Ki;
	config->Kd = Kd;
}

void setGrav(PID_Config* config, float grav){
	config->grav = grav;

}

void setCor(PID_Config* config, float cor){
	config->cor = cor;

}

float Compute(PID_Config* config){
	//Pull in the input and setpoint, and scale them into percent span.
	float scaledPV = (config->pos - config->min) / config->span;

	if (scaledPV > 1.0) {
		scaledPV = 1.0;
	} else if (scaledPV < 0.0) {
		scaledPV = 0.0;
	}

	float scaledSP = (config->SetPoint_ - config->min) / config->span;
	if (scaledSP > 1.0) {
		scaledSP = 1;
	} else if (scaledSP < 0.0) {
		scaledSP = 0;
	}

	float error = scaledSP - scaledPV;

	//Check and see if the output is pegged at a limit and only
	//integrate if it is not. This is to prevent reset-windup.
	if (!(config->PrevOutput >= 1 && error > 0) && !(config->PrevOutput <= 0 && error < 0)) {
		accError_ += error;
	}

	//Compute the current slope of the input signal.
	float dMeas = (scaledPV - config->prevPos) / interval;

	float scaledBias = 0.0;

	if (usingFeedForward) {
		scaledBias = (config->bias - config->lowerBound) / config->outSpan;
	}

	//Perform the PID calculation.
	float controllerOutput_ = scaledBias + config->Kp * (error + (config->Ki * accError_) - (config->Kd * dMeas));

	//Make sure the computed output is within output constraints.
	if (controllerOutput_ < 0.0) {
		controllerOutput_ = 0.0;
	} else if (controllerOutput_ > 1.0) {
		controllerOutput_ = 1.0;
	}

	//Remember this output for the windup check next time.
	Velocity = (config->prevPos-config->pos)/interval;

	config->prevPos = config->pos;

	config->PrevOutput = controllerOutput_;
	//Scale the output from percent span back out to a real world number.

	return (((controllerOutput_ * config->outSpan) + config->lowerBound)+config->grav+config->cor* Velocity);

}

void setSetpoint(PID_Config* config, int64_t SP){
	config->Setpoint = SP;
}

void setPos(PID_Config* config, int64_t pos){
	config->pos = pos;

}

void setOutputBounds(PID_Config* config, float min, float max){
	config->upperBound = max;
	config->lowerBound = min;
	config->outSpan = max-min;
}

void setInputBounds(PID_Config* config, int64_t min, int64_t max){
	config->max = max;
	config->min = min;
	config->span = max-min;
}


