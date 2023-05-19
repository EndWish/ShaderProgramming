#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <assert.h>

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
	m_FragmentSandboxShader = CompileShaders("./Shaders/FragmentSandbox.vs", "./Shaders/FragmentSandbox.fs");
	m_AlphaClearShader = CompileShaders("./Shaders/AlphaClear.vs", "./Shaders/AlphaClear.fs");
	m_VertexSandboxShader = CompileShaders("./Shaders/VertexSandbox.vs", "./Shaders/VertexSandbox.fs");
	m_TextureSandboxShader = CompileShaders("./Shaders/TextureSandbox.vs", "./Shaders/TextureSandbox.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}

	// 파티클 정점 생성
	CreateParticles(10000);

	// 프래그먼트 샌드박스 정점 생성
	CreateFragmentSandboxVertex(1);

	// 
	CreateAlphaClearVertex(1);

	// vertexSandbox
	CreateVertexSandboxVertex();

	// textureSandbox
	CreateTextureSandboxVertex();
	CreateTexture();

	// Load Texture
	m_RGBTexture = CreatePngTexture("./rgb.png", GL_NEAREST);
	m_Texture0 = CreatePngTexture("./texture1.png", GL_NEAREST);
	m_Texture1 = CreatePngTexture("./texture2.png", GL_NEAREST);
	m_Texture2 = CreatePngTexture("./texture3.png", GL_NEAREST);
	m_Texture3 = CreatePngTexture("./texture4.png", GL_NEAREST);
	m_Texture4 = CreatePngTexture("./texture5.png", GL_NEAREST);
	m_Texture5 = CreatePngTexture("./texture6.png", GL_NEAREST);
	m_MergedTexture = CreatePngTexture("./textureMerged.png", GL_NEAREST);
	
	m_ParticleTexture = CreatePngTexture("./rgb.png", GL_NEAREST);
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
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_time += 1 / 300.f;

	//OutDataFloat(shaderProgram, "a_Position", m_particleVBOPosition, 3);
	//int attribLoc_Position = -1;
	//attribLoc_Position = glGetAttribLocation(shaderProgram, "a_Position");
	//glEnableVertexAttribArray(attribLoc_Position);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	//glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOPosition);
	//glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	//int attribLoc_Color = -1;
	//attribLoc_Color = glGetAttribLocation(shaderProgram, "a_Color");
	//glEnableVertexAttribArray(attribLoc_Color);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	//glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBOColor);
	//glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	//int attribLoc_Velocity = -1;
	//attribLoc_Velocity = glGetAttribLocation(shaderProgram, "a_Velocity");
	//glEnableVertexAttribArray(attribLoc_Velocity);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	//glBindBuffer(GL_ARRAY_BUFFER, m_particleVBOVelocity);
	//glVertexAttribPointer(attribLoc_Velocity, 3, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(shaderProgram, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);

	int attribLoc_Color = -1;
	attribLoc_Color = glGetAttribLocation(shaderProgram, "a_Color");
	glEnableVertexAttribArray(attribLoc_Color);

	int attribLoc_Velocity = -1;
	attribLoc_Velocity = glGetAttribLocation(shaderProgram, "a_Velocity");
	glEnableVertexAttribArray(attribLoc_Velocity);

	int attribLoc_UV = -1;
	attribLoc_UV = glGetAttribLocation(shaderProgram, "a_Texcoord");
	glEnableVertexAttribArray(attribLoc_UV);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBOPositionColorVelUVVBO);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.
	glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (GLvoid*)(3 * sizeof(float)));	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.
	glVertexAttribPointer(attribLoc_Velocity, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (GLvoid*)(7 * sizeof(float)));	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.
	glVertexAttribPointer(attribLoc_UV, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (GLvoid*)(10 * sizeof(float)));	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.
	

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

	int attribLoc_Period = -1;
	attribLoc_Period = glGetAttribLocation(shaderProgram, "a_Period");
	glEnableVertexAttribArray(attribLoc_Period);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBOPeriod);
	glVertexAttribPointer(attribLoc_Period, 1, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int attribLoc_Amp = -1;
	attribLoc_Amp = glGetAttribLocation(shaderProgram, "a_Amp");
	glEnableVertexAttribArray(attribLoc_Amp);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBOAmp);
	glVertexAttribPointer(attribLoc_Amp, 1, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int attribLoc_Radian = -1;
	attribLoc_Radian = glGetAttribLocation(shaderProgram, "a_Radian");
	glEnableVertexAttribArray(attribLoc_Radian);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBORadian);
	glVertexAttribPointer(attribLoc_Radian, 1, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

	int uniformLoc_Time = -1;
	uniformLoc_Time = glGetUniformLocation(shaderProgram, "u_Time");
	glUniform1f(uniformLoc_Time, g_time);

	int texULoc =  glGetUniformLocation(shaderProgram, "u_Texture");
	glUniform1i(texULoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTexture);

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVerticesCount);	// 프리미티브, 시작인덱스?, 정점 몇개를 그릴지

	glDisable(GL_BLEND);
}

void Renderer::DrawFragmentSandbox() {
	g_time += 1 / 3000.f;

	GLuint shader = m_FragmentSandboxShader;
	glUseProgram(shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(shader, "a_Position");
	int texLoc = glGetAttribLocation(shader, "a_Texcoord");
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(texLoc);


	glBindBuffer(GL_ARRAY_BUFFER, m_FragmentSandboxVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	int uniformLoc_Time = -1;
	uniformLoc_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformLoc_Time, g_time);

	int texULoc = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(texULoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	int uniformLoc_TargetPos = -1;
	uniformLoc_TargetPos = glGetUniformLocation(shader, "u_TargetPos");
	glUniform2f(uniformLoc_TargetPos, 0.5f, 0.5f);	// 2f 인것에 주의

	float targetPoses[] = { 0.5,0.5,
		0.0f, 0.0f,
		1.0f, 1.0f, };

	int uniformLoc_TargetPoses = -1;
	uniformLoc_TargetPoses = glGetUniformLocation(shader, "u_TargetPoses");
	glUniform2fv(uniformLoc_TargetPoses, 3, targetPoses);	// 2f 인것에 주의

	glDrawArrays(GL_TRIANGLES, 0, m_FragmentSandboxVertexCount);

}

void Renderer::DrawAlphaClear() {

	GLuint shader = m_AlphaClearShader;
	glUseProgram(shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(shader, "a_Position");
	//int texLoc = glGetAttribLocation(shader, "a_Texcoord");
	glEnableVertexAttribArray(posLoc);
	//glEnableVertexAttribArray(texLoc);


	glBindBuffer(GL_ARRAY_BUFFER, m_AlphaClearVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	int uniformLoc_Time = -1;
	uniformLoc_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformLoc_Time, g_time);

	int uniformLoc_TargetPos = -1;
	uniformLoc_TargetPos = glGetUniformLocation(shader, "u_TargetPos");
	glUniform2f(uniformLoc_TargetPos, 0.5f, 0.5f);	// 2f 인것에 주의

	float targetPoses[] = { 0.5,0.5,
		0.0f, 0.0f,
		1.0f, 1.0f, };

	int uniformLoc_TargetPoses = -1;
	uniformLoc_TargetPoses = glGetUniformLocation(shader, "u_TargetPoses");
	glUniform2fv(uniformLoc_TargetPoses, 3, targetPoses);	// 2f 인것에 주의

	glDrawArrays(GL_TRIANGLES, 0, m_AlphaClearVertexCount);
}

void Renderer::CreateParticles(int numParticles) {
	float centerX, centerY;
	float size = 0.01f;
	int particleCount = numParticles;	// 버텍스가 6개 필요
	m_ParticleVerticesCount = particleCount * 6;
	int floatCount = particleCount * 6 * 3;
	int floatCountSingle = particleCount * 6;
	int floatCount4 = particleCount * 6 * 4;

	float* vertices = new float[floatCount];

	std::uniform_real_distribution<float> urdPosition(-1, 1);

	float vertexDx[] = { -1, -1, 1, 1, -1, 1 };
	float vertexDy[] = { 1, -1, 1, 1, -1, -1 };
	float uvDx[] = { 0, 0, 1, 1, 0, 1 };
	float uvDy[] = { 0, 1, 0, 0, 1, 1 };

	// position
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



	// Color
	std::vector<std::uniform_real_distribution<float>> urdColors{
		std::uniform_real_distribution<float>(0.f, 1.f),
		std::uniform_real_distribution<float>(0.f, 1.f),
		std::uniform_real_distribution<float>(0.f, 1.f),
		std::uniform_real_distribution<float>(1.f, 1.f),
	};
	CreateData(m_ParticleVBOColor, 4, particleCount, urdColors);

	// velocity
	std::vector<std::uniform_real_distribution<float>> urdVelocities{
		std::uniform_real_distribution<float>(-1.0f, 1.0f),
		std::uniform_real_distribution<float>(0.0f, 2.0f),
		std::uniform_real_distribution<float>(0.0f, 0.0f),
	};
	CreateData(m_particleVBOVelocity, 3, particleCount, urdVelocities);

	// position + color
	int floatCountPosColorVelUV = particleCount * 6 * (3 + 4 + 3 + 2);	// pos + color + vel + uv
	float* dataPosColorVels = new float[floatCountPosColorVelUV];

	index = 0;
	
	for (int i = 0; i < particleCount; ++i) {

		centerX = 0;	// urdPosition(rd);
		centerY = 0;	// urdPosition(rd);

		float* tempColorVal = new float[4];
		float* tempVelVal = new float[3];
		for (int j = 0; j < 4; ++j) {
			tempColorVal[j] = urdColors[j](rd);
		}
		for (int j = 0; j < 3; ++j) {
			tempVelVal[j] = urdVelocities[j](rd);
		}

		for (int j = 0; j < 6; ++j) {
			// position
			dataPosColorVels[index] = centerX + vertexDx[j] * size; ++index;
			dataPosColorVels[index] = centerY + vertexDy[j] * size; ++index;
			dataPosColorVels[index] = 0.f; ++index;

			// color
			for (int k = 0; k < 4; ++k) {
				dataPosColorVels[index] = tempColorVal[k]; ++index;
			}

			// velocity
			for (int k = 0; k < 3; ++k) {
				dataPosColorVels[index] = tempVelVal[k]; ++index;
			}

			// uv
			dataPosColorVels[index] = uvDx[j]; ++index;
			dataPosColorVels[index] = uvDy[j]; ++index;

		}

		delete[] tempColorVal;
		delete[] tempVelVal;
	}

	glGenBuffers(1, &m_ParticleVBOPositionColorVelUVVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBOPositionColorVelUVVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountPosColorVelUV, dataPosColorVels, GL_STATIC_DRAW);
	
	delete[] dataPosColorVels;


	
	// emitTime
	std::vector<std::uniform_real_distribution<float>> urdEmitTimes{
		std::uniform_real_distribution<float>(0.0f, 5.0f),
	};
	CreateData(m_particleVBOEmitTime, 1, particleCount, urdEmitTimes);

	// lifeTime
	std::vector<std::uniform_real_distribution<float>> urdLifeTimes{
		std::uniform_real_distribution<float>(2.0f, 3.0f)
	};
	CreateData(m_particleVBOLifeTime, 1, particleCount, urdLifeTimes);

	// period
	std::vector<std::uniform_real_distribution<float>> urdPeriods{
		std::uniform_real_distribution<float>(0.5f, 2.0f)
	};
	CreateData(m_ParticleVBOPeriod, 1, particleCount, urdPeriods);

	// Amp
	std::vector<std::uniform_real_distribution<float>> urdAmps{
		std::uniform_real_distribution<float>(-1.f, 1.0f)
	};
	CreateData(m_ParticleVBOAmp, 1, particleCount, urdAmps);

	// Radian
	std::vector<std::uniform_real_distribution<float>> urdRadians{
		std::uniform_real_distribution<float>(0.f, 3.14159f * 2)
	};
	CreateData(m_ParticleVBORadian, 1, particleCount, urdRadians);

	delete[] vertices;
}

void Renderer::CreateFragmentSandboxVertex(int numParticles) {
	m_FragmentSandboxVertexCount = numParticles * 6;

	int floatCount = 5 * m_FragmentSandboxVertexCount;
	float* vertices = new float[floatCount];

	std::uniform_real_distribution<float> urdPosition(-1, 1);

	float vertexDx[] = { -1, -1, 1, 1, -1, 1 };
	float vertexDy[] = { 1, -1, 1, 1, -1, -1 };
	float uvDx[] = { 0, 0, 1, 1, 0, 1 };
	float uvDy[] = { 0, 1, 0, 0, 1, 1 };

	// position
	int index = 0;
	for (int i = 0; i < numParticles; ++i) {
		float centerX = 0; // urdPosition(rd);
		float centerY = 0; // urdPosition(rd);
		float size = 1.f;

		for (int j = 0; j < 6; ++j) {
			// 정점
			vertices[index] = centerX + vertexDx[j] * size; ++index;
			vertices[index] = centerY + vertexDy[j] * size; ++index;
			vertices[index] = 0.f; ++index;

			vertices[index] = uvDx[j]; ++index;
			vertices[index] = uvDy[j]; ++index;
		}
	}

	glGenBuffers(1, &m_FragmentSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FragmentSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);

	delete[] vertices;
}

void Renderer::CreateAlphaClearVertex(int numParticles) {
	m_AlphaClearVertexCount = numParticles * 6;

	int floatCount = 3 * m_AlphaClearVertexCount;
	float* vertices = new float[floatCount];

	std::uniform_real_distribution<float> urdPosition(-1, 1);

	float vertexDx[] = { -1, -1, 1, 1, -1, 1 };
	float vertexDy[] = { 1, -1, 1, 1, -1, -1 };
	//float uvDx[] = { 0, 0, 1, 1, 0, 1 };
	//float uvDy[] = { 0, 1, 0, 0, 1, 1 };

	// position
	int index = 0;
	for (int i = 0; i < numParticles; ++i) {
		float centerX = 0; // urdPosition(rd);
		float centerY = 0; // urdPosition(rd);
		float size = 1.f;

		for (int j = 0; j < 6; ++j) {
			// 정점
			vertices[index] = centerX + vertexDx[j] * size; ++index;
			vertices[index] = centerY + vertexDy[j] * size; ++index;
			vertices[index] = 0.f; ++index;

			//vertices[index] = uvDx[j]; ++index;
			//vertices[index] = uvDy[j]; ++index;
		}
	}

	glGenBuffers(1, &m_AlphaClearVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_AlphaClearVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);

	delete[] vertices;
}

void Renderer::CreateVertexSandboxVertex() {
	m_HoriLineVertexCount = 100;
	int floatCount = m_HoriLineVertexCount * 3;
	int index = 0;
	float gap = 2.f / ((float)m_HoriLineVertexCount);
	float* lineVerteices = new float[floatCount];
	for (int i = 0; i < m_HoriLineVertexCount; ++i) {
		lineVerteices[index] = (float)i * gap - 1.f; ++index;
		lineVerteices[index] = 0.f; ++index;
		lineVerteices[index] = 0.f; ++index;
	}

	glGenBuffers(1, &m_HoriLineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_HoriLineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, lineVerteices, GL_STATIC_DRAW);

}

void Renderer::CreateTextureSandboxVertex() {
	float vertexDx[] = { -1, -1, 1, 1, -1, 1 };
	float vertexDy[] = { 1, -1, 1, 1, -1, -1 };
	float uvDx[] = { 0, 0, 1, 1, 0, 1 };
	float uvDy[] = { 0, 1, 0, 0, 1, 1 };

	const int floatCount = 6 * 5;
	float vertices[floatCount];

	// position
	int index = 0;
	for (int i = 0; i < 1; ++i) {
		float centerX = 0; // urdPosition(rd);
		float centerY = 0; // urdPosition(rd);
		float size = 1.f;

		for (int j = 0; j < 6; ++j) {
			// 정점
			vertices[index] = centerX + vertexDx[j] * size; ++index;
			vertices[index] = centerY + vertexDy[j] * size; ++index;
			vertices[index] = 0.f; ++index;

			vertices[index] = uvDx[j]; ++index;
			vertices[index] = uvDy[j]; ++index;
		}
	}

	glGenBuffers(1, &m_TextureSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);



}

void Renderer::DrawVertexSandbox() {
	g_time += 1 / 300.f;

	GLuint shader = m_VertexSandboxShader;
	glUseProgram(shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);

	glBindBuffer(GL_ARRAY_BUFFER, m_HoriLineVBO);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int uniformLoc_Time = -1;
	uniformLoc_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformLoc_Time, g_time);

	glDrawArrays(GL_LINE_STRIP, 0, m_HoriLineVertexCount);
	glUniform1f(uniformLoc_Time, g_time + 0.1f);
	glDrawArrays(GL_LINE_STRIP, 0, m_HoriLineVertexCount);
	glUniform1f(uniformLoc_Time, g_time + 0.2f);
	glDrawArrays(GL_LINE_STRIP, 0, m_HoriLineVertexCount);

	glDisable(GL_BLEND);

}


void Renderer::DrawTextureSandbox() {
	g_time += 1 / 300.f;

	GLuint shader = m_TextureSandboxShader;
	glUseProgram(shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(shader, "a_Position");
	int texLoc = glGetAttribLocation(shader, "a_TexPos");
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(texLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	int uniformLoc_Time = -1;
	uniformLoc_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformLoc_Time, g_time);

	int uniformLoc_TargetPos = -1;
	uniformLoc_TargetPos = glGetUniformLocation(shader, "u_TargetPos");
	glUniform2f(uniformLoc_TargetPos, 0.5f, 0.5f);	// 2f 인것에 주의

	float targetPoses[] = { 0.5,0.5,
		0.0f, 0.0f,
		1.0f, 1.0f, };

	int uniformLoc_TargetPoses = -1;
	uniformLoc_TargetPoses = glGetUniformLocation(shader, "u_TargetPoses");
	glUniform2fv(uniformLoc_TargetPoses, 3, targetPoses);	// 2f 인것에 주의

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Texture3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_Texture4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_Texture5);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_MergedTexture);
	glActiveTexture(GL_TEXTURE0);


	GLuint samplerULoc = glGetUniformLocation(shader, "u_TexSampler");
	glUniform1i(samplerULoc, 6);

	GLuint repeatULoc = glGetUniformLocation(shader, "u_XYRepeat");
	glUniform2f(repeatULoc, (float)((int)g_time % 4), 4.f);

	glDrawArrays(GL_TRIANGLES, 0, m_AlphaClearVertexCount);
}

void Renderer::CreateTexture() {
	GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0xFFFFFFFF
	};

	glGenTextures(1, &m_CheckerBoardTexture);
	glBindTexture(GL_TEXTURE_2D, m_CheckerBoardTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// GL_LINER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod) {
	//Load Png
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0) {
		std::cout << "PNG image loading failed : " << filePath << "\n";
		assert(0);
	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

	return temp;
}

void Renderer::CreateData(GLuint& VBO, int floatNumPerVertex, int particleCount, std::vector<std::uniform_real_distribution<float>>& urds) {
	int floatCount = floatNumPerVertex * 6 * particleCount;
	float* datas = new float[floatCount];

	int index = 0;
	float* tempVel = new float[floatNumPerVertex];
	for (int i = 0; i < particleCount; ++i) {

		for (int j = 0; j < floatNumPerVertex; ++j) {
			tempVel[j] = urds[j](rd);
		}

		// 6개의 점 모두에게 같은 속도를 준다.
		for (int j = 0; j < 6; ++j) {
			for (int k = 0; k < floatNumPerVertex; ++k) {
				datas[index] = tempVel[k]; ++index;
			}
		}
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, datas, GL_STATIC_DRAW);

	delete[] tempVel;
	delete[] datas;
}

void Renderer::OutDataFloat(int shaderProgram, const char* varName, GLuint VBO, GLuint nFloatPerVertex) {
	int attribLocation = -1;
	attribLocation = glGetAttribLocation(shaderProgram, varName);
	glEnableVertexAttribArray(attribLocation);	// 해당 번지의 attribute가 활성화된다. layout(location)를 적어준다.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(attribLocation, nFloatPerVertex, GL_FLOAT, GL_FALSE, 0, 0);	// Bind후 Pointer로 넘겨주면 쉐이더는 계속 기억을 하고 있다.

}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}


