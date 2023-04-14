#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) ������ in�̶�� �Ǿ��ִ� �� ������ vertex Shader�� �Է��� �ȴ�.
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_Period;
in float a_Amp;
in float a_Radian;
in vec4 a_Color;
in vec2 a_Texcoord;

out vec2 v_Texcoord;


uniform float u_Time;

const vec3 c_Gravity = vec3(0.0, -0.8, 0.0);
const float c_LifeTime = 2.0;
const vec3 c_Vel = vec3(0.3, 0.5, 0.0);
const float c_PI = 3.141592;

out vec4 v_Color;	// == out vec4 v_Color

void Circle();
void GraphSin();
void P1();
vec2 Rotate(vec2 direction, float radian);

void main()
{
	GraphSin();
	v_Texcoord = a_Texcoord;
}

void Circle() {
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

	
	gl_Position = newPosition;
	v_Color = a_Color;
}

void GraphSin() {
	vec4 newPosition = vec4(0,0,0,1);
	float newAlpha;

	float time = max(0.f, u_Time - a_EmitTime);
	

	if(time < 0.0f) {
		
	}
	else {
	float newTime = a_LifeTime * fract(time / a_LifeTime);		// 0 ~ a_LifeTime
		vec2 normal = Rotate(normalize(c_Vel).xy, c_PI/2);

		
		float nx = sin(a_Radian);
		float ny = cos(a_Radian);

		newPosition.x = a_Position.x + nx + newTime * c_Vel.x;
		newPosition.y = a_Position.y + ny + newTime * c_Vel.y;

		newPosition.xy += normal * a_Amp * (newTime / a_LifeTime) * sin(a_Period * newTime * c_PI * 2.0f);

		newAlpha =  1.0f - newTime / a_LifeTime;
		//newAlpha = pow(newAlpha, 0.5f);
	}

	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, a_Color.a * newAlpha);
}

void P1()
{
	float time = max(0.f, u_Time - a_EmitTime);
	vec4 newPosition = vec4(0,0,0,1);

	if(time < 0.0f) {
		
	}
	else {
		float newTime = a_LifeTime * fract(time / a_LifeTime);	// fract : �Ǽ��� �����θ� ��������
		newPosition.xyz = a_Position 
							+ a_Velocity * newTime
							+ 0.5 * c_Gravity * newTime * newTime;
		newPosition.w = 1;
		
	}
	
	gl_Position = newPosition;
	v_Color = a_Color;
}

vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}
