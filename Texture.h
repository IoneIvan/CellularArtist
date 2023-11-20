#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb\stb_image.h>
#include <stb\stb_image_write.h>

class Texture
{
public:
	Texture(const unsigned int, const unsigned int);
	GLuint texture;
	GLuint framebuffer;

};

