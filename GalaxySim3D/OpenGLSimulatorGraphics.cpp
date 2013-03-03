#include "StdAfx.h"
#include "OpenGLSimulatorGraphics.h"

//Material settings
GLfloat yellowEmissiveMaterial[] = {0.9, 0.9, 0.0}; 
GLfloat noneEmissiveMaterial[] = {0.0, 0.0, 0.0};

//Lighting settings
float lpos[4] = {100,100,100,0}; //light position
GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
GLfloat whiteDiffuseLight[] = {2.0, 2.0, 2.0}; //set the diffuse light to white
GLfloat mat_specular[] = { 1.0, .5f, .5f, .5f };
GLfloat mat_shininess[] = { 50.0 };

//Shader variables
GLhandleARB v,f,f2,p;
GLhandleARB theProgram;

OpenGLSimulatorGraphics::OpenGLSimulatorGraphics(void)
{
}


OpenGLSimulatorGraphics::~OpenGLSimulatorGraphics(void)
{
}

int OpenGLSimulatorGraphics::createWindow(int argc, char **argv)
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

	return winID;
}

void OpenGLSimulatorGraphics::setViewTransform(Controls& controls)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set perspective viewing transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(controls.viewData.windowW==0)?(1):((float)controls.viewData.windowW/controls.viewData.windowH),1,100000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OpenGLSimulatorGraphics::setCameraPosition(Controls& controls)
{
	gluLookAt(controls.CameraX(), controls.CameraY(), controls.CameraZ(),
	     	  controls.CameraLookX(), controls.CameraLookY(),  controls.CameraLookZ(),
			  0.0f, 1.0f,  0.0f);
}

void OpenGLSimulatorGraphics::setLightingAndMaterials()
{
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
}

void OpenGLSimulatorGraphics::printInfoLog(GLhandleARB obj)
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

void OpenGLSimulatorGraphics::setShaders() 
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
}

void OpenGLSimulatorGraphics::drawBodies(const std::list<Body> bodies, int sunThresholdSize)
{
	glEnable(GL_LIGHTING);
	for(auto b=bodies.begin(); b!=bodies.end(); b++)
	{
		glPushMatrix();
		glTranslatef(b->x, b->y, b->z);
		drawBody(*b, sunThresholdSize);
		glPopMatrix();
	}
}
	
void OpenGLSimulatorGraphics::drawTree(const BHTree& tree)
{
	glDisable(GL_LIGHTING);
	displayTree(tree);
}

void OpenGLSimulatorGraphics::displayTree(const BHTree& node)
{
	glColor3f(0.0,0.0,0.2); //blue
	glPointSize(0.1);  

	glPushMatrix();
	glTranslatef(node.fblCorner.x + node.width / 2, node.fblCorner.y + node.width / 2, node.fblCorner.z + node.width / 2);
	glutWireCube(node.width);
	glPopMatrix();

	if(node.isInternalNode)
	{
		displayTree(*node.fNE);
		displayTree(*node.fNW);
		displayTree(*node.fSE);
		displayTree(*node.fSW);
		displayTree(*node.bNE);
		displayTree(*node.bNW);
		displayTree(*node.bSE);
		displayTree(*node.bSW);
	}
}

void OpenGLSimulatorGraphics::drawBody(const Body& body, int sunThresholdSize)
{
	//Set correct color and material
	if(body.mass > sunThresholdSize)
	{
		//"Sun" sized body
		glEnable(GL_COLOR_MATERIAL);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowEmissiveMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowEmissiveMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowEmissiveMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellowEmissiveMaterial);
	}
	else
	{
		//Regular body
		glDisable(GL_COLOR_MATERIAL);
		GLfloat colorMaterial[] = {body.r, body.b, body.g};
		GLfloat colorSpecMaterial[] = {body.r, body.b, body.g};
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noneEmissiveMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorSpecMaterial);
	}
		
	// Draw sphere
	glutSolidSphere(body.radius,20,20);
}