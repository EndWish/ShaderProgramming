#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;

const vec3 c_Gravity = vec3(0.0, -0.8, 0.0);
const float c_LifeTime = 2.0;
const vec3 c_Vel = vec3(0.1, 0.0, 0.0);
const float c_PI = 3.141592;

void main()
{
	vec4 newPosition = vec4(0,0,0,1);
	float newTime = 10.f * fract(u_Time/10.f);

	newPosition.x = a_Position.x + newTime * c_Vel.x;
	float yTime = (newTime / 10.f) * c_PI * 2.0f;
	newPosition.y = a_Position.y +  sin(yTime) + newTime * c_Vel.y;
	gl_Position = newPosition;

	/*float time = max(0.f, u_Time - a_EmitTime);
	vec4 newPosition = vec4(0,0,0,1);

	if(time < 0.0) {
		
	}
	else {
		float newTime = a_LifeTime * fract(time / a_LifeTime);	// fract : 실수의 정수부만 리턴해줌
		newPosition.xyz = a_Position 
							+ a_Velocity * newTime
							+ 0.5 * c_Gravity * newTime * newTime;
		newPosition.w = 1;
		
	}
	gl_Position = newPosition;*/
}
