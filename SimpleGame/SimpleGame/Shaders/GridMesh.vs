#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.

const float PI = 3.141592f;

void main()
{
	float u = a_Position.x  + 0.5;
	float v = -a_Position.y  + 0.5;
	
	float offsetY = sin(u * PI * 2.f) * 0.5f;
	 
	vec4 newPosition = vec4(a_Position.x, a_Position.y + offsetY, a_Position.z, 1.0f);
	
	gl_Position = newPosition;
}
