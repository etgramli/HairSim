#pragma once

#include "Window.hpp"
#include "BodySolver.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <cmath>

// Global BodySolver in main.cpp
extern BodySolver *bodySolver;

void initializeOpenGL();
void drawOpenGL(Window const * const _window, float deltaSeconds);
GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
std::vector<GLfloat> generateMesh();
std::vector<GLfloat> generateColorData();
void updateHair(const float deltaSeconds);
