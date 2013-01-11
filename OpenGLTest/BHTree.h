#pragma once

#define BH_THETA 0.5
#define MAX_TREE_DEPTH 10000

class BHTree
{
public:
	Point fblCorner;		//front bottom left point on the cube that represents this node
	float width;
	long long mass;
	float cgX, cgY, cgZ;	//center of gravity
	Body body;
	bool hasBody;
	bool isInternalNode;	//indicates that his node only contains other nodes, not a single body
	
	//Child nodes
	BHTree *fNW, *fNE, *fSW, *fSE; //"front" 4 quadrants
	BHTree *bNW, *bNE, *bSW, *bSE; //"back" 4 quadrants

	BHTree() {}
	BHTree(Point fblCorner, float width, int parentDepth) 
		: fblCorner(fblCorner), width(width), mass(0), cgX(0), cgY(0), cgZ(0), isInternalNode(false), hasBody(false), treeDepth(parentDepth + 1) {}
				
	//Static methods
	static void insertBody(BHTree& node, Body& b);
	static void updateTotalAndCenterMass(BHTree& node, const Body& b);
	static BHTree &getCorrectQuadForBody(BHTree& node, const Body& b);
	static void calculateForce(BHTree& node, Body& b, std::list<Body>& bodies, bool canCombineMass);
	static void applyForceToBody(Body& b1, Body& b2, std::list<Body>& bodies, bool canCombineMass);
	static void destroyTree(BHTree *node);
private:
	int treeDepth;

	void subdivide();
};

