#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) ������ in�̶�� �Ǿ��ִ� �� ������ vertex Shader�� �Է��� �ȴ�.

void main()
{
	vec4 newPosition;
	newPosition.xyz = a_Position;
	newPosition.w = 1;
	gl_Position = newPosition;
}
