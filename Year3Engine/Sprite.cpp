#include "Sprite.h"

Sprite::Sprite(std::string filename, std::string name, GLfloat x, GLfloat y)
	: GameComponent(name),
	texture(0)
{
	this->filename = filename;
	positionX = x;
	positionY = y;
	scaleX = 1;
	scaleY = 1;
	rotation = 0;
	bounds = SDL_Rect();
	drawColor[0] = drawColor[1] = drawColor[2] = drawColor[3] = 1.0;
}

Sprite::~Sprite(void)
{
	// Delete the texture
	glDeleteTextures(1, &texture);
}

GLfloat Sprite::getPositionX()
{
	return positionX;
}

GLfloat Sprite::getPositionY()
{
	return positionY;
}

GLfloat Sprite::getRotation()
{
	return rotation;
}

void Sprite::setPositionX(GLfloat x)
{
	positionX = x;
}

void Sprite::setPositionY(GLfloat y)
{
	positionY = y;
}

void Sprite::setRotation(GLfloat rot)
{
	rotation = rot;
}


void Sprite::LoadContent()
{
	// load the sdl sprite
	SDL_Surface* sdlSprite = LoadImage(filename);

	if(sdlSprite != NULL)
	{
		// setup the bounds
		bounds.x = 0;
		bounds.y = 0;
		width = bounds.w = sdlSprite->w;
		height = bounds.h = sdlSprite->h;

		texture = CreateTexture(sdlSprite); // Create the OpenGL Texture
	}
}

void Sprite::Draw()
{
	glColor4f(drawColor[0], drawColor[1], drawColor[2], drawColor[3]);

	if(texture != 0)
	{
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, texture);
		DrawTexture();
	}
}

void Sprite::DrawTexture()
{
	glPushMatrix();

	// Translate
	glTranslatef(getPositionX(), getPositionY(), 0);

	// Rotate on the Z axis
	glRotatef(getRotation(), 0.0f, 0.0f, 1.0f);

	// Scale
	glScalef(scaleX, scaleY, 0);

	// Tell OpenGL we're drawing a quad
	glBegin(GL_QUADS);

	// Bottom-left vertex (corner)
	glTexCoord2f(bounds.x / width, bounds.y / height);
	glVertex3f(-(bounds.w / 2), -(bounds.h / 2), 0);

	// Bottom-right vertex (corner)
	glTexCoord2f((bounds.x + bounds.w) / width, bounds.y / height);
	glVertex3f((bounds.w / 2), -(bounds.h / 2), 0);

	// Top-right vertex (corner)
	glTexCoord2f((bounds.x + bounds.w) / width, (bounds.y + bounds.h) / height);
	glVertex3f((bounds.w / 2), (bounds.h / 2), 0);

	// Top-left vertex (corner)
	glTexCoord2f(bounds.x / width, (bounds.y + bounds.h) / height);
	glVertex3f(-(bounds.w / 2), (bounds.h / 2), 0);

	glEnd();
	glPopMatrix();
}

SDL_Surface* Sprite::LoadImage(std::string filepath) 
{ 
	//The image that's loaded 
	SDL_Surface* loadedImage = NULL; 

	//The optimized image that will be used 
	SDL_Surface* optimizedImage = NULL; 
	
	loadedImage = IMG_Load(filepath.c_str()); 

	//If the image loaded 
	if( loadedImage != NULL ) 
	{ 
		//Create an optimized image 
		optimizedImage = SDL_DisplayFormatAlpha( loadedImage ); 

		//Free the old image 
		SDL_FreeSurface( loadedImage ); 
	} 

	//Return the optimized image 
	return optimizedImage; 
}

GLuint Sprite::CreateTexture(SDL_Surface* sdlSprite)
{
	GLenum texture_format;
	GLint  nOfColors;
	GLuint textureID;

	// get the number of channels in the SDL surface
	nOfColors = sdlSprite->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (sdlSprite->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	} 
	else if (nOfColors == 3)     // no alpha channel
	{
		if (sdlSprite->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	} 
	else 
	{
		printf("warning: the image is not truecolor..  this will probably break\n");
		SDL_Quit();
	}


	// Create the OpenGL Texture Handle
	glGenTextures(1, &textureID);
	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set Texture Wrap to Clamp Mode
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Set Min and Mag Filters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//setup all the information
	//width = (GLfloat)sdlSprite->w;
	//height = (GLfloat)sdlSprite->h;

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, (GLfloat)sdlSprite->w, (GLfloat)sdlSprite->h, 0,
		texture_format, GL_UNSIGNED_BYTE, sdlSprite->pixels );

	//free the image
	SDL_FreeSurface(sdlSprite);

	return textureID;
}

