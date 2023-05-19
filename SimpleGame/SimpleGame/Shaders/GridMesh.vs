#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) ������ in�̶�� �Ǿ��ִ� �� ������ vertex Shader�� �Է��� �ȴ�.

const float PI = 3.141592f;

void main()
{
	float u = a_Position.x  + 0.5;
	float v = -a_Position.y  + 0.5;
	
	float offsetY = sin(u * PI * 2.f) * 0.5f;
	 
	vec4 newPosition = vec4(a_Position.x, a_Position.y + offsetY, a_Position.z, 1.0f);
	
	gl_Position = newPosition;
}
