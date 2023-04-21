#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) ������ in�̶�� �Ǿ��ִ� �� ������ vertex Shader�� �Է��� �ȴ�.
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
