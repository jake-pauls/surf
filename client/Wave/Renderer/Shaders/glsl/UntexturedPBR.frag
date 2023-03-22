#version 450

layout(location = 0) in vec3 in_FragColor;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_WorldPosition;
layout(location = 3) in vec3 in_Normal;

layout(location = 0) out vec4 o_Color;

const float PI = 3.14159265359;

layout(binding = 0) uniform UniformBufferObject
{
    // PBR
    vec4 m_LightPosition;
    vec4 m_LightColor;
    vec4 m_Albedo;
    vec4 m_PBRSettings; 
    // View
    mat4 m_ViewMatrix;
    mat4 m_ProjectionMatrix;
} u_UBO;

layout(push_constant) uniform PushConstants 
{
    vec4 m_CameraPosition;
    mat4 m_ModelMatrix;
} u_PCS;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

void main()
{
    // Unpack uniform values
    vec3 lightPosition = u_UBO.m_LightPosition.xyz;
    vec3 lightColor = u_UBO.m_LightColor.xyz;
    vec3 albedo = u_UBO.m_Albedo.xyz;
    const float metallic = u_UBO.m_PBRSettings.x;
    const float roughness = u_UBO.m_PBRSettings.y;
    const float ao = u_UBO.m_PBRSettings.z;

    vec3 N = normalize(in_Normal);
    vec3 V = normalize(u_PCS.m_CameraPosition.xyz - in_WorldPosition);

    // Reflectance at normal incidence
    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metallic);

    // Reflectance equation
    vec3 Lo = vec3(0.0f);

    // Per-light radiance
    vec3 L = normalize(lightPosition - in_WorldPosition);
    vec3 H = normalize(V + L);
    float distance = length(lightPosition - in_WorldPosition);
    float attenuation = 1.0f / (distance * distance);
    vec3 radiance = lightColor * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0f, 1.0f), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001;
    vec3 specular = numerator / denominator;

    // kS = Fresnel
    vec3 kS = F;
    vec3 kD = vec3(1.0f) - kS;
    kD *= 1.0f - metallic;

    // Scale light
    float NdotL = max(dot(N, L), 0.0f);

    // Add outgoing radiance
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    // Ambient lighting
    vec3 ambient = vec3(0.03f) * albedo * ao;

    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f / 2.2f));

    o_Color = vec4(color, 1.0f);
}