#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 uv;

layout(set = 0, binding = 0) uniform sampler2D tex;

void main()
{
    outColor = texture(tex, uv);
}