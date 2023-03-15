#version 330

layout(location = 0) in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.
layout(location = 1) in vec3 a_Position1;	// ATTRIBUTE (VS INPUT)

uniform vec4 u_Trans;

uniform float u_Scale;

void main()
{
	vec4 newPosition;
	newPosition.xy = u_Scale * (a_Position.xy + a_Position1.xy) * u_Trans.w + u_Trans.xy;
	newPosition.z = 0;
	newPosition.w = 1;
	gl_Position = newPosition;
}
