#pragma once

class Simulator
{
public:
	std::list<Body> bodies;
	BHTree *tree;
	//GLint loc;
	int windowID; //needed to destroy the window when user presses 'q'
	bool ShowTree;//toggle for showing the BH tree
	bool createSolarSystemsInGalaxy;

	float DT;		//simulation speed
	int NUM_BODIES;
	int GALAXY_WIDTH;
	int GALAXY_ARM_WIDTH;
	int PERCENT_SUNS;
	int MAX_SUN_PLANETS;
	int GCENTER_TO_P_MASS_RATIO;
	bool COMBINE_MASS_ON_COLLISION;

	Simulator() 
		: DT(0.1f) 
	{
		tree = new BHTree();
		createSolarSystemsInGalaxy = true;
		NUM_BODIES = DEFAULT_NUM_BODIES;
		GALAXY_WIDTH = DEFAULT_GALAXY_WIDTH;
		GALAXY_ARM_WIDTH = DEFAULT_GALAXY_ARM_WIDTH;
		PERCENT_SUNS = DEFAULT_PERCENT_SUNS;
		MAX_SUN_PLANETS = DEFAULT_MAX_SUN_PLANETS;
		GCENTER_TO_P_MASS_RATIO = DEFAULT_GCENTER_TO_P_MASS_RATIO;
		COMBINE_MASS_ON_COLLISION = DEFAULT_COMBINE_MASS_ON_COLLISION;
	}

	void initSimulation();
	void run();
	void createGalaxy(float x, float y, float z);
	void createGalaxy(float x, float y, float z, int diameter, long long centerMass, int numBodies, float dx, float dy, float dz, int armWidth);
	void clearSpace();
private:
	float minX, maxX, minY, maxY, minZ, maxZ; //bounding box for BH tree

	void drawSphere(const Body& b);
	void updatePositions(float dt);
	void updateVelocities(float dt);
	void createTree();
	void doPhysics();
	void draw();
	void displayTree(const BHTree& node);
};

