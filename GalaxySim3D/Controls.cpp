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

	//if(viewData.angleX > 360) viewData.angleX = 0;
	//if(viewData.angleX < 0) viewData.angleX = 360;
	//if(viewData.angleY > 360) viewData.angleY = 0;
	//if(viewData.angleY < 0) viewData.angleY = 360;

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
			//glutReshapeWindow(800, 600);        
			//glutPositionWindow(100,100);
			//glutDestroyWindow(windowID);
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
			for(auto b = buttons.begin(); b != buttons.end(); b++)
			{
				if((*b)->checkForClick(x, y))
				{
					(*b)->click(x, y);
				}
			}
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
