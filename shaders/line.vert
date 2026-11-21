#version 330 core

layout(location = 0) in vec2 aStartPosition;
layout(location = 1) in vec2 aDirection;
layout(location = 2) in float aSpeed;
layout(location = 3) in float aStartTime;
layout(location = 4) in float aLength;
layout(location = 5) in float aLineWidth;
layout(location = 6) in vec4 aColorRGBA;

out vec4 fragColorRGBA;
out vec2 fragTexCoord;

uniform float uTime;
uniform float uViewportRatio;

const vec2 corners[4] = vec2[4](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0)
);

const vec2 texCoords[4] = vec2[4](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0)
);

mat2 rotate2d()
{
    return mat2(
        aDirection.x, aDirection.y,
       -aDirection.y, aDirection.x);
}

vec2 convertNVStoNDC(vec2 position)
{
    return position * 2.0 - 1.0;
}

vec2 convertNSSToNVS(vec2 position)
{
    if (uViewportRatio >= 1.0f)
        return vec2(position.x, position.y * uViewportRatio);
    return vec2(position.x / uViewportRatio, position.y);
}

vec2 calculatePositionNVS()
{
    vec2 directionNVS = convertNSSToNVS(aDirection);
    directionNVS = normalize(directionNVS);

    float offsetMultiplier = aSpeed * (uTime - aStartTime);
    vec2 offset = directionNVS * offsetMultiplier;

    return aStartPosition + offset;
}

vec2 calculateCornerOffset()
{
    vec2 corner = corners[gl_VertexID];
    vec2 lineCorner = corner * vec2(aLength, aLineWidth);
    return convertNSSToNVS(rotate2d() * lineCorner);
}

void main()
{
    vec2 cornerOffsetNVS = calculateCornerOffset();
    vec2 positionNVS = calculatePositionNVS();
    vec2 positionNDC = convertNVStoNDC(positionNVS + cornerOffsetNVS);
    gl_Position = vec4(positionNDC, 0.0, 1.0);
    fragColorRGBA = aColorRGBA;
    fragTexCoord = texCoords[gl_VertexID];
}
