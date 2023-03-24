#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}

	CreateParticles(1);
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float vertices[] = { 0,0,0, 1,0,0, 1,1,0 };	//CPU메로리에 생성된다.

	glGenBuffers(1, &m_testVBO);	//glGenBuffers(개수, id가 반환될 변수); Buffer Object ID 가 testVBO에 리턴된다.
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);	//glBindBuffer(형태,id);	// 쓰임새(형태)를 구체화 해준다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*	GPU에 메모리할당하겠다. (GL_STATIC_DRAW은 데이터가 한번 정해지면 바꾸지 않겠다는 의미 vs DYNAMIC)
		glBufferData는 실제로 메모리를 할당하고 올리기 때문에 시간이 많이 걸릴 수 있다.*/

	float vertices2[] = { -1,-1,0, 0,-1,0, 0,0,0 };	//CPU메로리에 생성된다.

	glGenBuffers(1, &m_testVBO1);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	float verticesColors[] = {	1,0,0,1,
						0,1,0,1,
						0,0,1,1 };	//CPU메로리에 생성된다.

	glGenBuffers(1, &m_testVBOColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColors), verticesColors, GL_STATIC_DRAW);

}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float g_time = 0.f;

void Renderer::Class0310_Render() {
	//Program select
	glUseProgram(m_SolidRectShader);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), 1, 1, 1, 1);

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int attribLoc_Position1 = glGetAttribLocation(m_SolidRectShader, "a_Position1");
	glEnableVertexAttribArray(attribLoc_Position1);	// layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);
	glVertexAttribPointer(attribLoc_Position1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Color = -1;
	attribLoc_Color = glGetAttribLocation(m_SolidRectShader, "a_Color");
	glEnableVertexAttribArray(attribLoc_Color);	// layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);
	glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);

	int uniformLoc_Scale = -1;
	uniformLoc_Scale = glGetUniformLocation(m_SolidRectShader, "u_Scale");	// 함수이름이 Attribe인지 Uniform인지 잘 확인하자.
	glUniform1f(uniformLoc_Scale, g_time);
	//g_time += 0.001f;
	//if (g_time > 1.f)
	//	g_time = 0.f;

	glDrawArrays(GL_TRIANGLES, 0, 3);	// 프리미티브, 시작인덱스?, 정점 몇개를 그릴지


}

void Renderer::DrawParticleEffect() {
	//Program select
	int shaderProgram = m_ParticleShader;
	glUseProgram(shaderProgram);

	g_time += 1 / 300.f;

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(shaderProgram, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOPosition);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int attribLoc_Velocity = -1;
	attribLoc_Velocity = glGetAttribLocation(shaderProgram, "a_Velocity");
	glEnableVertexAttribArray(attribLoc_Velocity);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOVelocity);
	glVertexAttribPointer(attribLoc_Velocity, 3, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int attribLoc_Emittime = -1;
	attribLoc_Emittime = glGetAttribLocation(shaderProgram, "a_EmitTime");
	glEnableVertexAttribArray(attribLoc_Emittime);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOEmitTime);
	glVertexAttribPointer(attribLoc_Emittime, 1, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int attribLoc_LifeTime = -1;
	attribLoc_LifeTime = glGetAttribLocation(shaderProgram, "a_LifeTime");
	glEnableVertexAttribArray(attribLoc_LifeTime);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOLifeTime);
	glVertexAttribPointer(attribLoc_LifeTime, 1, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int uniformLoc_Time = -1;
	uniformLoc_Time = glGetUniformLocation(shaderProgram, "u_Time");
	glUniform1f(uniformLoc_Time, g_time);


	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVerticesCount);	// 프리미티브, 시작인덱스?, 정점 몇개를 그릴지
}

void Renderer::CreateParticles(int numParticles) {
	float centerX, centerY;
	float size = 0.01f;
	int particleCount = numParticles;	// 버텍스가 6개 필요
	m_ParticleVerticesCount = particleCount * 6;
	int floatCount = particleCount * 6 * 3;
	int floatCountSingle = particleCount * 6;
	float* vertices = new float[floatCount];

	std::uniform_real_distribution<float> urdPosition(-1, 1);

	float vertexDx[] = { -1, -1, 1, 1, -1, 1 };
	float vertexDy[] = { 1, -1, 1, 1, -1, -1 };

	int index = 0;
	for (int i = 0; i < particleCount; ++i) {
		centerX = 0;// urdPosition(rd);
		centerY = 0;// urdPosition(rd);

		for (int j = 0; j < 6; ++j) {
			vertices[index] = centerX + vertexDx[j] * size; ++index;
			vertices[index] = centerY + vertexDy[j] * size; ++index;
			vertices[index] = 0.f; ++index;
		}
	}

	glGenBuffers(1, &m_particleVBOPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOPosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);

	// velocity
	float* velocities = new float[floatCount];
	std::uniform_real_distribution<float> urdVelocity1(-1.0f, 1.0f);
	std::uniform_real_distribution<float> urdVelocity2(0.0f, 2.0f);

	index = 0;
	for (int i = 0; i < particleCount; ++i) {
		float tempVel[3] = { urdVelocity1(rd), urdVelocity2(rd) , 0 };	// 새로운 속도
		
		// 6개의 점 모두에게 같은 속도를 준다.
		for (int j = 0; j < 6; ++j) {
			velocities[index] = tempVel[0]; ++index;
			velocities[index] = tempVel[1]; ++index;
			velocities[index] = tempVel[2]; ++index;
		}
	}

	glGenBuffers(1, &m_particleVBOVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOVelocity);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, velocities, GL_STATIC_DRAW);	// 이 API가 호출되고 끝나면 해당 데이터는 모두 복사가 된다.

	// emitTime
	float* emitTimes = new float[floatCountSingle];
	std::uniform_real_distribution<float> urdEmitTime(0.0f, 5.0f);

	index = 0;
	for (int i = 0; i < particleCount; ++i) {
		float tempEmitTime = urdEmitTime(rd);	// 새로운 생성시간.

		// 6개의 점 모두에게 같은 생성시간을 준다.
		for (int j = 0; j < 6; ++j) {
			emitTimes[index] = tempEmitTime; ++index;
		}
	}

	glGenBuffers(1, &m_particleVBOEmitTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOEmitTime);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle, emitTimes, GL_STATIC_DRAW);	// 이 API가 호출되고 끝나면 해당 데이터는 모두 복사가 된다.

	// lifeTime
	float* lifeTimes = new float[floatCountSingle];
	std::uniform_real_distribution<float> urdLifeTime(2.0f, 3.0f);

	index = 0;
	for (int i = 0; i < particleCount; ++i) {
		float tempLifeTime = urdLifeTime(rd);	// 새로운 생성시간.

		// 6개의 점 모두에게 같은 생성시간을 준다.
		for (int j = 0; j < 6; ++j) {
			lifeTimes[index] = tempLifeTime; ++index;
		}
	}

	glGenBuffers(1, &m_particleVBOLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOLifeTime);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle, lifeTimes, GL_STATIC_DRAW);	// 이 API가 호출되고 끝나면 해당 데이터는 모두 복사가 된다.

	delete[] vertices;
	delete[] velocities;
	delete[] emitTimes;
	delete[] lifeTimes;
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}
