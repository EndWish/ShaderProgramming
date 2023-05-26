#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.

out float height;
out vec2 v_TexPos;

uniform float u_Time;

const float PI = 3.141592f;

vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}

void P1() {
	float u = a_Position.x  + 0.5;
	float v = -a_Position.y  + 0.5;
	height = (sin((u - u_Time) * PI * 2.f) + 1.0f) / 2.0f;
	
	float offsetY = sin((u - u_Time) * PI * 2.f) * 0.5f * u;
	 
	vec4 newPosition = vec4(a_Position.x, a_Position.y + offsetY, a_Position.z, 1.0f);
	float dist = distance(newPosition, vec4(1,-1,0,1)) / 3.f;
	newPosition = mix(newPosition, vec4(1,-1,0,1), pow(fract(u_Time / 2.f) * 1.5f * (1.f - pow(dist,5.f)), 2.f) );

	v_TexPos = vec2(u, v);
	gl_Position = newPosition;
}

void P2() {
	float u = a_Position.x  + 0.5;
	float v = -a_Position.y  + 0.5;
	height = (sin((u - u_Time) * PI * 2.f) + 1.0f) / 2.0f;
	
	float offsetY = sin((u - u_Time) * PI * 2.f) * 0.5f * u;
	 
	vec4 newPosition = vec4(a_Position.x, a_Position.y + offsetY, a_Position.z, 1.0f);
	float dist = distance(newPosition, vec4(0,0,0,1)) / 3.f;
	//newPosition = mix(newPosition, vec4(0,0,0,1), fract(u_Time));
	newPosition.xy = Rotate(newPosition.xy, fract(u_Time));

	v_TexPos = vec2(u, v);
	gl_Position = newPosition;
}

void main()
{
	P1();
}
