#include "StdAfx.h"
#include "Controls.h"


void Controls::computePos() 
{
	viewData.x += viewData.deltaMove * viewData.lx * 0.1f;
	viewData.y += viewData.deltaMove * viewData.ly * 0.1f;
	viewData.z += viewData.deltaMove * viewData.lz * 0.1f;

	viewData.x += viewData.deltaStrafe * viewData.sx * 0.1f;
	viewData.y += viewData.deltaStrafe * viewData.sy * 0.1f;
	viewData.z += viewData.deltaStrafe * viewData.sz * 0.1f;
}

void Controls::computeDir() 
{
	viewData.angleX += viewData.deltaAngleX;
	viewData.angleY += viewData.deltaAngleY;

	viewData.ly = sin(viewData.angleY);
	float XZfactor = cos(viewData.angleY);
	viewData.lx = XZfactor * sin(viewData.angleX);
	viewData.lz = -XZfactor * cos(viewData.angleX);

	viewData.sx = cos(viewData.angleX);
	viewData.sy = 0; //strafing doesn't affect y vector
	viewData.sz = sin(viewData.angleX);
}

void Controls::pressKey(int key, int xx, int yy) 
{
	switch (key) 
	{
		case GLUT_KEY_UP : viewData.deltaMove = MOVE_SPEED; break;
		case GLUT_KEY_DOWN : viewData.deltaMove = -MOVE_SPEED; break;
		case GLUT_KEY_LEFT : viewData.deltaStrafe = -MOVE_SPEED; break;
		case GLUT_KEY_RIGHT : viewData.deltaStrafe = MOVE_SPEED; break;
	}
}

void Controls::releaseKey(int key, int x, int y) 
{
	switch (key) {
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN :
			viewData.deltaMove = 0;
			break;
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT :
			viewData.deltaStrafe = 0;
			break;
	}
}

void Controls::regularKey(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
			exit(0);
			break;
		case 's':
			simulator.ShowTree = !simulator.ShowTree;
			break;
		case 'a':
			if(simulator.DT > 0.005)
				simulator.DT -= 0.005;
			break;
		case 'd':
			if(simulator.DT < 2)
				simulator.DT += 0.005;
			break;
		case 'g':
			simulator.createGalaxy(CameraLookX(), CameraLookY(), CameraLookZ());
			break;
	}
}

void Controls::mouseMove(int x, int y)
{
	for(auto b = buttons.begin(); b != buttons.end(); b++)
	{
		if((*b)->checkForClick(x, y)) 
		{
			viewData.deltaAngleX = 0;
			viewData.deltaAngleY = 0;
			return;
		}
	}

	if(x < 100)
	{
		viewData.deltaAngleX = -ROTATE_SPEED * ((100.0 - x) / 100.0);
	}
	else if(x > viewData.windowW - 100)
	{
		viewData.deltaAngleX = ROTATE_SPEED * ((100.0 - (viewData.windowW - x)) / 100.0);
	}
	else 
	{
		viewData.deltaAngleX = 0.0f;
	}

	if(y < 100)
	{
		viewData.deltaAngleY = ROTATE_SPEED * ((100.0 - y) / 100.0);
	}
	else if(y > viewData.windowH - 100)
	{
		viewData.deltaAngleY = -ROTATE_SPEED * ((100.0 - (viewData.windowH - y)) / 100.0);
	}
	else 
	{
		viewData.deltaAngleY = 0.0f;
	}
}

void Controls::mouseEntry(int state)
{
	if(state == GLUT_LEFT)
	{
		//DOESNT WORK: mouse click outside window is required for this even to fire
		viewData.deltaAngleX = 0.0f;
		viewData.deltaAngleY = 0.0f;
	}
}

void Controls::mouseClick(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			std::for_each(buttons.begin(), buttons.end(), [x, y](IButton* b) { 
				if(b->checkForClick(x, y)) 
				{
					b->click(x, y);
				} 
			});
		}
	}
}

//Keeps aspect ratio when window size changes
void Controls::changeSize(int w, int h)
{
	viewData.windowW = w;
	viewData.windowH = h;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,100000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void Controls::computeCameraPosition()
{
	if (viewData.deltaMove || viewData.deltaStrafe)
		computePos();

	computeDir();
}

//BUTTONS
void Controls::createButtons()
{
	FunctionButton *clearButton = new FunctionButton();
	clearButton->x = 10;
	clearButton->y = 10;
	clearButton->width = 100;
	clearButton->height = 40;
	clearButton->text = "Clear Space";
	clearButton->callback = [=](int x, int y){simulator.clearSpace();};
	buttons.push_back(clearButton);

	ToggleButton *toggleGalaxiesInSolarSystem = new ToggleButton(simulator.createSolarSystemsInGalaxy);
	toggleGalaxiesInSolarSystem->x = 120;
	toggleGalaxiesInSolarSystem->y = 10;
	toggleGalaxiesInSolarSystem->width = 130;
	toggleGalaxiesInSolarSystem->height = 40;
	toggleGalaxiesInSolarSystem->text = "Create mini solar systems";

	buttons.push_back(toggleGalaxiesInSolarSystem);

	IncrementButton<float> *simSpeedButton = new IncrementButton<float>(simulator.DT);
	simSpeedButton->x = 260;
	simSpeedButton->y = 10;
	simSpeedButton->width = 80;
	simSpeedButton->height = 40;
	simSpeedButton->text = "Sim Speed";
	simSpeedButton->increment = 0.005;
	simSpeedButton->minVal = 0.005;
	simSpeedButton->maxVal = 2;

	buttons.push_back(simSpeedButton);

	IncrementButton<int> *numBodiesButton = new IncrementButton<int>(simulator.NUM_BODIES);
	numBodiesButton->x = 350;
	numBodiesButton->y = 10;
	numBodiesButton->width = 120;
	numBodiesButton->height = 40;
	numBodiesButton->text = "Bodies per Galaxy";
	numBodiesButton->increment = 10;
	numBodiesButton->minVal = 10;
	numBodiesButton->maxVal = 1000;

	buttons.push_back(numBodiesButton);

	IncrementButton<int> *galaxyWidthButton = new IncrementButton<int>(simulator.GALAXY_WIDTH);
	galaxyWidthButton->x = 480;
	galaxyWidthButton->y = 10;
	galaxyWidthButton->width = 100;
	galaxyWidthButton->height = 40;
	galaxyWidthButton->text = "Galaxy width";
	galaxyWidthButton->increment = 10;
	galaxyWidthButton->minVal = 50;
	galaxyWidthButton->maxVal = 1000;

	buttons.push_back(galaxyWidthButton);

	IncrementButton<int> *percentSunsButton = new IncrementButton<int>(simulator.PERCENT_SUNS);
	percentSunsButton->x = 590;
	percentSunsButton->y = 10;
	percentSunsButton->width = 100;
	percentSunsButton->height = 40;
	percentSunsButton->text = "% Suns in galaxy";
	percentSunsButton->increment = 1;
	percentSunsButton->minVal = 1;
	percentSunsButton->maxVal = 50;

	buttons.push_back(percentSunsButton);

	IncrementButton<int> *orbitMassRatioButton = new IncrementButton<int>(simulator.GCENTER_TO_P_MASS_RATIO);
	orbitMassRatioButton->x = 700;
	orbitMassRatioButton->y = 10;
	orbitMassRatioButton->width = 160;
	orbitMassRatioButton->height = 40;
	orbitMassRatioButton->text = "G. center to orbit mass ratio";
	orbitMassRatioButton->increment = 100;
	orbitMassRatioButton->minVal = 500;
	orbitMassRatioButton->maxVal = 50000;

	buttons.push_back(orbitMassRatioButton);

	ToggleButton *toggleShowBHTree = new ToggleButton(simulator.ShowTree);
	toggleShowBHTree->x = 870;
	toggleShowBHTree->y = 10;
	toggleShowBHTree->width = 100;
	toggleShowBHTree->height = 40;
	toggleShowBHTree->text = "Show BH tree";

	buttons.push_back(toggleShowBHTree);

	ToggleButton *toggleCombineMasses = new ToggleButton(simulator.COMBINE_MASS_ON_COLLISION);
	toggleCombineMasses->x = 980;
	toggleCombineMasses->y = 10;
	toggleCombineMasses->width = 120;
	toggleCombineMasses->height = 40;
	toggleCombineMasses->text = "Combine mass on collision";

	buttons.push_back(toggleCombineMasses);
}

void Controls::drawButtons()
{
	//Set orthographic viewing transformation
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,viewData.windowW,viewData.windowH,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Draw the buttons
	for(auto b = buttons.begin(); b != buttons.end(); b++)
	{
		(*b)->draw();
	}
}
