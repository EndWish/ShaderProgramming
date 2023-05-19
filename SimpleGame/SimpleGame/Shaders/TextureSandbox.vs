#version 330

in vec3 a_Position;	// ATTRIBUTE (VS INPUT) 전역에 in이라고 되어있는 이 변수는 vertex Shader의 입력이 된다.
in vec2 a_TexPos;

out vec2 v_TexPos;

void main()
{
	vec4 newPosition = vec4(a_Position, 1.0f);
	gl_Position = newPosition;
	v_TexPos = a_TexPos;
}
