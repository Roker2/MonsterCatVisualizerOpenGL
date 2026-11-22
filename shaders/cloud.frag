#version 330 core

in vec4 fragColorRGBA;
in vec2 fragTexCoord;
flat in int fragIsThemeColored;
flat in vec2 fragNoiseOffset;
flat in vec2 fragCloudVelocityUV;
flat in float fragAge;

out vec4 FragColor;

uniform vec3 uThemeColor;

const float noiseScale = 5.0;
const float backflowMultiplier = 1.2;
const float edgeInnerRadius = 0.18;
const float edgeOuterRadius = 0.5;
const float densityThresholdMin = 0.25;
const float densityThresholdMax = 0.70;

const float ditherFullAlpha = 0.05;
const float ditherMaxAlpha = 0.15;
const float alphaDitherStrength = 1.0 / 255.0;

vec4 calculateBaseColor()
{
    if (fragIsThemeColored == 1)
        return vec4(uThemeColor, fragColorRGBA.w);

    return fragColorRGBA;
}

float calculateEdgeMask()
{
    vec2 centeredUV = fragTexCoord - vec2(0.5);
    float distanceToCenter = length(centeredUV);

    return 1.0 - smoothstep(
        edgeInnerRadius,
        edgeOuterRadius,
        distanceToCenter);
}

float hash21(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float valueNoise(vec2 p)
{
    vec2 cell = floor(p);
    vec2 local = fract(p);

    vec2 weight = local * local * (3.0 - 2.0 * local);

    float a = hash21(cell);
    float b = hash21(cell + vec2(1.0, 0.0));
    float c = hash21(cell + vec2(0.0, 1.0));
    float d = hash21(cell + vec2(1.0, 1.0));

    return mix(
        mix(a, b, weight.x),
        mix(c, d, weight.x),
        weight.y);
}

float fbm(vec2 p)
{
    float total = 0.0;
    float amplitude = 0.5;
    float normalization = 0.0;
    float frequency = 1.0;

    for (int octave = 0; octave < 4; ++octave)
    {
        total += amplitude * valueNoise(p * frequency);
        normalization += amplitude;

        frequency *= 2.0;
        amplitude *= 0.5;
    }

    return total / normalization;
}

float calculateSmokeDensity()
{
    vec2 animatedUV = fragTexCoord +
        fragCloudVelocityUV * fragAge * backflowMultiplier;

    vec2 noiseCoord = animatedUV * noiseScale + fragNoiseOffset;
    float noise = fbm(noiseCoord);

    return smoothstep(
        densityThresholdMin,
        densityThresholdMax,
        noise);
}

float calculateDither(vec2 pixelPosition)
{
    return fract(
        52.9829189 * fract(
            dot(pixelPosition, vec2(0.06711056, 0.00583715))));
}

float applyDithering(float alpha)
{
    if (alpha <= 0.0 || alpha >= ditherMaxAlpha)
        return alpha;

    float ditherFactor = 1.0 - smoothstep(
        ditherFullAlpha,
        ditherMaxAlpha,
        alpha);

    float dither = calculateDither(gl_FragCoord.xy) - 0.5;

    return clamp(alpha + dither * alphaDitherStrength * ditherFactor, 0.0, 1.0);
}

void main()
{
    vec4 color = calculateBaseColor();

    float edgeMask = calculateEdgeMask();
    float smokeDensity = calculateSmokeDensity();
    float mask = edgeMask * smokeDensity;

    float alpha = applyDithering(color.a * mask);

    FragColor = vec4(color.rgb, alpha);
}
