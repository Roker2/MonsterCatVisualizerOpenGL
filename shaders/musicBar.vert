#version 330 core

layout(location = 0) in float aHeight;

uniform int uCount;
uniform vec2 uPanelCenterOffset;
uniform vec2 uPanelSize;
uniform float uElementWidthScale;

// LS - music bar panel local space [0, 1]

const vec2 corners[4] = vec2[4](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0)
);

vec2 convertNVStoNDC(vec2 position)
{
    return position * 2.0 - 1.0;
}

vec2 convertLStoNVS(vec2 position)
{
    return (position - vec2(0.5)) * uPanelSize + vec2(0.5) + uPanelCenterOffset;
}

vec2 calculatePositionLS()
{
    float delta = 1.0f / uCount;
    float relativeIndex = (gl_InstanceID + 0.5) * delta;
    return vec2(relativeIndex, aHeight * 0.5);
}

vec2 calculateCornerOffsetLS()
{
    vec2 barSizeLS = vec2(
        1.0 / uCount * uElementWidthScale,
        aHeight);

    vec2 corner = corners[gl_VertexID];
    return corner * barSizeLS * 0.5;
}

void main()
{
    vec2 cornerOffsetLS = calculateCornerOffsetLS();
    vec2 positionLS = calculatePositionLS();
    vec2 positionNDC = convertNVStoNDC(convertLStoNVS(positionLS + cornerOffsetLS));
    gl_Position = vec4(positionNDC, 0.0, 1.0);
}
