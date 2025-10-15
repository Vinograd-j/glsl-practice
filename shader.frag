#version 330 core

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

out vec4 fragColor;

float hash(vec2 p)
{
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

float particle(vec2 uv, vec2 center, float size)
{
    float d = length(uv - center);
    return exp(-pow(d / size, 6.0));
}

void main()
{
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= u_resolution.x / u_resolution.y;

    float t = u_time * 0.8;
    float brightness = 0.0;

    for (int i = 0; i < 200; ++i)
    {
        float fi = float(i);
        float a = hash(vec2(fi, fi * 0.123)) * 6.2831; // угол
        float r = hash(vec2(fi * 0.7, fi * 0.91));     // радиус
        float speed = 0.2 + 0.8 * hash(vec2(fi * 0.91, fi * 1.3));
        float phase = hash(vec2(fi * 2.7, fi * 4.1)) * 6.2831;

        vec2 pos = vec2(cos(a), sin(a)) * (r + 0.5 * fract(t * speed + phase));

        float size = 0.0015 + 0.02 * hash(vec2(fi * 0.1, fi * 0.8));

        brightness += particle(uv, pos, size);
    }

    vec3 col = vec3(0.3, 0.6, 1.0) * brightness;
    fragColor = vec4(col, 1.0);
}
