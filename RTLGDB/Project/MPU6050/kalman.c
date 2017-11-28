#include "kalman.h"

const double Q_angle = 0.01;
const double Q_bias = 0.003;
const double R_measure = 0.03;

static double angle;
static double bias;
static double rate;

double P[2][2];
double K[2];
double y;
double S;

void Kalman_Init()
{
	bias = 0;
	rate = 0;
	angle = 180;

	P[0][0] = 0;
	P[0][1] = 0;
	P[1][0] = 0;
	P[1][1] = 0;
}

// Угол в градусах и угловая скорость в градус\сек, период в секундах.
double Kalman_GetAngle(double newAngle, double newRate, double dt)
{
	// Step 1
	rate = newRate - bias;
	angle += dt * rate;

	// Update estimation error covariance - Project the error covariance ahead
	// Step 2
	P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
	P[0][1] -= dt * P[1][1];
	P[1][0] -= dt * P[1][1];
	P[1][1] += Q_bias * dt;

	// Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
	// Calculate Kalman gain - Compute the Kalman gain
	// Step 4
	S = P[0][0] + R_measure;
	// Step 5
	K[0] = P[0][0] / S;
	K[1] = P[1][0] / S;

	// Calculate angle and bias - Update estimate with measurement zk(newAngle)
	// Step 3
	y = newAngle - angle;
	// Step 6
	angle += K[0] * y;
	bias += K[1] * y;

	// Calculate estimation error covariance - Update the error covariance
	// Step 7
	P[0][0] -= K[0] * P[0][0];
	P[0][1] -= K[0] * P[0][1];
	P[1][0] -= K[1] * P[0][0];
	P[1][1] -= K[1] * P[0][1];

	return angle;
}