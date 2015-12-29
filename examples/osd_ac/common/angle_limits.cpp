/*
 * angle_limits.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: andrew
 */
float limit_360(float a)
{
	if(a < 0.0f)
		return a + 360.0f;
	else
		return a;
}

float limit_plus_minus_180(float a)
{
	if(a > 180.0f)
		return a - 360.0f;
	else if(a <= -180.0f)
		return a + 360.0f;
	else
		return a;
}




