#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.

void main()
{
	vec4 newPosition;
	newPosition.xyz = a_Position;
	newPosition.w = 1;
	gl_Position = newPosition;
}
