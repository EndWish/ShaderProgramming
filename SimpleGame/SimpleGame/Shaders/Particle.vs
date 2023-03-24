#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) ������ in�̶�� �Ǿ��ִ� �� ������ vertex Shader�� �Է��� �ȴ�.
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;

const vec3 c_Gravity = vec3(0.0, -0.8, 0.0);
const float c_LifeTime = 2.0;

void main()
{
	float time = max(0.f, u_Time - a_EmitTime);
	vec4 newPosition = vec4(0,0,0,1);

	if(time < 0.0) {
		
	}
	else {
		float newTime = a_LifeTime * fract(time / a_LifeTime);	// fract : �Ǽ��� �����θ� ��������
		newPosition.xyz = a_Position 
							+ a_Velocity * newTime
							+ 0.5 * c_Gravity * newTime * newTime;
		newPosition.w = 1;
		
	}
	gl_Position = newPosition;
}
