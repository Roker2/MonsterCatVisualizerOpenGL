#version 330 core

in vec4 fragColorRGBA;
in vec2 fragTexCoord;

out vec4 FragColor;

void main()
{
    float alphaGradient = fragTexCoord.x;
    FragColor = vec4(fragColorRGBA.xyz, fragColorRGBA.w * alphaGradient);
}
