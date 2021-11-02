#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D colorTexture;
uniform sampler2D HDRcolorTexture;

void main()
{
    vec3 color = texture(colorTexture, TexCoords).rgb;
    vec3 HDRcolor = texture(HDRcolorTexture, TexCoords).rgb;
    FragColor = vec4(color + HDRcolor, 1.0f);
}