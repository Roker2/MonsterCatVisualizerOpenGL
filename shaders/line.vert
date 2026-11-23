#version 330 core

layout(location = 0) in vec2 aStartPosition;
layout(location = 1) in vec2 aDirection;
layout(location = 2) in float aSpeed;
layout(location = 3) in float aStartTime;
layout(location = 4) in float aLifetime;
layout(location = 5) in float aFadeIn;
layout(location = 6) in float aFadeOut;
layout(location = 7) in float aLength;
layout(location = 8) in float aLineWidth;
layout(location = 9) in vec4 aColorRGBA;

out vec4 fragColorRGBA;
out vec2 fragTexCoord;
flat out float fragFadeFactor;
flat out float fragLengthToWidth;

uniform float uTime;
uniform float uViewportRatio;

const float headMarginWidths = 2.5;
const float halfQuadWidthWidths = 2.5;

const vec2 corners[4] = vec2[4](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0)
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

vec2 calculatePositionNVS(float age)
{
    vec2 directionNVS = convertNSSToNVS(aDirection);
    directionNVS = normalize(directionNVS);

    float offsetMultiplier = aSpeed * age;
    vec2 offset = directionNVS * offsetMultiplier;

    return aStartPosition + offset;
}

vec2 calculateLineCorner()
{
    vec2 corner = corners[gl_VertexID];

    float x = mix(
        -aLength * 0.5,
        aLength * 0.5 + aLineWidth * headMarginWidths,
        (corner.x + 1.0) * 0.5);

    float y = corner.y * aLineWidth * halfQuadWidthWidths;

    return vec2(x, y);
}

vec2 calculateFragTexCoord(vec2 lineCorner)
{
    return vec2(
        lineCorner.x / aLength + 0.5,
        lineCorner.y / aLineWidth + 0.5);
}

float calculateFadeFactor(float age)
{
    float fadeInFactor = smoothstep(
        0.0,
        aFadeIn,
        age);

    float fadeOutFactor = 1.0 - smoothstep(
        aLifetime - aFadeOut,
        aLifetime,
        age);

    return fadeInFactor * fadeOutFactor;
}

void main()
{
    float age = uTime - aStartTime;
    vec2 lineCorner = calculateLineCorner();
    vec2 cornerOffsetNVS = convertNSSToNVS(rotate2d() * lineCorner);
    vec2 positionNVS = calculatePositionNVS(age);
    vec2 positionNDC = convertNVStoNDC(positionNVS + cornerOffsetNVS);

    gl_Position = vec4(positionNDC, 0.0, 1.0);

    fragColorRGBA = aColorRGBA;
    fragTexCoord = calculateFragTexCoord(lineCorner);

    fragFadeFactor = calculateFadeFactor(age);

    fragLengthToWidth = aLength / aLineWidth;
}
