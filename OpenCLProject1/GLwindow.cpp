#include "GLwindow.h"


GLuint g_vertexArrayId = 0;
GLuint g_shaderId = 0;
const int n = 40;
const int l = 4;

GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

std::vector<GLfloat> generateMesh()
{
	GLsizei const size = n * n * 9 * l * 2 + 18;
	float s = 4.0f / (float)n;

	GLfloat vertices[size];
	float j = 0;
	float k = 0;
	for (int i = 0; i < size - 18; i += 18 * l) {
		if (i % (n * l * 2) == 0 && i != 0) {
			j += s;
			k = 0;
		}

		for (int m = 0; m < l; m++) {
			int idx = i + m * 18;
			vertices[idx] = -2.0f + k,                          vertices[idx + 1] = 0.0f + m * 0.5f,  vertices[idx + 2] = -2.0f + j,
			vertices[idx + 3] = -2.0f - 0.03 * pow(0.8, m) + k, vertices[idx + 4] = -1.0f + m * 0.5f, vertices[idx + 5] = -2.0f + j,
			vertices[idx + 6] = -2.0f + 0.03 * pow(0.2, m) + k, vertices[idx + 7] = -1.0f + m * 0.5f, vertices[idx + 8] = -2.0f + j,

			vertices[idx + 9] = -2.0f + k,  vertices[idx + 10] = 0.0f + m * 0.5f,  vertices[idx + 11] = -2.0f + j,
			vertices[idx + 12] = -2.0f + k, vertices[idx + 13] = -1.0f + m * 0.5f, vertices[idx + 14] = -2.0f - 0.03 * pow(0.8, m) + j,
			vertices[idx + 15] = -2.0f + k, vertices[idx + 16] = -1.0f + m * 0.5f, vertices[idx + 17] = -2.0f + 0.03 * pow(0.2, m) + j;

			//vertices[idx + 9] = -2.0f + k, vertices[idx + 10] = 0.0f + m, vertices[idx + 11] = -2.0f + j,
			//vertices[idx + 12] = -2.0f + k, vertices[idx + 13] = -1.0f + m, vertices[idx + 14] = -2.003f + j,
			//vertices[idx + 15] = -2.0f + k, vertices[idx + 16] = -1.0f + m, vertices[idx + 17] = -1.997f + j;
		}

		k += s;
	}

	vertices[size - 18] = -2.0f, vertices[size - 17] = -1.0f, vertices[size - 16] = -2.0f,
	vertices[size - 15] = -2.0f, vertices[size - 14] = -1.0f, vertices[size - 13] = 2.0f,
	vertices[size - 12] = 2.0f, vertices[size - 11] = -1.0f, vertices[size - 10] = 2.0f,

	vertices[size - 9] = -2.0f, vertices[size - 8] = -1.0f, vertices[size - 7] = -2.0f,
	vertices[size - 6] = 2.0f, vertices[size - 5] = -1.0f, vertices[size - 4] = 2.0f,
	vertices[size - 3] = 2.0f, vertices[size - 2] = -1.0f, vertices[size - 1] = -2.0f;

	std::vector<GLfloat> mesh;

	for (int index = 0; index < size; index++)
	{
		mesh.push_back(vertices[index]);
	}
	
	return mesh;
}

std::vector<GLfloat> generateColorData()
{
	GLsizei const size = n * n * 9 * l * 2 + 18;

	GLfloat raw[size];
	for (int i = 0; i < size; i += 3) {
		if (i < size - 18) {
			raw[i] = 0.98f, raw[i + 1] = 0.941f, raw[i + 2] = 0.745f;
		}
		else {
			raw[i] = 0.98f, raw[i + 1] = 0.824f, raw[i + 2] = 0.745f;
		}
		
	}

	std::vector<GLfloat> colors;

	for (int index = 0; index < size; index++)
	{
		colors.push_back(raw[index]);
	}

	return colors;
}

void initializeOpenGL()
{
	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &g_vertexArrayId);
	glBindVertexArray(g_vertexArrayId);

	GLuint vertexBufferId = 0;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	std::vector<GLfloat> const mesh = generateMesh();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.size(), &mesh[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	GLuint colorBufferId = 0;
	glGenBuffers(1, &colorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	std::vector<GLfloat> const colors = generateColorData();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	glBindVertexArray(0);

	g_shaderId = loadShaders("vertex.glsl", "fragment.glsl");
}

void drawOpenGL(Window const * const _window, clock_t const & _lastInterval)
{
	glUseProgram(g_shaderId);

	GLfloat const rotationAngle = static_cast< GLfloat >(_lastInterval) / 1000.0f * 20.0f;
	Matrix4x4 const rotationMatrix = Matrix4x4::rotate(Vector4(0.0f, 1.0f, 0.0f, 0.0f), DegreeAngle(rotationAngle));
	Matrix4x4 const scalingMatrix = Matrix4x4::scale(100.0f, 100.0f, 100.0f);
	Matrix4x4 const modelMatrix = rotationMatrix * scalingMatrix;

	Camera const camera = _window->getCamera();
	Matrix4x4 const viewMatrix = camera.viewMatrix();
	Matrix4x4 const projectionMatrix = camera.projectionMatrix();

	Matrix4x4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

	GLuint matrixUniform = glGetUniformLocation(g_shaderId, "modelViewPerspective");
	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &modelViewProjectionMatrix.getData());

	glBindVertexArray(g_vertexArrayId);

	glDrawArrays(GL_TRIANGLES, 0, n * n * l * 2 * 3 + 6);//number of vertices

	glBindVertexArray(0);

	glUseProgram(0);
}
