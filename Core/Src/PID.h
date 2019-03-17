/*
 * PID.h
 *
 *  Created on: Mar 16, 2019
 *      Author: kbisland
 */

#ifndef APPLICATION_USER_CORE_PID_PID_H_
#define APPLICATION_USER_CORE_PID_PID_H_
typedef struct PID_Config PID_Config;

struct PID_Config{
   float Kp;
   float Ki;
   float Kd;
   float grav;
   float cor;
   double interval;
   int64_t Setpoint;
   int64_t pos;
   int64_t min;
   int64_t max;
   float upperBound;
   float lowerBound;
   float span;
   float PrevOutput;
   int64_t prevPos;
   float bias;
   float outSpan;

};

void init(PID_Config*, float, float, float, float, float, double);
void setPID(PID_Config*, float, float, float);
void setGrav(PID_Config*, float);
void setCor(PID_Config*, float);
float Compute(PID_Config*);
void setSetpoint(PID_Config*, int64_t);
void setPos(PID_Config*, int64_t);
void setOutputBounds(PID_Config*, float, float);
void setInputBounds(PID_Config*, int64_t, int64_t);




#endif /* APPLICATION_USER_CORE_PID_PID_H_ */
