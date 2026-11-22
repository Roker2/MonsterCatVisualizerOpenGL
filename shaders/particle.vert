#version 330 core

layout(location = 0) in vec2 aStartPosition;
layout(location = 1) in vec2 aDirection;
layout(location = 2) in float aSpeed;
layout(location = 3) in float aStartTime;
layout(location = 4) in float aLifetime;
layout(location = 5) in float aFadeIn;
layout(location = 6) in float aFadeOut;
layout(location = 7) in float aRadius;
layout(location = 8) in vec4 aColorRGBA;

out vec4 fragColorRGBA;
out vec2 fragLocalPosition;
flat out float fragAge;
flat out float fragLifetime;
flat out float fragFadeIn;
flat out float fragFadeOut;
flat out float fragRadius;

uniform float uTime;
uniform float uViewportRatio;
uniform float uMinRadius;

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
    return convertNSSToNVS(corner * max(aRadius, uMinRadius));
}

void main()
{
    vec2 cornerOffsetNVS = calculateCornerOffset();
    vec2 positionNVS = calculatePositionNVS();
    vec2 positionNDC = convertNVStoNDC(positionNVS + cornerOffsetNVS);
    gl_Position = vec4(positionNDC, 0.0, 1.0);

    fragColorRGBA = aColorRGBA;
    fragLocalPosition = corners[gl_VertexID];

    fragAge = uTime - aStartTime;
    fragLifetime = aLifetime;
    fragFadeIn = aFadeIn;
    fragFadeOut = aFadeOut;

    fragRadius = aRadius;
}
