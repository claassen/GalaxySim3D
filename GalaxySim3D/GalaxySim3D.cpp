// GalaxySim3D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//Set initial view properties
ViewControlData controlsData = {0, 0,				//window size
							    0.0f, 0.0f,			//starting camera angle
							    0.0f, 0.0f, -1.0f,  //starting camera look vector (normally derived from angles)
								1.0f, 0.0f, 0.0f,	//starting camera strafe vector ( ... )
							    0.0f, 1.0f, 50.0f,   //starting camera position
							    0.0f,				//X rotation speed
							    0.0f,				//Y rotation speed
							    0,					//forward move speed
								0};					//strafe move speed


OpenGLSimulatorGraphics graphics;
Simulator simulator(graphics);
Controls controls(simulator, controlsData);


void pressKey(int key, int xx, int yy) 
{
	controls.pressKey(key, xx, yy);
}

void releaseKey(int key, int x, int y) 
{
	controls.releaseKey(key, x, y);
}

void regularKey(unsigned char key, int x, int y)
{
	controls.regularKey(key, x, y);
}

void mouseMove(int x, int y)
{
	controls.mouseMove(x, y);
}

void mouseEntry(int state)
{
	controls.mouseEntry(state);
}

void mouseClick(int button,int state,int x, int y)
{
	controls.mouseClick(button, state, x, y);
}

void changeSize(int w, int h) 
{
	controls.changeSize(w, h);
}


void displayTitle()
{
	std::cout << "\n Galaxy Simulator 3D \n" << std::endl;
	std::cout << "Written by Michael Claassen (2012)\n" << std::endl;
}

void displayUsage()
{
	std::cout << "\nINSTRUCTIONS:" << std::endl;
	std::cout << " Move mouse to edges of screen to look around.\n Use UP and DOWN arrows to move forward/backwards." << std::endl;
	std::cout << " Use LEFT and RIGHT arrows to strafe." << std::endl;
	std::cout << " Press 's' to to toggle showing the BH tree." << std::endl;
	std::cout << " Press 'a' and 'd' to slow down and speed up the simulation." << std::endl;
	std::cout << " Press 'g' to create a new galaxy in front of the camera." << std::endl;
	std::cout << " Press 'q' to quit.\n" << std::endl;
	
	system("PAUSE");
}

void displayOpenGLInfo()
{
	const char* vendor = (const char*)glGetString( GL_VENDOR );
	const char* renderer = (const char*)glGetString( GL_RENDERER );
	const char* version = (const char*)glGetString( GL_VERSION );
	const char* extensions = (const char*)glGetString( GL_EXTENSIONS );

	std::cout << "\nOpenGL Info:" << std::endl;
	std::cout << "Vendor: \t" << vendor << std::endl;
	std::cout << "Renderer: \t" << renderer << std::endl;
	std::cout << "Version: \t" << version << std::endl;
	//std::cout << "Extensions: \t" << extensions << std::endl << std::endl;
}


void Run() 
{
	//Reset view transformation
	graphics.setViewTransform(controls);
	
	// Set the camera
	controls.computeCameraPosition();
	graphics.setCameraPosition(controls);
	
	//Enable lighting and materials
	graphics.setLightingAndMaterials();
	
	//Run the simulation step
	simulator.run();

	//Draw UI buttons
	controls.drawButtons();
	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	displayTitle();

	int windowID = graphics.createWindow(argc, argv);
	displayOpenGLInfo();
	controls.windowID = windowID;

	simulator.initSimulation();

	//Register callbacks (stupid glut api doesnt support passing state information, so we can't use lamdas or bind)
	glutDisplayFunc(Run);
	glutIdleFunc(Run);
	glutSpecialUpFunc(releaseKey);
	glutPassiveMotionFunc(mouseMove);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(regularKey);
	glutSpecialFunc(pressKey);
	glutEntryFunc(mouseEntry);
	glutMouseFunc(mouseClick);

	displayUsage();

	//Enter GLUT event processing cycle
	glutFullScreen();
	glutMainLoop();

	return 0;
}

