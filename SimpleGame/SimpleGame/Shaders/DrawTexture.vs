#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.
in vec2 a_TexPos;

out vec2 v_TexPos;

uniform vec4 u_PosScale;

void main()
{
	vec4 newPosition = vec4(a_Position.xy * u_PosScale.zw, 0.0, 1.0);
	newPosition.xy = newPosition.xy + u_PosScale.xy;
	gl_Position = newPosition;
	v_TexPos = vec2(a_TexPos.x, 1.0 - a_TexPos.y);
}
