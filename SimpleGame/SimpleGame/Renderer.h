#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void Class0310_Render();
	void DrawParticleEffect();
	

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void CreateData(GLuint& VBO, int floatNumPerVertex, int particleCount, std::vector<std::uniform_real_distribution<float>>& urds);
	void OutDataFloat(int shaderProgram, const char* varName, GLuint VBO, GLuint nFloatPerVertex);
	void GetGLPosition(float x, float y, float *newX, float *newY);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_testVBO = 0;
	GLuint m_testVBO1 = 0;
	GLuint m_testVBOColor = 0;

	// particle VBO
	void CreateParticles(int numParticles);
	GLuint m_ParticleShader = 0;
	GLuint m_particleVBOPosition = -1;
	GLuint m_particleVBOVelocity = -1;
	GLuint m_particleVBOEmitTime = -1;
	GLuint m_particleVBOLifeTime = -1;
	GLuint m_ParticleVBOPeriod = -1;
	GLuint m_ParticleVBOAmp = -1;
	GLuint m_ParticleVBORadian = -1;
	GLuint m_ParticleVBOColor = -1;
	GLuint m_ParticleVerticesCount = -1;

	GLuint m_ParticleVBOPositionColorVelVBO = -1;	// pos + color VBO
	

};

