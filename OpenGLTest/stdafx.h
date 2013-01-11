// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//Constants
#define PI 3.1415
#define G 6.67e-6			//-6

//Simulation settings
#define MAX_MASS 100000000  //100000000
#define DEFAULT_NUM_BODIES 300		//300 //*670
#define DEFAULT_GALAXY_WIDTH 500	//600 //*500
#define DEFAULT_GALAXY_ARM_WIDTH 20 //20
#define DEFAULT_PERCENT_SUNS 3		//5   //*0
#define DEFAULT_MAX_SUN_PLANETS 10	//10
#define DEFAULT_GCENTER_TO_P_MASS_RATIO 3000 //5000 //*2500 relative size or orbiting bodies to galaxy center mass
#define DEFAULT_COMBINE_MASS_ON_COLLISION false

//Controls settings
#define MOVE_SPEED 50.0f
#define ROTATE_SPEED 0.03f

struct Point 
{
	float x, y, z;
};


//Standard lib headers
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <list>


//3rd party libs
#include "targetver.h"
#include "include\GL\glew.h"
#include "include\GL\glut.h"
#include <gl\GL.h>
#include <gl\GLU.h>


//Project header files
#include "Button.h"
#include "FileOps.h"
#include "Body.h"
#include "BHTree.h"
#include "Simulator.h"
#include "Controls.h"




