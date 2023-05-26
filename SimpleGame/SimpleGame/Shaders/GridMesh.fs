#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_TexPos;
in float height;

uniform sampler2D u_TexSampler;
uniform float u_Time;

const float PI = 3.141592f;

void realFlag(){
    vec2 v_Texcoord = fract(v_TexPos * 4.f);

    float period = (v_Texcoord.x + 1.0) * 1.f;
    float xValue = v_Texcoord.x * 2.0f * PI * period;
    float yValue = ((1.0 - v_Texcoord.y) - 0.5) * 2.0f;
    float sinValue = 0.25 * sin(xValue - u_Time * PI * 2);
    if(sinValue * v_Texcoord.x + 0.75 > yValue
    && sinValue * v_Texcoord.x - 0.75 < yValue){
        float vX = v_Texcoord.x;
        float yWidth = 1.5f;
        float yDistance = yValue - (sinValue * v_Texcoord.x - 0.75);
        float vY = 1.0 - yDistance/yWidth;

        FragColor = texture(u_TexSampler, vec2(vX, vY));
        //FragColor = vec4(vX, vY, 0, 1);

    }
    else{
        FragColor = vec4(0);
    }
}

void main()
{
    realFlag();

    //vec2 newTexPos = fract(v_TexPos * 4.f);
    //FragColor = texture(u_TexSampler, newTexPos);
    //FragColor.xyz *= height;


}

