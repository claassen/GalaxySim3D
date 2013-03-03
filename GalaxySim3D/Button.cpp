#include "stdafx.h"
#include "Button.h"


/*
 * Draws the given text centered in a box specified by x, y, width and height
 */
void drawText(int x, int y, int width, int height, std::string text)
{
	unsigned char *t = (unsigned char *)text.c_str();

	int fontx = x + (width - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, t)) / 2 ;
	int fonty = y + (height+10)/2;

	glRasterPos2i(fontx, fonty);
	
	while(*t != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *t);
		++t;
	}
}

void drawButton(int x, int y, int width, int height, std::string text)
{
	//Draw box
	glBegin(GL_LINE_STRIP);
		glVertex2f(x, y);
		glVertex2f(x, y+height);
		glVertex2f(x+width, y+height);
		glVertex2f(x+width, y);
		glVertex2f(x, y);
	glEnd();

	drawText(x, y, width, height, text);
}

