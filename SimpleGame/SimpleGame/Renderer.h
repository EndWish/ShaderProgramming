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
	void DrawFragmentSandbox();
	void DrawAlphaClear();
	void DrawVertexSandbox();
	void DrawTextureSandbox();
	void DrawGridMesh();
	void CreateTexture();
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod);
	

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

	GLuint m_ParticleVBOPositionColorVelUVVBO = -1;	// pos + color VBO + uv

	///////
	void CreateFragmentSandboxVertex(int numVertex);
	GLuint m_FragmentSandboxShader = 0;
	GLuint m_FragmentSandboxVBO = 0;
	GLuint m_FragmentSandboxVertexCount = 0;


	//// 화면 전체에 (0,0,0,0.2) 반투명한 검정색을 그린다.
	void CreateAlphaClearVertex(int numParticles);
	GLuint m_AlphaClearShader = 0;
	GLuint m_AlphaClearVBO = -1;
	GLuint m_AlphaClearVertexCount = 0;

	// 
	void CreateVertexSandboxVertex();

	GLuint m_VertexSandboxShader = 0;
	GLuint m_HoriLineVBO = 0;
	GLuint m_HoriLineVertexCount = 0;

	//
	void CreateTextureSandboxVertex();
	GLuint m_TextureSandboxShader = 0;
	GLuint m_TextureSandboxVBO = 0;
	GLuint m_CheckerBoardTexture = 0;
	GLuint m_RGBTexture = 0;

	//
	GLuint m_Texture0 = 0;
	GLuint m_Texture1 = 0;
	GLuint m_Texture2 = 0;
	GLuint m_Texture3 = 0;
	GLuint m_Texture4 = 0;
	GLuint m_Texture5 = 0;
	GLuint m_MergedTexture = 0;
	GLuint m_ExploreTexture = 0;

	GLuint m_ParticleTexture = 0;

	//
	void CreateGridMesh();
	GLuint m_GridMeshShader = 0;
	GLuint m_GridMeshVertexCount = 0;
	GLuint m_GridMeshVBO = 0;

};

