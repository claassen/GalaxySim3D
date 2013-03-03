
#pragma once

typedef void (*ButtonCallback)(int x, int y);


//Forward declarations of global utility functions defined in Button.cpp
void drawText(int x, int y, int width, int height, std::string text);
void drawButton(int x, int y, int width, int height, std::string text);


//Abstract class
class IButton
{
public:
	int x, y, width, height;
	std::string text;

	bool checkForClick(int mouseX, int mouseY)
	{
		return mouseX > x && mouseX < x+width &&
	           mouseY > y && mouseY < y+height;
	}
	virtual void draw()
	{
		glColor3f(0.7f,0.7f,0.8f);
		drawButton(x, y, width, height, text);
	}
	virtual void click(int mouseX, int mouseY) =0; //no default implementation, makes class abstract
};


//ToggleButton class
class ToggleButton : public IButton
{
public:
	bool& controlValue;

	ToggleButton(bool& controlValue) : controlValue(controlValue) {}
	
	void click(int mouseX, int mouseY)
	{
		controlValue = !controlValue;
	}
	void draw()
	{
		glColor3f(0.7f,0.7f,0.8f);
		drawText(x, y, width, height / 6, text);

		std::string useText;
		if(controlValue)
		{
			useText = "ON";
			glColor3f(0.0f,1.0f,0.0f);
		}
		else
		{
			useText = "OFF";
			glColor3f(0.7f,0.7f,0.8f);
		}

		drawButton(x, y + height / 4, width, height * 0.75, useText);
	}
};


//IncrementButton class
template <typename T>
class IncrementButton : public IButton
{
public:
	T& controlValue;
	T increment;
	T minVal, maxVal;

	IncrementButton(T& controlValue) : controlValue(controlValue) {}
	
	void click(int mouseX, int mouseY)
	{
		if(mouseX < x + width / 2)
		{
			//Decrement
			controlValue -= increment;
		}
		else
		{
			//Increment
			controlValue += increment;
		}
	}
	void draw()
	{
		glColor3f(0.7f,0.7f,0.8f);
		std::stringstream ss;
		ss << controlValue;
		drawText(x, y, width, height / 6, text + ": " + ss.str());
		drawButton(x, y + height / 4, width / 2, height * 0.75, "-");
		drawButton(x + width / 2, y + height / 4, width / 2, height * 0.75, "+");
	}
};


//FunctionButton class
class FunctionButton : public IButton
{
public:
	//ButtonCallback callback;
	std::function<void(int,int)> callback;

	void click(int mouseX, int mouseY)
	{
		callback(x, y);
	}
};
