#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
float weight[11] = float[] (0.058323059082146814
, 0.057868553057353486
, 0.056560992726918474
, 0.054458018111117105
, 0.05165067052852921
, 0.048256895571623555
, 0.044413231951294575
, 0.04026570117861841
, 0.035960708301434094
, 0.03163671243685924
, 0.027417290859961457);

uniform bool blurOn = true;

void main()
{
    if(blurOn)
    {
        float texelSize = (1.0 / textureSize(screenTexture, 0)).x;
        vec3 result = texture(screenTexture, TexCoords).rgb * weight[0]; // main texel

        for(int i = 1; i < 11; i++)
        {
            result += texture(screenTexture, TexCoords + vec2(texelSize*i, 0)).rgb * weight[i];
            result += texture(screenTexture, TexCoords - vec2(texelSize*i, 0)).rgb * weight[i];
        }
        
        FragColor = vec4(result, 1.0f);
    }
    else{
        FragColor = texture(screenTexture, TexCoords);
    }
}