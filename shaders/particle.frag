#version 330 core

in vec4 fragColorRGBA;
in vec2 fragLocalPosition;
flat in float fragFadeFactor;
flat in float fragRadius;

out vec4 FragColor;

const float coreStartSquared = 0.005;
const float coreEndSquared = 0.12;

const float haloStartSquared = 0.0;
const float haloEndSquared = 0.95;
const float haloStrength = 0.2;

const float rayWidth = 0.16;
const float rayLengthStart = 0.14;
const float rayLengthEnd = 0.78;
const float rayStrength = 0.22;

// Radius thresholds for enabling additional components.
const float haloEnableRadiusStart = 0.001;
const float haloEnableRadiusEnd = 0.0015;

const float raysEnableRadiusStart = 0.002;
const float raysEnableRadiusEnd = 0.0025;

float calculateCore(float distanceSquared)
{
    return 1.0 - smoothstep(
        coreStartSquared,
        coreEndSquared,
        distanceSquared);
}

float calculateHalo(float distanceSquared)
{
    float halo = 1.0 - smoothstep(
        haloStartSquared,
        haloEndSquared,
        distanceSquared);

    return halo * haloStrength * halo;
}

float calculateRays(vec2 localPosition)
{
    vec2 absPosition = abs(localPosition);

    float horizontalWidth = 1.0 - smoothstep(
        0.0,
        rayWidth,
        absPosition.y);

    float horizontalLength = 1.0 - smoothstep(
        rayLengthStart,
        rayLengthEnd,
        absPosition.x);

    float horizontalRay = horizontalWidth * horizontalLength;

    float verticalWidth = 1.0 - smoothstep(
        0.0,
        rayWidth,
        absPosition.x);

    float verticalLength = 1.0 - smoothstep(
        rayLengthStart,
        rayLengthEnd,
        absPosition.y);

    float verticalRay = verticalWidth * verticalLength;

    float rays = max(horizontalRay, verticalRay);
    return rays * rayStrength;
}

void main()
{
    float distanceSquared = dot(fragLocalPosition, fragLocalPosition);

    float core = calculateCore(distanceSquared);
    float halo = calculateHalo(distanceSquared);
    float rays = calculateRays(fragLocalPosition);

    // Smooth activation based on star radius.
    float haloFactor = smoothstep(
        haloEnableRadiusStart,
        haloEnableRadiusEnd,
        fragRadius);

    float raysFactor = smoothstep(
        raysEnableRadiusStart,
        raysEnableRadiusEnd,
        fragRadius);

    halo *= haloFactor;
    rays *= raysFactor;

    float intensity = 1.0
        - (1.0 - core)
        * (1.0 - halo)
        * (1.0 - rays);

    FragColor = vec4(
        fragColorRGBA.rgb,
        fragColorRGBA.a * intensity * fragFadeFactor);
}
