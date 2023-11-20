#version 330 core
out vec4 FragColor;

uniform sampler2D colorTexture;  // The previous texture
uniform sampler2D geneTexture;     // The previous texture
uniform sampler2D sampleTexture;  // The previous texture

void main()
{
    float size = 1024.0;
    vec2 step = vec2(1.0, 1.0) / size;
    vec2 uv = gl_FragCoord.xy / size;

    // Initialize color with colorTexture value
    vec3 color = texture(colorTexture, uv).rgb;

    // Brain calculation
    float minErr = 1000000.0;
    vec2 minIndex = uv;
    
    for (int x = -1; x < 2; ++x)
    {
        for (int y = -1; y < 2; ++y)
        {
            vec3 colorNeighbor = texture(colorTexture, uv + vec2(x, y) * step).rgb;
            vec3 originalNeighbor = texture(sampleTexture, uv + vec2(x, y) * step).rgb;

            vec3 err = abs(colorNeighbor - originalNeighbor);
            float totalErr = err.r + err.g + err.b;

            if (totalErr < minErr)
            {
                minIndex = uv + vec2(x, y) * step;
                minErr = totalErr;
            }
        }
    }

    // Use vec2 for texture lookup
    color = texture(geneTexture, minIndex).rgb;
    FragColor = vec4(color, 1.0);
}
