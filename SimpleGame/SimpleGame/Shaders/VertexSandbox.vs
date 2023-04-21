#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) ������ in�̶�� �Ǿ��ִ� �� ������ vertex Shader�� �Է��� �ȴ�.
out vec3 v_Position;

uniform float u_Time;

const float PI = 3.141592;

void main()
{
	float value = PI * (a_Position.x  - u_Time * 2.f);
	float newY = sin(value) * (a_Position.x + 1) * 0.3f;
	//float newY = (sin(value) * sin(value) * tan(value / 10.f) *  tan(value / 10.f))  * (a_Position.x + 1) * 0.3f;
	vec4 newPos = vec4(a_Position, 1.0f);
	newPos.y = newY;
	gl_Position = newPos;

	v_Position = newPos.xyz;
}
