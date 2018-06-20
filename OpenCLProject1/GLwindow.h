#pragma once

#include "Window.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <cmath>


void initializeOpenGL();
void drawOpenGL(Window const * const _window, float deltaSeconds);
GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
std::vector<GLfloat> generateMesh();
std::vector<GLfloat> generateColorData();
void updateHair(const float deltaSeconds);
