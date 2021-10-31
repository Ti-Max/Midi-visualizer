#version 460 core
out vec4 FragColor;

uniform vec4 color;

void main()
{    
    float brightness =  color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
    if(brightness > 0.7)
    {
        FragColor = color;
    }
    else{

    FragColor = vec4(0.0);
    }
}