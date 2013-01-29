#pragma once

class Body
{
public:
	int id;
	long long mass;
	float x, y, z;
	float radius;
	float dx, dy, dz;
	float ax, ay, az;
	float r, g, b;
	bool markForDeletion;

	Body() : id(0) {}
	Body(int id, long long mass, float x, float y, float z, float dx, float dy, float dz) 
		: id(id), mass(mass), x(x), y(y), z(z), dx(dx), dy(dy), dz(dz), ax(0), ay(0), az(0), markForDeletion(false)
	{
		setRadiusAndColor();
	}

	Body(int id, long long mass, float x, float y, float z) 
		: id(id), mass(mass), x(x), y(y), z(z), dx(0), dy(0), dz(0), ax(0), ay(0), az(0), markForDeletion(false)
	{
		setRadiusAndColor();
	}

	bool operator==(const Body& rhs) { return rhs.id == id; }
	bool operator!=(const Body& rhs) { return !operator==(rhs); } 

	void setRadiusAndColor();
	
	//Static methods
	static float radiusFromMass(long long mass);
	static float distance(const Body& b1, const Body& b2);
	static float distance(float x1, float y1, float z1, float x2, float y2, float z2);
	static float forceOfGravity(const Body& b1, const Body& b2, float distance, float axisValue);
};

