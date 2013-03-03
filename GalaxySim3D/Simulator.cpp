#include "StdAfx.h"
#include "Simulator.h"

void Simulator::initSimulation()
{
	tree = new BHTree();
	srand(time(NULL));
	createGalaxy(0.0f, 0.0f, -100.0f);
}

void Simulator::createGalaxy(float x, float y, float z)
{
	createGalaxy(x, y, z, GALAXY_WIDTH, MAX_MASS, NUM_BODIES, 0, 0, 0, GALAXY_ARM_WIDTH);
}

void Simulator::createGalaxy(float x, float y, float z, int diameter, long long centerMass, int numBodies, float dx, float dy, float dz, int armWidth)
{
	long long gcMass = centerMass; //MAX_MASS * 10000;
	long long max_p_mass = centerMass / GCENTER_TO_P_MASS_RATIO + 1;
	float gcX = x;
	float gcY = y;
	float gcZ = z;

	int index = bodies.size() + 1;
	if(index == 0) index = 1;

	bodies.push_back(Body(index, gcMass, gcX, gcY, gcZ, dx, dy, dz));

	for(int i=index+1; i<numBodies+index; i++)
	{
		float px = gcX + (rand() % diameter - diameter / 2);
		float py = gcY + (rand() % armWidth - armWidth / 2);
		float pz = gcZ + (rand() % armWidth - armWidth / 2);
		
		long long pm = createSolarSystemsInGalaxy 
					   && PERCENT_SUNS > 0 
					   && (i % (100 / PERCENT_SUNS)) == 0 ? rand() % max_p_mass * 50 + 1 //make this body a larger "sun" sized one
														  : rand() % max_p_mass + 1;
		float r = Body::distance(gcX, gcY, gcZ, px, py, pz);
		float pv = sqrtf(G * (pm + gcMass) / r);

		float theta = asin(abs(px - gcX) / r);
		float pvx = pv * cos(theta);
		float pvy = pv * sin(theta);

		if(px <= gcX && py <= gcY) {pvx *= -1;}
		if(px > gcX && py <= gcY) {pvx *= -1; pvy *= -1;}
		if(px > gcX && py > gcY) {pvy *= -1;}

		pvx += dx;
		pvy += dy;

		if(createSolarSystemsInGalaxy && PERCENT_SUNS > 0 && (i % (100 / PERCENT_SUNS)) == 0)
		{
			int numPlanets = std::min(MAX_SUN_PLANETS, (100 / PERCENT_SUNS) - 2);
			createGalaxy(px, py, pz, diameter / 20, pm, numPlanets, pvx, pvy, 0, std::max(armWidth / 10, 1));
			i = bodies.size();
			//index += numPlanets;
		}
		else
		{
			bodies.push_back(Body(i, pm, px, py, pz, pvx, pvy, 0));
		}
	}
}

void Simulator::run()
{
	delete(tree);
	updatePositions(DT*0.5);
	createTree();
	doPhysics();
	updateVelocities(DT);
	updatePositions(DT*0.5);
	draw();
}

void Simulator::createTree()
{
	//Get bounding box for BH tree
	if(bodies.size() > 0)
	{
		auto firstBody = bodies.begin();
		minX = firstBody->x - 10;
		maxX = firstBody->x + 10;
		minY = firstBody->y - 10;
		maxY = firstBody->y + 10;
		minZ = firstBody->z - 10;
		maxZ = firstBody->z + 10;
	}
	else
	{
		minX = maxX = minY = maxY = minZ = maxZ = 0;
	}

	for(auto b=bodies.begin(); b!=bodies.end(); b++)
	{
		//Body &b = *b;
		
		b->ax = 0;
		b->ay = 0;
		b->az = 0;
		
		if(b->x < minX) minX = b->x - 10;
		else if(b->x > maxX) maxX = b->x + 10;
		if(b->y < minY) minY = b->y - 10;
		else if(b->y > maxY) maxY = b->y + 10;
		if(b->z < minZ) minZ = b->z - 10;
		else if(b->z > maxZ) maxZ = b->z + 10;
	}

	float w = maxX - minX;
	float h = maxY - minY;
	float d = maxZ - minZ;
	float treeWidth = std::max(w, std::max(h, d));

	//Point treeFblCorner = {tree.cgX - treeWidth / 2, tree.cgY - treeWidth / 2, tree.cgZ - treeWidth / 2};
	Point treeFblCorner = {minX, minY, minZ};

	//Create tree
	tree = new BHTree(treeFblCorner, treeWidth, 0);

	//Populate tree with bodies
	for(auto b=bodies.begin(); b!=bodies.end(); b++)
	{
		try {
			BHTree::insertBody(*tree, *b);
		}
		catch(std::exception e)
		{
			std::cout << "Exception in createTree():\n" << e.what() << std::endl;
		}
	}
}

void Simulator::doPhysics()
{
	//Calculate forces
	for(auto b=bodies.begin(); b!=bodies.end(); b++)
	{
		BHTree::calculateForce(*tree, *b, bodies, COMBINE_MASS_ON_COLLISION);
	}

	//Remove bodies which collided into other bodies
	for(auto b=bodies.begin(); b!=bodies.end();)
	{
		if(b->markForDeletion)
		{
			bodies.erase(b++);
		}
		else
		{
			b++;
		}
	}
}

void Simulator::updateVelocities(float dt)
{
	for(auto b=bodies.begin(); b!=bodies.end(); b++)
	{
		b->dx += b->ax * dt;
		b->dy += b->ay * dt;
		b->dz += b->az * dt;
	}
}

void Simulator::updatePositions(float dt)
{
	for(auto b=bodies.begin(); b!=bodies.end(); b++)
	{
		b->x += b->dx * dt;
		b->y += b->dy * dt;
		b->z += b->dz * dt;
	}
}

void Simulator::draw()
{
	graphics.drawBodies(bodies, SUN_THRESHOLD_SIZE);

	if(ShowTree)
	{
		graphics.drawTree(*tree);
	}
}

void Simulator::clearSpace()
{
	bodies.clear();
	run();
}