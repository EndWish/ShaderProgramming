#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_Period;
in float a_Amp;
in float a_Radian;
in vec4 a_Color;

uniform float u_Time;

const vec3 c_Gravity = vec3(0.0, -0.8, 0.0);
const float c_LifeTime = 2.0;
const vec3 c_Vel = vec3(0.3, 0.5, 0.0);
const float c_PI = 3.141592;

varying vec4 v_Color;	// == out vec4 v_Color

vec4 Circle();
vec4 GraphSin();
vec4 P1();
vec2 Rotate(vec2 direction, float radian);

void main()
{
	gl_Position = GraphSin();
	v_Color = a_Color;
}

vec4 Circle() {
	vec4 newPosition = vec4(0,0,0,1);
	float time = max(0.f, u_Time - a_EmitTime);

	if(time < 0.0) {

	}
	else {

		float newTime = a_LifeTime * fract(time / a_LifeTime);
		float r = newTime / a_LifeTime;
		float nx = r * sin(a_Radian * newTime);
		float ny = r * cos(a_Radian * newTime);

		newPosition.x = a_Position.x + nx;
		newPosition.y = a_Position.y + ny;
	}

	return newPosition;
}

vec4 GraphSin() {
	vec4 newPosition = vec4(0,0,0,1);
	float time = max(0.f, u_Time - a_EmitTime);

	if(time < 0.0) {

	}
	else {
		vec2 normal = Rotate(normalize(c_Vel).xy, c_PI/2);

		float newTime = a_LifeTime * fract(time / a_LifeTime);
		float nx = sin(a_Radian);
		float ny = cos(a_Radian);

		newPosition.x = a_Position.x + nx + newTime * c_Vel.x;
		newPosition.y = a_Position.y + ny + newTime * c_Vel.y;

		newPosition.xy += normal * a_Amp * (newTime / a_LifeTime) * sin(a_Period * newTime * c_PI * 2.0f);

	}

	return newPosition;
}

vec4 P1()
{
	float time = max(0.f, u_Time - a_EmitTime);
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
	return newPosition;
}

vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}
