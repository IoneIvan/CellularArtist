#version 330 core
out vec4 FragColor;

uniform sampler2D colorTexture; // The previous texture
uniform sampler2D geneTexture; // The previous texture

void main()
{
    float size = 1024;
    vec2 step = vec2(1.0, 1.0) / size;
    vec2 uv = gl_FragCoord.xy / size;

    vec3 color = texture(colorTexture, uv).rgb;

    // Iterate over the 4 diagonal neighbors
    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            vec2 colorNeighborCoord = uv + vec2(i, j) * step;
            vec3 colorNeighbor = texture(colorTexture, colorNeighborCoord).rgb;
            if (colorNeighborCoord.x >= 0.0 || colorNeighborCoord.x <= 1.0 || colorNeighborCoord.y >= 0.0 || colorNeighborCoord.y <= 1.0)
            {
                colorNeighbor = vec3(1.0,1.0,1.0);
            }
            // Iterate over the 9 neighbors for brain calculation
            for (int x = -1; x < 2; ++x)
            {
                for (int y = -1; y < 2; ++y)
                {
                    vec2 brainNeighborCoord = uv + vec2(x, y) * step;
                    vec3 brainNeighbor = texture(geneTexture, brainNeighborCoord).rgb;

                    float red   = (brainNeighbor.r - 0.5) * 1.5  * colorNeighbor.r;
                    float green = (brainNeighbor.g - 0.5) * 1.5  * colorNeighbor.g;
                    float blue  = (brainNeighbor.b - 0.5) * 1.5  * colorNeighbor.b;

                    color.r += red;
                    color.g += green;
                    color.b += blue;
                }
            }
        }
    }

    // Clamp color values to the range [0, 1]
    color /= 32;
    color = clamp(color, 0.0, 1.0);

    FragColor = vec4(color, 1.0);
}