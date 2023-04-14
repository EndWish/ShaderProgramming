#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Texcoord;

uniform float u_Time;

vec2 Rotate(vec2 direction, float radian);

void test() {
    vec2 newCoord = v_Texcoord - vec2(0.5f, 0.5f);
    vec2 rotationCoord = Rotate(newCoord, u_Time);

    if(rotationCoord.y > 0.f) {
        float distance = length(newCoord);
        float height = sin(distance * 100 - u_Time * 10);
        height = pow(height, pow(distance + 1, 20));

        FragColor = vec4(height * v_Texcoord.x, height * v_Texcoord.y, height, 1.0f);
    }
    else{
        float distance = sqrt( pow(v_Texcoord.x - 0.5f, 2) + pow(v_Texcoord.y - 0.5f, 2));
        float height = sin( distance * 100);
        height = pow(height, pow(distance + 1, 5));

        FragColor = vec4(height, 0, 0, 1.0f);
    }
}

void circle() {
    FragColor = vec4(v_Texcoord, 0, 1.0f);
    vec2 newCoord = v_Texcoord - vec2(0.5f, 0.5f);

    if(length(newCoord) <= 0.5f){
        FragColor = vec4(1, 1, 1, 1.0f);
    }else{
        FragColor = vec4(0, 0, 0, 1.0f);
    }
}

void main()
{
    test();
    //circle();
}

vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}