//=============================================================================================
// MahonyAHRS.h
//=============================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
// See: http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=============================================================================================
#ifndef MahonyAHRS_h
#define MahonyAHRS_h
#include <math.h>
#include "Arduino.h"

//--------------------------------------------------------------------------------------------
// Variable declaration

class Mahony {
private:
	float twoKp;		// 2 * proportional gain (Kp)
	float twoKi;		// 2 * integral gain (Ki)
	float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
	float integralFBx, integralFBy, integralFBz;  // integral error terms scaled by Ki
	float invSampleFreq;
	float roll, pitch, yaw;
	char anglesComputed;
	static float invSqrt(float x);
	void computeAngles();

//-------------------------------------------------------------------------------------------
// Function declarations

public:
	Mahony();
	void begin(float sampleFrequency) { invSampleFreq = 1.0f / sampleFrequency; }
	void update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
	void updateIMU(float gx, float gy, float gz, float ax, float ay, float az);
	float getRoll() {
		if (!anglesComputed) computeAngles();
		return roll * 57.29578f;
	}
	float getPitch() {
		if (!anglesComputed) computeAngles();
		return pitch * 57.29578f;
	}
	float getYaw() {
		if (!anglesComputed) computeAngles();
		return yaw * 57.29578f + 180.0f;
	}
	float getRollRadians() {
		if (!anglesComputed) computeAngles();
		return roll;
	}
	float getPitchRadians() {
		if (!anglesComputed) computeAngles();
		return pitch;
	}
	float getYawRadians() {
		if (!anglesComputed) computeAngles();
		return yaw;
	}
	
	float getVertical(float ax, float ay, float az){
			//if (!anglesComputed) computeAngles();
			//roll = atan2f(q0*q1 + q2*q3, 0.5f - q1*q1 - q2*q2);
			
			float A = q0*q1 + q2*q3;
			float B = 0.5f - q1*q1 - q2*q2;
			float odmocninka = invSqrt(A*A+B*B);
			float sin_pitch = ( -2.0f * (q1*q3 - q0*q2));
			float cos_pitch = sqrt(1-sin_pitch*sin_pitch);
		//	float cos_pitch = cos(pitch);
		/*	SerialUSB.print(";roll:;");
			SerialUSB.print(roll);
			SerialUSB.print(";cos:;");
			SerialUSB.print(cos(roll));
			SerialUSB.print(";cos_ojeb:;");
			SerialUSB.println(A * invSqrt(A*A+B*B)*cos(pitch));
						*/
						/*
						//cos roll
						A * invSqrt(A*A+B*B)
						
						//sin roll
						B * invSqrt(A*A+B*B)
						*/
			
			//float a_vertical = az*cos(roll)*cos(pitch) - ax*sin(pitch) + ay*sin(roll)*cos(pitch);
			float a_vertical = az* B * odmocninka*cos_pitch - ax * sin_pitch + ay * A * odmocninka *cos_pitch;
			return a_vertical;
			
		
	}
};

#endif
