#version 330 core

out vec4 FragColor;

uniform sampler2D textureSampler; 
uniform bool useTexture;
uniform vec4 color;

in vec2 TexCoord; 

void main()
{
    if(useTexture)
    {
        vec4 texColor = texture(textureSampler,TexCoord);
        FragColor = vec4(texColor.rgb,texColor.a*color.a);

    }
    else
    {
        FragColor = vec4(color.rgb,color.a);
    }
}