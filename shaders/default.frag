#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 uv;

layout(set = 0, binding = 0) uniform sampler2D tex;

vec3 gammaCorrection(vec3 color, float gamma)
{
    return pow(color, vec3(1.0 / gamma));
}

void main()
{
    vec4 texColor = texture(tex, uv);

    vec3 color = gammaCorrection(texColor.rgb, 2.2);

    outColor = vec4(color, texColor.a);
}