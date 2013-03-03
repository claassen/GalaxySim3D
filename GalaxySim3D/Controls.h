#pragma once

struct ViewControlData {
	int windowW, windowH; //window size
	float angleX, angleY; //current camera angle
	float lx, ly, lz;     //camera look vector (derived from angles)
	float sx, sy, sz;	  //camera strafe vector
	float x, y, z;        //camera position
	float deltaAngleX;	  //indicates we are moving the camera about the X axis
	float deltaAngleY;    //indicates we are moving the camera about the Y axis
	float deltaMove;      //indicates we are moving the camera position along the Z axis
	float deltaStrafe;
};

class Controls
{
public:
	int windowID;
	Simulator& simulator;
	ViewControlData viewData;
	std::vector<IButton*> buttons;

	Controls(Simulator& sim, ViewControlData viewData) 
		: simulator(sim), viewData(viewData) 
	{
		buttons = std::vector<IButton*>();
		createButtons();
	}
	~Controls()
	{
		for(int i=0; i<buttons.size(); i++)
		{
			delete(buttons[i]);
		}
	}

	void pressKey(int key, int xx, int yy);
	void releaseKey(int key, int x, int y);
	void regularKey(unsigned char key, int x, int y);
	void mouseMove(int x, int y);
	void mouseEntry(int state);
	void mouseClick(int button, int state, int x, int y);
	void changeSize(int w, int h);
	void computeCameraPosition();
	void drawButtons();
	int WindowWidth();
	int WindowHeight();
	float CameraX() { return viewData.x; };
	float CameraY() { return viewData.y; };
	float CameraZ() { return viewData.z; };
	float CameraLookX() { return viewData.x + viewData.lx; };
	float CameraLookY() { return viewData.y + viewData.ly; };
	float CameraLookZ() { return viewData.z + viewData.lz; };
private:
	void createButtons();
	void computePos();
	void computeDir();
};

