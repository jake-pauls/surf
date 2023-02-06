#version 450

layout(location = 0) out vec3 o_FragColor;

vec2 g_Positions[3] = vec2[](
    vec2( 0.0, -0.5),
    vec2( 0.5,  0.5),
    vec2(-0.5,  0.5)
);

vec3 g_Colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main()
{
    gl_Position = vec4(g_Positions[gl_VertexIndex], 0.0, 1.0);

    o_FragColor = g_Colors[gl_VertexIndex];
}