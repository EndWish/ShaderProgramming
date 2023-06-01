#version 330

layout(location=0) out vec4 FragColor0;
layout(location=1) out vec4 FragColor1;
layout(location=2) out vec4 FragColor2;
layout(location=3) out vec4 FragColor3;
layout(location=4) out vec4 FragColor4;

in vec2 v_Texcoord;

uniform float u_Time;
uniform sampler2D u_Texture;

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

        FragColor0 = vec4(height * v_Texcoord.x, height * v_Texcoord.y, height, 1.0f);
    }
    else{
        float distance = sqrt( pow(v_Texcoord.x - 0.5f, 2) + pow(v_Texcoord.y - 0.5f, 2));
        float height = sin( distance * 100);
        height = pow(height, pow(distance + 1, 5));

        FragColor0 = vec4(height, 0, 0, 1.0f);
    }
}

void circle() {
    FragColor1 = vec4(v_Texcoord, 0, 1.0f);
    vec2 newCoord = v_Texcoord - vec2(0.5f, 0.5f);

    if(length(newCoord) <= 0.5f){
        FragColor1 = vec4(1, 1, 1, 1.0f);
    }else{
        FragColor1 = vec4(0, 0, 0, 1.0f);
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

	FragColor2 = vec4(result + 10*value);
}

void flag(){
    FragColor3 = vec4(0);

    for(int i = 0; i < 5; ++i){
        float newTime = u_Time + i * 0.2f;

        float newColor = v_Texcoord.x * 0.5 * sin(v_Texcoord.x * c_PI * 2 - 10 * newTime);
        float sinValue = sin((v_Texcoord.x * c_PI * 2 * 100) / (1 + v_Texcoord.x * 10) - newTime * 200.f );
        float width = 0.1f * v_Texcoord.x;
        if(newColor < (v_Texcoord.y  - 0.5f) * 2.0f && 
            (v_Texcoord.y  - 0.5f) * 2.0f < newColor + width) {
            FragColor3 = max(FragColor3, vec4(1 * sinValue));
        }
    }


    //FragColor3 = vec4(v_Texcoord, 0, 1);
    //FragColor3 = vec4(newColor);
}

void test(){
    float newValueX = v_Texcoord.x * 100.f * c_PI; // 2*PI로 나눈 개수만큼 줄무늬가 생긴다.
    float newValueY = v_Texcoord.y * 100.f * c_PI; // 2*PI로 나눈 개수만큼 줄무늬가 생긴다.
    float outColorGreyVertical = sin(newValueX);
    float outColorGreyHorizontal = sin(newValueY);
    float newColor = max(outColorGreyVertical, outColorGreyHorizontal);
    FragColor0 = vec4(newColor, newColor, newColor, 1);
}

void realFlag(){
    float period = (v_Texcoord.x + 1.0) * 1.f;
    float xValue = v_Texcoord.x * 2.0f * c_PI * period;
    float yValue = ((1.0 - v_Texcoord.y) - 0.5) * 2.0f;
    float sinValue = 0.25 * sin(xValue - u_Time * 100.f);
    if(sinValue * v_Texcoord.x + 0.75 > yValue
    && sinValue * v_Texcoord.x - 0.75 < yValue){
        float vX = v_Texcoord.x;
        float yWidth = 1.5f;
        float yDistance = yValue - (sinValue * v_Texcoord.x - 0.75);
        float vY = 1.0 - yDistance/yWidth;

        FragColor4 = texture(u_Texture, vec2(vX, vY));
        //FragColor4 = vec4(vX, vY, 0, 1);

    }
    else{
        FragColor4 = vec4(0);
    }
}

void main()
{
    wave();
    circle();
    radar();
    flag();
    realFlag();

    //test();
}

vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}