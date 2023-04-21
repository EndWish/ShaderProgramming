#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.
in vec2 a_Texcoord;

out vec2 v_Texcoord;

uniform float u_Time;

uniform vec2 u_TargetPos;
uniform vec2 u_TargetPoses[10];

void main()
{
	gl_Position = vec4(a_Position, 1.0);
	v_Texcoord = a_Texcoord;
}
