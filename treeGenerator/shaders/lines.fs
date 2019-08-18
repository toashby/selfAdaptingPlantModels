#version 330 core
out vec4 FragColor;

uniform vec4 uColor;

void main()
{
    //FragColor = vec4(0.15f, 0.65f, 0.15f, 1.0f);
    FragColor = uColor;
}
