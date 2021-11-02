#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 texel = texture(screenTexture, TexCoords).rgb;
    float brightness = (texel.r + texel.g + texel.b) / 3.0f;
    if(brightness > 1.0f)
    {
        FragColor = vec4(texel, 1.0f);
    }
    else
    {
        FragColor = vec4(0.0f);
    }
}