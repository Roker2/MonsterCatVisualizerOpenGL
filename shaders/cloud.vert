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
layout(location = 9) in int aIsThemeColored;

out vec4 fragColorRGBA;
out vec2 fragTexCoord;
flat out float fragAge;
flat out float fragLifetime;
flat out float fragFadeIn;
flat out float fragFadeOut;
flat out int fragIsThemeColored;
flat out vec2 fragNoiseOffset;
flat out vec2 fragCloudVelocityUV;

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

vec2 convertNVStoNDC(vec2 position)
{
    return position * 2.0 - 1.0;
}

vec2 convertNVSToNSS(vec2 position)
{
    if (uViewportRatio >= 1.0)
        return vec2(position.x, position.y / uViewportRatio);

    return vec2(position.x * uViewportRatio, position.y);
}

vec2 convertNSSToNVS(vec2 position)
{
    if (uViewportRatio >= 1.0)
        return vec2(position.x, position.y * uViewportRatio);

    return vec2(position.x / uViewportRatio, position.y);
}

vec2 calculateDirectionNVS()
{
    vec2 directionNVS = convertNSSToNVS(aDirection);
    return normalize(directionNVS);
}

vec2 calculatePositionNVS(vec2 directionNVS)
{
    float offsetMultiplier = aSpeed * (uTime - aStartTime);
    vec2 offset = directionNVS * offsetMultiplier;

    return aStartPosition + offset;
}

vec2 calculateCornerOffsetNVS()
{
    vec2 corner = corners[gl_VertexID];
    return convertNSSToNVS(corner * aRadius);
}

vec2 calculateCloudVelocityUV(vec2 directionNVS)
{
    vec2 velocityNSS = convertNVSToNSS(directionNVS * aSpeed);

    // Convert velocity from NSS units to local UV units per second.
    return velocityNSS / (2.0 * aRadius);
}

void main()
{
    vec2 directionNVS = calculateDirectionNVS();
    vec2 positionNVS = calculatePositionNVS(directionNVS);
    vec2 cornerOffsetNVS = calculateCornerOffsetNVS();
    vec2 positionNDC = convertNVStoNDC(positionNVS + cornerOffsetNVS);

    gl_Position = vec4(positionNDC, 0.0, 1.0);

    fragColorRGBA = aColorRGBA;
    fragTexCoord = texCoords[gl_VertexID];

    fragAge = uTime - aStartTime;
    fragLifetime = aLifetime;
    fragFadeIn = aFadeIn;
    fragFadeOut = aFadeOut;

    fragIsThemeColored = aIsThemeColored;

    fragNoiseOffset = aStartPosition * 41.0 + aDirection * 13.0;
    fragCloudVelocityUV = calculateCloudVelocityUV(directionNVS);
}
