#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
const int radius = 6;
float weight[radius] = float[] (0.19859610213125314
  ,0.17571363439579307
  ,0.12170274650962626
  ,0.06598396774984912
  ,0.028001560233780885
  , 0.009300040045324049);

uniform bool blurOn = true;

void main()
{
    if(true)
    {
        float texelSize = (1.0 / textureSize(screenTexture, 0)).x;
        vec3 result = texture(screenTexture, TexCoords).rgb * weight[0]; // main texel

        for(int i = 1; i < radius; i++)
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