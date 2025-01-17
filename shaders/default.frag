#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 uv;
void main()
{
    outColor = vec4(0.5, 0.5, 0.5, 1.0);
}