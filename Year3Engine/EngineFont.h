#pragma once

#include <FTGL\FTGL\ftgl.h>

#include <string>
#include <SDL\SDL_opengl.h>

// Wraps FreeType font functionality
// Renders a font in openGL at the specified x and y
class EngineFont
{
public:
	EngineFont(std::string filename);
	~EngineFont(void);

	void SetSize(int size);

	void LoadContent(); // Load the font
	// Render a string with this font at the specified x and y and color
	void RenderText(std::string text, GLfloat x = 0, GLfloat y = 0, GLfloat r = 1, GLfloat g = 1, GLfloat b = 1, GLfloat a = 1);
private:
	FTFont* font; // The font to render with
	std::string filename; // The name/filepath of the font
	GLfloat GetWidth(std::string text); // Used to render glyphs beside each other
};

