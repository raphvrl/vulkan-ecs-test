#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec3 normal;
layout (location = 1) out vec2 uv;

layout (push_constant) uniform PushConstant {
    mat4 mvp;
} pc;

void main()
{
    gl_Position = pc.mvp * vec4(inPos, 1.0);
    
    normal = inNormal;
    uv = inUV;
}