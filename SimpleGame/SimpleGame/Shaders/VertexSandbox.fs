#version 330

layout(location=0) out vec4 FragColor;
in vec3 v_Position;

void main()
{
    //float color = float(int((v_Position.x + 1) * 100) % 5); // 점선으로 만들기 (% 모듈러 사용)
    float color = sin((v_Position.x + 1) * 100) + 0.5f; // sin을 사용하기

    FragColor = vec4(color, color, color, 1.0f - (v_Position.x + 1) / 2.0f);
}