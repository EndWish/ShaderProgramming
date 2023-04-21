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

//////////////////////////////////////////////////////////////////
 //   vec2 temp = v_Texcoord - vec2(0.5, 0.5);
 //   float d = length(temp);
 //   float value = sin(d * 3 * c_PI - u_Time * c_PI * 3);
 //   value = pow(value, pow(d + 1, 20));
 //   //FragColor = vec4(value);

 //   // 각 타겟의 위치와 (0,0)위치의 거리를 재서 그릴지 안그릴지를 정한다.
 //   for(int i = 0; i < 3; ++i){
 //       FragColor = vec4(v_Texcoord, 0, 1.0f);
 //       vec2 newCoord = v_Texcoord - u_TargetPoses[i];
 //       float dist = length(newCoord);
 //       if(dist <= 0.1f){
 //           FragColor = vec4(value, value, value, 1.0f);
 //           return;
 //       }
 //   }

 //   FragColor = vec4(value, value, value, 1.0f);
}

void main()
{
    //wave();
    //circle();
    radar();
}

vec2 Rotate(vec2 direction, float radian) {
	vec2 result;
	result.x = direction.x * cos(radian) - direction.y * sin(radian);
	result.y = direction.x * sin(radian) - direction.y * cos(radian);
	return result;
}