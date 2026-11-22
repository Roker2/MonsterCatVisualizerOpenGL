#version 330 core

in vec4 fragColorRGBA;
in vec2 fragTexCoord;
flat in float fragFadeFactor;
flat in float fragLengthToWidth;

out vec4 FragColor;

const float edgeFadeStart = 0.1;
const float edgeFadeEnd = 1.0;

const float tailFadePower = 1.35;
const float tailMinWidth = 0.15;
const float tailTaperEnd = 0.85;
const float tailAlphaScale = 0.65;
const float tailCutoffWidths = 0.5;
const float minTailHalfWidthPixels = 2.2;

const float headCenterX = 1.0;
const float headCenterY = 0.5;

const float headCoreRadius = 0.6;
const float headHaloRadius = 2.4;
const float headHaloStrength = 0.24;
const float headStrength = 0.9;

float calculateLengthFade(float texCoordX)
{
    float fade = smoothstep(0.0, 1.0, texCoordX);
    return pow(fade, tailFadePower);
}

float calculateWidthFade(vec2 texCoord)
{
    float clampedTexCoordX = clamp(texCoord.x, 0.0, 1.0);
    float signedDistance = texCoord.y * 2.0 - 1.0;

    float widthFactor = mix(
        tailMinWidth,
        1.0,
        smoothstep(0.0, tailTaperEnd, clampedTexCoordX));

    float pixelWidth = fwidth(signedDistance);
    float effectiveWidth = max(
        widthFactor,
        pixelWidth * minTailHalfWidthPixels);

    float normalizedDistance = abs(signedDistance) / effectiveWidth;

    float coverageScale = widthFactor / effectiveWidth;

    float fade = 1.0 - smoothstep(
        edgeFadeStart,
        edgeFadeEnd,
        normalizedDistance);

    return fade * fade * coverageScale;
}

float calculateHeadDistanceSquared(vec2 texCoord)
{
    vec2 offset = texCoord - vec2(headCenterX, headCenterY);
    offset.x *= fragLengthToWidth;

    return dot(offset, offset);
}

float calculateHeadCore(vec2 texCoord)
{
    float distanceSquared = calculateHeadDistanceSquared(texCoord);

    return 1.0 - smoothstep(
        0.0,
        headCoreRadius * headCoreRadius,
        distanceSquared);
}

float calculateHeadHalo(vec2 texCoord)
{
    float distanceSquared = calculateHeadDistanceSquared(texCoord);

    float halo = 1.0 - smoothstep(
        0.0,
        headHaloRadius * headHaloRadius,
        distanceSquared);

    return halo * halo * headHaloStrength;
}

float calculateTailEndFade(float texCoordX)
{
    return 1.0 - smoothstep(
        1.0,
        1.0 + tailCutoffWidths / fragLengthToWidth,
        texCoordX);
}

void main()
{
    float lengthFade = clamp(calculateLengthFade(fragTexCoord.x), 0.0, 1.0);
    float widthFade = calculateWidthFade(fragTexCoord);
    float tailEndFade = calculateTailEndFade(fragTexCoord.x);

    float tailAlpha = lengthFade * widthFade * tailEndFade * tailAlphaScale;

    float headCore = calculateHeadCore(fragTexCoord) * headStrength;
    float headHalo = calculateHeadHalo(fragTexCoord);

    float combinedAlpha = 1.0
        - (1.0 - tailAlpha)
        * (1.0 - headCore)
        * (1.0 - headHalo);

    FragColor = vec4(
        fragColorRGBA.rgb,
        fragColorRGBA.a * combinedAlpha * fragFadeFactor);
}
