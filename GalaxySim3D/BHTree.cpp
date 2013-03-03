#include "StdAfx.h"
#include "BHTree.h"


void BHTree::subdivide()
{
	isInternalNode = true;

	float halfWidth = width / 2;

	//Front 4 corners
	Point fbl = fblCorner;
	Point fbr = {fbl.x + halfWidth, fbl.y, fbl.z};
	Point fml = {fbl.x, fbl.y + halfWidth, fbl.z};
	Point fmr = {fbr.x, fml.y, fbl.z};

	//Back 4 corners
	Point bbl = {fbl.x, fbl.y, fbl.z + halfWidth};
	Point bbr = {fbr.x, bbl.y, bbl.z};
	Point bml = {fml.x, fml.y, bbl.z};
	Point bmr = {fmr.x, fmr.y, bbr.z};

	try
	{
		//Front 4 quadrants
		fSW = new BHTree(fbl, halfWidth, treeDepth);
		fSE = new BHTree(fbr, halfWidth, treeDepth);
		fNW = new BHTree(fml, halfWidth, treeDepth);
		fNE = new BHTree(fmr, halfWidth, treeDepth);

		//Back 4 quadrants
		bSW = new BHTree(bbl, halfWidth, treeDepth);
		bSE = new BHTree(bbr, halfWidth, treeDepth);
		bNW = new BHTree(bml, halfWidth, treeDepth);
		bNE = new BHTree(bmr, halfWidth, treeDepth);
	}
	catch(std::exception e)
	{
		std::cout << "Exception in subdivide():\n" << e.what() << std::endl;
	}
}

void BHTree::insertBody(BHTree& node, Body& b)
{
	if(!node.hasBody && !node.isInternalNode) {//empty node
		node.body = b;
		node.hasBody = true;
		updateTotalAndCenterMass(node, b);
	}
	else if(node.isInternalNode) {//internal node
		//update total and center of mass
		updateTotalAndCenterMass(node, b);
		
		//recursively insert into correct quadrant
		BHTree& correctQuad = getCorrectQuadForBody(node, b);
		insertBody(correctQuad, b);
	}
	else { //external node that already contains a body
		
		//In order to avoid stack overflow when 2 bodies happen to be extrememly close to each other,
		//we will not try to subdivide a node further if the tree is already too deep. Instead we will simply update
		//the lowest nodes mass and center of mass with the new body (essentially just 'cramming' more than one body
		//into a single node)
		if(node.treeDepth < MAX_TREE_DEPTH)
		{
			node.subdivide();
			Body& existingBody = node.body;
			node.hasBody = false;
			BHTree& correctQuadForNewBody = getCorrectQuadForBody(node, b);
			BHTree& correctQuadForExistingBody = getCorrectQuadForBody(node, existingBody);
			insertBody(correctQuadForNewBody, b);
			insertBody(correctQuadForExistingBody, existingBody);
		}
		else
		{
			std::cout << "MAX_TREE_DEPTH reached" << std::endl;
		}
		
		//update total and center of mass
		updateTotalAndCenterMass(node, b);
	}
}

void BHTree::updateTotalAndCenterMass(BHTree& node, const Body& b)
{
	node.cgX *= node.mass;
	node.cgY *= node.mass;
	node.cgZ *= node.mass;

	node.mass += b.mass;

	node.cgX += b.x*b.mass;
	node.cgY += b.y*b.mass;
	node.cgZ += b.z*b.mass;
	node.cgX /= node.mass;
	node.cgY /= node.mass;
	node.cgZ /= node.mass;
}

BHTree &BHTree::getCorrectQuadForBody(BHTree& node, const Body& b)
{
	if(b.z <= node.bSW->fblCorner.z)
	{
		//Front half
		if(b.x <= node.fSE->fblCorner.x)
		{
			//Left half
			if(b.y <= node.fNW->fblCorner.y)
			{
				//Bottom
				return *node.fSW;
			}
			else
			{
				//Top
				return *node.fNW;
			}
		}
		else
		{
			//Right half
			if(b.y <= node.fNW->fblCorner.y)
			{
				//Bottom
				return *node.fSE;
			}
			else
			{
				//Top
				return *node.fNE;
			}
		}
	}
	else
	{
		//Back half
		if(b.x <= node.fSE->fblCorner.x)
		{
			//Left half
			if(b.y <= node.fNW->fblCorner.y)
			{
				//Bottom
				return *node.bSW;
			}
			else
			{
				//Top
				return *node.bNW;
			}
		}
		else
		{
			//Right half
			if(b.y <= node.fNW->fblCorner.y)
			{
				//Bottom
				return *node.bSE;
			}
			else
			{
				//Top
				return *node.bNE;
			}
		}
	}
}

void BHTree::applyForceToBody(Body& b1, Body& b2, std::list<Body>& bodies, bool canCombineMass)
{
	float distance = Body::distance(b1, b2);

	if(distance < b1.radius + b2.radius)
	{
		//2 bodies are close enough that we consider it a collision,
		//dont update forces
		if(canCombineMass)
		{
			if(b1.mass > b2.mass)
			{
				b1.mass += b2.mass;
				b1.setRadiusAndColor();
				b2.mass = 0;
				b2.markForDeletion = true;
			}
			else
			{
				b2.mass += b1.mass;
				b2.setRadiusAndColor();
				b1.mass = 0;
				b1.markForDeletion = true;
			}
		}
	}
	else
	{
		float d = std::max(distance, b1.radius + b2.radius);

		float fx = Body::forceOfGravity(b1, b2, d, b1.x - b2.x);
		float fy = Body::forceOfGravity(b1, b2, d, b1.y - b2.y);
		float fz = Body::forceOfGravity(b1, b2, d, b1.z - b2.z);

		if(b1.x <= b2.x) fx = abs(fx);
		else fx = abs(fx) * -1;
		if(b1.y <= b2.y) fy = abs(fy);
		else fy = abs(fy) * -1;
		if(b1.z <= b2.z) fz = abs(fz);
		else fz = abs(fz) * -1;

		b1.ax += fx/b1.mass;
		b1.ay += fy/b1.mass;
		b1.az += fz/b1.mass;
	}
}

void BHTree::calculateForce(BHTree& node, Body& b, std::list<Body>& bodies, bool canCombineMass)
{
	if(!node.isInternalNode) {
		if(node.hasBody && node.body != b) {
			//apply force of current node to body
			applyForceToBody(b, node.body, bodies, canCombineMass && true);
		}
	}
	else 
	{
		float s = node.width;
		float d = Body::distance(node.cgX, node.cgY, node.cgZ, b.x, b.y, b.z);
		float ratio = s/d;
		
		if(s/d <= BH_THETA) {
			//apply force of current node to body
			applyForceToBody(b, Body(-1, node.mass, node.cgX, node.cgY, node.cgZ), bodies, false);
		}
		else 
		{
			//recursively apply force of child nodes
			calculateForce(*node.fNW, b, bodies, canCombineMass);
			calculateForce(*node.fNE, b, bodies, canCombineMass);
			calculateForce(*node.fSW, b, bodies, canCombineMass);
			calculateForce(*node.fSE, b, bodies, canCombineMass);
			calculateForce(*node.bNW, b, bodies, canCombineMass);
			calculateForce(*node.bNE, b, bodies, canCombineMass);
			calculateForce(*node.bSW, b, bodies, canCombineMass);
			calculateForce(*node.bSE, b, bodies, canCombineMass);
		}
	}
}

