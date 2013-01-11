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


//Shader program variables
GLhandleARB v,f,f2,p;
float lpos[4] = {100,100,100,0}; //light position
//GLint loc;


//Simulation and control objects
Simulator simulator;
Controls controls(simulator, controlsData);
std::vector<IButton*> buttons;



//Shaders
GLhandleARB theProgram;


//Lighting settings
GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
GLfloat whiteDiffuseLight[] = {2.0, 2.0, 2.0}; //set the diffuse light to white
GLfloat mat_specular[] = { 1.0, .5f, .5f, .5f };
GLfloat mat_shininess[] = { 50.0 };


void clearButtonCallback(int x, int y)
{
	simulator.clearSpace();
}

void createButtons()
{
	FunctionButton *clearButton = new FunctionButton();
	clearButton->x = 10;
	clearButton->y = 10;
	clearButton->width = 100;
	clearButton->height = 40;
	clearButton->text = "Clear Space";
	clearButton->callback = clearButtonCallback;

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


void printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
					 &infologLength);

    if (infologLength > 0)
    {
	infoLog = (char *)malloc(infologLength);
	glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
	std::cout << infoLog << std::endl;
	free(infoLog);
    }
}

void setShaders() 
{
	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	f2 = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);


	vs = textFileRead("toon.vert");
	fs = textFileRead("toon.frag");
	fs2 = textFileRead("toon2.frag");

	const char * ff = fs;
	const char * ff2 = fs2;
	const char * vv = vs;

	glShaderSourceARB(v, 1, &vv,NULL);
	glShaderSourceARB(f, 1, &ff,NULL);
	glShaderSourceARB(f2, 1, &ff2,NULL);

	free(vs);free(fs);

	std::cout << "Compiling shaders..." << std::endl;
	glCompileShaderARB(v);
	glCompileShaderARB(f);
	glCompileShaderARB(f2);

	std::cout << "Vertex shader: ";
	printInfoLog(v);
	std::cout << "Fragment shader: ";
	printInfoLog(f);
	std::cout << "Fragment shader 2: ";
	printInfoLog(f2);

	p = glCreateProgramObjectARB();
	glAttachObjectARB(p,f);
	glAttachObjectARB(p,f2);
	glAttachObjectARB(p,v);

	glLinkProgramARB(p);
	glUseProgramObjectARB(p);

	//loc = glGetAttribLocationARB(p,"color");
}


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
	//std::cout << "\n Enter number of bodies to create:" << std::endl;
	//std::cin >> bodyCount;
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


void Run(void) 
{
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set perspective viewing transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(controls.viewData.windowW==0)?(1):((float)controls.viewData.windowW/controls.viewData.windowH),1,100000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the camera
	controls.computeCameraPosition();
	gluLookAt(controls.CameraX(), controls.CameraY(), controls.CameraZ(),
	     	  controls.CameraLookX(), controls.CameraLookY(),  controls.CameraLookZ(),
			  0.0f, 1.0f,  0.0f);
	
	//Set lighting
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	
	//Run the simulation step
	simulator.run();

	//Draw UI buttons
	controls.drawButtons();
	
	glutSwapBuffers();
}

int initGlut(int argc, char **argv)
{
	//Init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,600);
	int winID = glutCreateWindow("Galaxy Simulator 3D");
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutIgnoreKeyRepeat(1);

	std::cout << "glutInit OK" << std::endl;

	//Enable lighting
	glEnable(GL_LIGHT0);

	//Make OpenGL functions such as 'glUseProgram' available
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "glewInit FAILED" << std::endl;
	}
	std::cout << "glewInit: " << err << std::endl;

	//Check that we will be able to use shaders
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		std::cout << "Ready for GLSL" << std::endl;
	else {
		std::cout << "Not totally ready   \n" << std::endl;
		exit(1);
	}

	//InitializeShaders();
	//setShaders();

	displayOpenGLInfo();

	return winID;
}

int main(int argc, char **argv)
{
	displayTitle();

	int windowID = initGlut(argc, argv);
	controls.windowID = windowID;

	createButtons();
	controls.buttons = buttons;

	simulator.initSimulation();

	//Register callbacks
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

