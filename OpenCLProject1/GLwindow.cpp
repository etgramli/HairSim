#include "GLwindow.h"
#include "HairPiece.h"


GLuint g_vertexArrayId = 0;
GLuint g_vertexArrayId2 = 1;
GLuint g_shaderId = 0;

std::vector<GLfloat> hairColors;

GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	} else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
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
	if (InfoLogLength > 0) {
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
	if (InfoLogLength > 0) {
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
	if (InfoLogLength > 0) {
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

std::vector<GLfloat> generateFloor() {
	GLsizei const size = 18;
	GLfloat vertices[size];

	vertices[0] = -0.5f, vertices[1] = 0.0f, vertices[2] = -0.5f,
	vertices[3] = -0.5f, vertices[4] = 0.0f, vertices[5] = 4.5f,
	vertices[6] = 4.5f, vertices[7] = 0.0f, vertices[8] = 4.5f,

	vertices[9] = -0.5f, vertices[10] = 0.0f, vertices[11] = -0.5f,
	vertices[12] = 4.5f, vertices[13] = 0.0f, vertices[14] = 4.5f,
	vertices[15] = 4.5f, vertices[16] = 0.0f, vertices[17] = -0.5f;

	std::vector<GLfloat> mesh;

	for (int index = 0; index < size; index++) {
		mesh.push_back(vertices[index]);
	}

	return mesh;
}

std::vector<GLfloat> generateFloorColorData() {
	GLsizei const size = 18;

	GLfloat raw[size];
	for (int i = 0; i < size; i += 3) {
		raw[i] = 0.98f, raw[i + 1] = 0.824f, raw[i + 2] = 0.745f;
	}

	std::vector<GLfloat> colors;

	for (int index = 0; index < size; ++index) {
		colors.push_back(raw[index]);
	}

	return colors;
}

void initializeOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);

	glGenVertexArrays(1, &g_vertexArrayId);
	glBindVertexArray(g_vertexArrayId);

	GLuint vertexBufferId = 0;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	std::vector<GLfloat> const mesh = generateFloor();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.size(), &mesh[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	GLuint colorBufferId = 0;
	glGenBuffers(1, &colorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	std::vector<GLfloat> const colors = generateFloorColorData();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	glBindVertexArray(0);

	srand((unsigned)time(NULL));
	for (GLfloat c : bodySolver->getHairPiece()->getCoordinatesForGL()) {
		float r = (float)rand() / RAND_MAX;
		hairColors.push_back(0.98f - r);
		hairColors.push_back(0.941f - r);
		hairColors.push_back(0.745f - r);
	}
	updateHair(0);

	g_shaderId = loadShaders("vertex.glsl", "fragment.glsl");
}

void drawOpenGL(Window const * const _window, float deltaSeconds) {
	updateHair((float) deltaSeconds);

	glUseProgram(g_shaderId);

	//GLfloat const rotationAngle = static_cast< GLfloat >(_lastInterval) / 1000.0f * 20.0f;
	//Matrix4x4 const rotationMatrix = Matrix4x4::rotate(Vector4(0.0f, 1.0f, 0.0f, 0.0f), DegreeAngle(rotationAngle));
	Matrix4x4 const scalingMatrix = Matrix4x4::scale(50.0f, 50.0f, 50.0f);
	Matrix4x4 const modelMatrix = scalingMatrix; // rotationMatrix * 

	Camera const camera = _window->getCamera();
	Matrix4x4 const viewMatrix = camera.viewMatrix();
	Matrix4x4 const projectionMatrix = camera.projectionMatrix();

	Matrix4x4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

	GLuint matrixUniform = glGetUniformLocation(g_shaderId, "modelViewPerspective");
	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &modelViewProjectionMatrix.getData());

	glBindVertexArray(g_vertexArrayId);

	glDrawArrays(GL_TRIANGLES, 0, 6);//number of vertices

	glBindVertexArray(0);

	glBindVertexArray(g_vertexArrayId2);

	glDrawArrays(GL_LINES, 0, bodySolver->getHairPiece()->getCoordinatesForGL().size() / 3.0f);//number of vertices

	glBindVertexArray(0);

	glUseProgram(0);
}

void updateHair(const float deltaSeconds) {
	bodySolver->pSolve_Links(deltaSeconds);
	glGenVertexArrays(1, &g_vertexArrayId2);
	glBindVertexArray(g_vertexArrayId2);

	GLuint vertexBufferId = 1;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	std::vector<GLfloat> const hairMesh = bodySolver->getHairPiece()->getCoordinatesForGL();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * hairMesh.size(), &hairMesh[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	GLuint colorBufferId = 1;
	glGenBuffers(1, &colorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * hairColors.size(), &hairColors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	glBindVertexArray(0);
 }
