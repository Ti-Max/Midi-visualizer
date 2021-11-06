#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float threshold;
void main()
{

    vec3 texel = texture(screenTexture, TexCoords).rgb;
    float brightness = texel.r * 0.2126 + texel.g * 0.7152 + texel.b * 0.0722;
    if(brightness > threshold)
    {
        FragColor = vec4(texel, 1.0f);
    }
    else
    {
        FragColor = vec4(0.0f);
    }
}