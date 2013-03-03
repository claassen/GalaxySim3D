#include "stdafx.h"
#include "Body.h"


float Body::radiusFromMass(long long mass)
{
	return pow((3/4.0)*mass/PI, 0.333333) / 100 + 0.1; // /100
}

float Body::distance(const Body& b1, const Body& b2)
{
	return sqrtf(pow(b1.x - b2.x, 2) + pow(b1.y - b2.y, 2) + pow(b1.z - b2.z, 2)) + 0.1;
}

float Body::distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return sqrtf(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2)) + 0.1;
}

float Body::forceOfGravity(const Body& b1, const Body& b2, float distance, float axisLength)
{
	return (G*b1.mass*b2.mass/pow(distance, 3)) * axisLength;
}

void Body::setRadiusAndColor()
{
	radius = radiusFromMass(mass);
	float ratio = (float)mass / ((float)DEFAULT_MAX_MASS/(DEFAULT_GCENTER_TO_P_MASS_RATIO*2));
		
	if(mass > DEFAULT_MAX_MASS)
	{
		r = 1.0f;
		b = 1.0f;
		g = 0;
	}
	else
	{
		if(mass < DEFAULT_MAX_MASS / (DEFAULT_GCENTER_TO_P_MASS_RATIO*2))
		{
			r = 1.0;// - ratio; //std::max(ratio * 10, 1.0f);//(rand() % 100) / 100.0;
			b = 1.0;// - ratio; //(rand() % 100) / 100.0;
			g = 1.0 - ratio;//(rand() % 100) / 100.0;
		}
		else
		{
			r = 1.0;
			b = 1.0 - ratio/2;
			g = 1.0 - ratio;
		}
	}
}