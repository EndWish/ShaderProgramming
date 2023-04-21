#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Texcoord;

uniform float u_Time;

uniform vec2 u_TargetPos;
uniform vec2 u_TargetPoses[10];
const float c_PI = 3.141592;

vec2 Rotate(vec2 direction, float radian);

void wave() {
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

void radar()
{
    vec2 temp = v_Texcoord - vec2(0.5f, 0.5f);
	float d = length(temp); 
	float value = 0.2*(pow(sin(d*2*c_PI - 50*u_Time), 12)-0.5);
	float temp1 = ceil(value);

	vec4 result = vec4(0);
	for(int i=0; i<3; i++)
	{
		vec2 temp = v_Texcoord - u_TargetPoses[i];
		float d = length(temp);

		if(d<0.1)
		{
			result += 1.0*temp1;
		}
	}

	FragColor = vec4(result + 10*value);
}

void flag(){
    FragColor = vec4(0);

    for(int i = 0; i < 5; ++i){
        float newTime = u_Time + i * 0.2f;

        float newColor = v_Texcoord.x * 0.5 * sin(v_Texcoord.x * c_PI * 2 - 10 * newTime);
        float sinValue = sin((v_Texcoord.x * c_PI * 2 * 100) / (1 + v_Texcoord.x * 10) - newTime * 200.f );
        float width = 0.1f * v_Texcoord.x;
        if(newColor < (v_Texcoord.y  - 0.5f) * 2.0f && 
            (v_Texcoord.y  - 0.5f) * 2.0f < newColor + width) {
            FragColor = max(FragColor, vec4(1 * sinValue));
        }
    }


    //FragColor = vec4(v_Texcoord, 0, 1);
    //FragColor = vec4(newColor);
}

void test(){
    float newValueX = v_Texcoord.x * 100.f * c_PI; // 2*PI로 나눈 개수만큼 줄무늬가 생긴다.
    float newValueY = v_Texcoord.y * 100.f * c_PI; // 2*PI로 나눈 개수만큼 줄무늬가 생긴다.
    float outColorGreyVertical = sin(newValueX);
    float outColorGreyHorizontal = sin(newValueY);
    float newColor = max(outColorGreyVertical, outColorGreyHorizontal);
    FragColor = vec4(newColor, newColor, newColor, 1);
}

void main()
{
    //wave();
    //circle();
    //radar();
    flag();
    //test();
}

vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}