//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

#define sampleFreq	100.0f		// sample frequency in Hz
#define betaDef		0.1f		// 2 * proportional gain

#include <math.h>


class Madgwick {
private:

volatile float beta = betaDef;								// 2 * proportional gain (Kp)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;	// quaternion of sensor frame relative to auxiliary frame
float roll, pitch, yaw;
char anglesComputed;

	static float invSqrt(float x);
	void computeAngles();

//-------------------------------------------------------------------------------------------
// Function declarations

public:

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
	
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
		return 180-yaw * 57.29578f;
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
	
			float A = q0*q1 + q2*q3;
			float B = 0.5f - q1*q1 - q2*q2;
			float odmocninka = invSqrt(A*A+B*B);
			float sin_pitch = ( -2.0f * (q1*q3 - q0*q2));
			float cos_pitch = sqrt(1-sin_pitch*sin_pitch);

			float a_vertical = az* B * odmocninka*cos_pitch - ax * sin_pitch + ay * A * odmocninka *cos_pitch;
			return a_vertical;
			
		
	}
};


extern Madgwick Madgwick_filter;

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
