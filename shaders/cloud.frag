#version 330 core

in vec4 fragColorRGBA;
flat in int fragIsThemeColored;

out vec4 FragColor;

uniform vec3 uThemeColor;

vec4 calculateColor()
{
    if (fragIsThemeColored == 1)
        return vec4(uThemeColor, fragColorRGBA.w);
    return fragColorRGBA;
}

void main()
{
    FragColor = calculateColor();
}
