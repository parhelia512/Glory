#define MAX_SIZE        4
#define MAX_KERNEL_SIZE ((MAX_SIZE * 2 + 1) * (MAX_SIZE * 2 + 1))
#define MAX_BINS_SIZE   100

const uint BlurType_Box = 0;
const uint BlurType_Median = 1;

#ifdef WITH_ONLY_RED
float BoxBlur(sampler2D inColor, int blurSize, float separation)
#else
vec4 BoxBlur(sampler2D inColor, int blurSize, float separation)
#endif
{
    vec2 texSize = textureSize(inColor, 0).xy;
	vec2 texCoord = gl_FragCoord.xy / texSize;

    if (blurSize <= 0)
#ifdef WITH_ONLY_RED
        return texture(inColor, texCoord).r;
#else
        return texture(inColor, texCoord);
#endif

    separation = max(separation, 1.0);

#ifdef WITH_ONLY_RED
    float color = 0.0;
#else
    vec3 color = vec3(0.0);
#endif
	for(int x = -blurSize; x <= blurSize; ++x)
	{
	    for(int y = -blurSize; y <= blurSize; ++y)
	    {
#ifdef WITH_ONLY_RED
	        color += texture(inColor, (gl_FragCoord.xy + vec2(x, y)*separation)/texSize).r;
#else
	        color += texture(inColor, (gl_FragCoord.xy + vec2(x, y)*separation)/texSize).xyz;
#endif
	    }
	}
	color /= pow(blurSize*2.0 + 1.0, 2.0);
#ifdef WITH_ONLY_RED
	return color;
#else
	return vec4(vec3(color), 1.0);
#endif
}

#ifdef WITH_ONLY_RED
float MedianBlur(sampler2D inColor, int blurSize, int binsSize)
#else
vec4 MedianBlur(sampler2D inColor, int blurSize, int binsSize)
#endif
{
    vec2 texSize = textureSize(inColor, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    if (blurSize <= 0)
#ifdef WITH_ONLY_RED
        return texture(inColor, texCoord).r;
#else
        return texture(inColor, texCoord);
#endif
	
    if (blurSize > MAX_SIZE)
        blurSize = MAX_SIZE;

    int kernelSize = int(pow(blurSize * 2 + 1, 2));
    binsSize = clamp(binsSize, 1, MAX_BINS_SIZE);

    int i = 0;
    int j = 0;
    int count = 0;
    int binIndex = 0;
    vec4 colors[MAX_KERNEL_SIZE];

    float bins[MAX_BINS_SIZE];
    int binIndexes[colors.length()];

    float total = 0;
    float limit = floor(float(kernelSize) / 2) + 1;

    float value = 0;
    vec3 valueRatios = vec3(0.3, 0.59, 0.11);

    for (i = -blurSize; i <= blurSize; ++i)
    {
        for (j = -blurSize; j <= blurSize; ++j)
        {
            colors[count] =
#ifdef WITH_ONLY_RED
                texture(inColor, (gl_FragCoord.xy + vec2(i, j))/texSize);
#else
                vec4(texture(inColor, (gl_FragCoord.xy + vec2(i, j))/texSize).r);
#endif
            count += 1;
        }
    }

    for (i = 0; i < binsSize; ++i)
        bins[i] = 0;

    for (i = 0; i < kernelSize; ++i)
    {
        value = dot(colors[i].rgb, valueRatios);
        binIndex = int(floor(value * binsSize));
        binIndex = clamp(binIndex, 0, binsSize - 1);
        bins[binIndex] += 1;
        binIndexes[i] = binIndex;
    }

    binIndex = 0;

    for (i = 0; i < binsSize; ++i)
    {
        total += bins[i];
        if (total >= limit)
        {
            binIndex = i;
            break;
        }
    }

    vec4 color = colors[0];

    for (i = 0; i < kernelSize; ++i)
    {
        if (binIndexes[i] == binIndex)
        {
            color = colors[i];
            break;
        }
    }
#ifdef WITH_ONLY_RED
    return color.r;
#else
    return color;
#endif
}