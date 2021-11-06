#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloom;
uniform float exposure;
vec3 ACESFilm(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0f, 1.0f);
}

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(bloom, TexCoords).rgb;

    // тональная компрессия с экспозицией
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // vec3 mapped = ACESFilm(hdrColor * exposure);

    // гамма-коррекция
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}