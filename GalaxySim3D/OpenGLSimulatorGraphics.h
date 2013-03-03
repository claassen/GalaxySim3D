#pragma once
#include "simulatorgraphics.h"

extern class Controls; 

class OpenGLSimulatorGraphics : public SimulatorGraphics
{
public:
	OpenGLSimulatorGraphics(void);
	~OpenGLSimulatorGraphics(void);
	int createWindow(int argc, char **argv);
	void setViewTransform(Controls& controls);
	void setCameraPosition(Controls& controls);
	void setLightingAndMaterials();
	void drawBodies(const std::list<Body> bodies, int sunThresholdSize);
	void drawTree(const BHTree& tree);
private:
	void setShaders();					//not used
	void printInfoLog(GLhandleARB obj); //not used
	void drawBody(const Body& body, int sunThresholdSize);
	void displayTree(const BHTree& node);
};

