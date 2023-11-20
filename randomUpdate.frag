#version 330 core

out vec4 FragColor;
uniform sampler2D texture;
uniform float time;

// Simplex noise function for better randomness
float snoise(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {

    float size = 1024.0;
    vec2 uv = gl_FragCoord.xy / size;
    vec3 color = texture(texture, uv).rgb;
    // Use simplex noise for better randomness
    float r = snoise(uv * 10.0 + time);
    float g = snoise(uv * 20.0 - time);
    float b = snoise(uv * 30.0 + time);
    // Set alpha channel to a random value
    float alpha = snoise(uv * 50.0 - time);

     // Use the noise values to create a random color
    if(alpha >= 0.999)
    {
        color += (vec3(r, g, b)-0.5);
    }
     color = clamp(color, 0.0, 1.0);
    // Output the color with alpha
    FragColor = vec4(color, 1);
}
