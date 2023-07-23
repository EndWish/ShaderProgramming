#version 330

layout(location=0) out vec4 FragColor0;
layout(location=1) out vec4 FragColor1;
layout(location=2) out vec4 FragColor2;
layout(location=3) out vec4 FragColor3;

in vec4 v_Color;
in vec2 v_Texcoord;

uniform float u_Time;
uniform sampler2D u_Texture;

vec2 Rotate(vec2 direction, float radian);

void wave();
void circle();
void Textured();

void main()
{
    Textured();
    FragColor1 = vec4(1,0,0,1);
    FragColor2 = vec4(0,1,0,1);
    FragColor3 = vec4(0,0,1,1);
}


vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}

void Textured() {
    vec4 result = texture(u_Texture, v_Texcoord) * v_Color;
    FragColor0 = result;
}

void circle() {
    FragColor0 = vec4(v_Texcoord, 0, 1.0f);
    vec2 newCoord = v_Texcoord - vec2(0.5f, 0.5f);

    if(length(newCoord) <= 0.5f){
        FragColor0 = vec4(1, 1, 1, 1.0f);
    }else{
        FragColor0 = vec4(0, 0, 0, 0.0f);
    }
}

void wave() {
    vec2 newCoord = v_Texcoord - vec2(0.5f, 0.5f);
    vec2 rotationCoord = Rotate(newCoord, u_Time);

    if(rotationCoord.y > 0.f) {
        float distance = length(newCoord);
        float height = sin(distance * 100 - u_Time * 10);
        height = pow(height, pow(distance + 1, 20));

        FragColor0 = vec4(height * v_Texcoord.x, height * v_Texcoord.y, height, 1.0f);
    }
    else{
        float distance = sqrt( pow(v_Texcoord.x - 0.5f, 2) + pow(v_Texcoord.y - 0.5f, 2));
        float height = sin( distance * 100);
        height = pow(height, pow(distance + 1, 5));

        FragColor0 = vec4(height, 0, 0, 1.0f);
    }
}
