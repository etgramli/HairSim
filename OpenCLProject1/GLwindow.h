#pragma once
#include "Window.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>

void initializeOpenGL();
void drawOpenGL(Window const * const _window, clock_t const & _lastInterval);
GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
std::vector<GLfloat> generateMesh();
std::vector<GLfloat> generateColorData();