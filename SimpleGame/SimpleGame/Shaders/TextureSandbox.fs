#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexSampler;
uniform vec2 u_XYRepeat;
uniform float u_Time;
in vec2 v_TexPos;

void Test(){
    //FragColor = vec4(v_TexPos, 0, 1.f);
    float x = v_TexPos.x;
    float y = 1.0 - abs( fract(v_TexPos.y * 3) * 2 - 1);

    //x = v_TexPos.x * 3.0f;
    //y = v_TexPos.y / 3.0f + (1.0 / 3.0f) * floor(x);

    x = v_TexPos.x * 3.0f;
    y = v_TexPos.y / 3.0f + (1.0 / 3.0f) * floor(3 - x);

    x = v_TexPos.x * 3.0f;
    y = v_TexPos.y / 3.0f + (1.0 / 3.0) * (2 - floor(x));

    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos);
}

void P1(){
    //FragColor = vec4(v_TexPos, 0, 1.f);
    float x = v_TexPos.x;
    float y = 1.0 - abs( v_TexPos.y * 2.0 - 1.0);
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
}

void P2(){
    float x = fract(v_TexPos.x * 3.0);
    float dy = v_TexPos.y / 3.0;
    float y = floor(3.0 * (1.0 - v_TexPos.x)) / 3.0 + dy;
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
}

void P3(){
    float x = fract(v_TexPos.x * 3.0);
    float dy = v_TexPos.y / 3.0;
    float y = floor(3.0 * v_TexPos.x) / 3.0 + dy;
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
}

void P4(){
    float x = v_TexPos.x;
    float dy = fract(v_TexPos.y * 3.0) / 3.0;
    float y = floor(3.0 * (1.0 - v_TexPos.y)) / 3.0 + dy;
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
}

void P5(){
    float x_repeat = u_XYRepeat.x;
    float y_repeat = u_XYRepeat.y;

    float dx = v_TexPos.x * x_repeat;
	float x = fract(dx + floor((1.0 - v_TexPos.y) * y_repeat) * 0.5);
    float y = fract(v_TexPos.y * y_repeat);
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
}

void P6(){
    float x_repeat = u_XYRepeat.x;
    float y_repeat = u_XYRepeat.y;

    float dy = v_TexPos.y * y_repeat;
	float x = fract(v_TexPos.x * x_repeat);
	float y = fract(dy + floor(v_TexPos.x * x_repeat) * 0.5f);
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
}

void P7(){
    //float x_repeat = u_XYRepeat.x;
    //float y_repeat = u_XYRepeat.y;

    float y = fract(v_TexPos.x + v_TexPos.y);
	float x = fract(v_TexPos.x + (1.0 - v_TexPos.y));
	
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
    //FragColor = vec4(y);
}

void MultiTexture() {
    FragColor = texture(u_TexSampler, v_TexPos); 
}

void MergedTexture() {
    int index = int(u_Time) % 6;
    float x = v_TexPos.x;
    float y = (index + v_TexPos.y) / 6.0f;
	
    vec2 newTexPos = vec2(x, y);
    FragColor = texture(u_TexSampler, newTexPos); 
}

void main()
{
    //P7();
    //MultiTexture();
    MergedTexture();
}