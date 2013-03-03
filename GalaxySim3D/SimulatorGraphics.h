#pragma once
class SimulatorGraphics
{
public:
	SimulatorGraphics(void);
	~SimulatorGraphics(void);
	virtual void drawBodies(const std::list<Body> bodies, int sunThresholdSize) =0;
	virtual void drawTree(const BHTree& tree) =0;
};


