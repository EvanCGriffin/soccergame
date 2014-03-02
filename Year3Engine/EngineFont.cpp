#include "EngineFont.h"

#include <iostream>
#include <FTGL\FTGL\FTGLPixmapFont.h>

EngineFont::EngineFont(std::string filename)
	:filename(filename),
	font(NULL)
{
}


EngineFont::~EngineFont(void)
{
	if(font != NULL)
		delete font;
}

void EngineFont::LoadContent()
{
	font = new FTGLPixmapFont(filename.c_str());

	if( font->Error() )
	{
		std::cout << "Error loading Font '" << filename << "'" << std::endl;
		delete font;
		font = NULL;
	}
	else
	{
		//set a default size
		font->FaceSize(16);
	}
}

void EngineFont::SetSize(int size)
{
	if(font != NULL)
		font->FaceSize(size);
}

void EngineFont::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	//do nothing if the font isn't setup
	if(font == NULL)
	{
		return;
	}

	std::string tempStr;

	//fix bug in ftgl when part of the text is offscreen nothing draws
	while( (x < 0) && (text.length() != 0))
	{
		tempStr = text[0];
		x += GetWidth(tempStr);
		text = text.substr(1, text.length());
	}

	glColor4f(r, g, b, a);
	glRasterPos2i((int)x, (int)y);

	glPushMatrix();

	font->Render(text.c_str());

	glPopMatrix();
}

GLfloat EngineFont::GetWidth(std::string text)
{
	return (GLfloat) font->Advance(text.c_str());
}