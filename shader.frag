#version 330 core
out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;
uniform vec3 u_camera;

vec2 sphere(in vec3 origin, in vec3 rd, float ra)
{
    float b = dot(origin, rd);
    float c = dot(origin, origin) - ra * ra;
    float h = b * b - c;

    if (h < 0.0) return vec2(-1.0);
    h = sqrt(h);
    return vec2(-b - h, -b + h);
}

float plaIntersect(in vec3 ro, in vec3 rd, in vec4 p)
{
    return -(dot(ro, p.xyz) + p.w) / dot(rd, p.xyz);
}

vec3 castRay(vec3 ro, vec3 rd)
{
    vec2 minIt = vec2(99999.0);
    vec2 it;
    vec3 n;
    vec3 spherePos = vec3(0.0, -1.0, 0.0);
    it = sphere(ro - spherePos, rd, 1.0);

    if (it.x > 0.0 && it.x < minIt.x){
        minIt = it;
        vec3 itPos = ro+ rd * it.x;
        n = itPos - spherePos;
    }

    vec3 planeNormal = vec3(0.0, 0.0, 1.0);
    vec4 plane = vec4(planeNormal, 1.0); // y = -1

    it = vec2(plaIntersect(ro, rd, plane));

    if (it.x > 0.0 && it.x < minIt.x){
        minIt = it;
        n = planeNormal;
    }

    if(minIt.x == 99999.0) return vec3(0.0);

    vec3 light = normalize(vec3(-0.5, 0.75, -1.0));

    float diffuse = max(0.0, dot(light, n)) * 0.5;
    vec3 reflected = reflect(-rd, normalize(n));

    float specular = max(0.0, dot(reflected, light)) * 0.5;
    return vec3(diffuse) + vec3(specular);
}

mat2 rot(float a)
{
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution) / u_resolution.y;
    vec3 rayOrigin = u_camera;
    vec3 rayDirection = normalize(vec3(1.0, uv));
    rayDirection.zx *= rot(-u_mouse.y);
    rayDirection.xy *= rot(-u_mouse.x);
    vec3 col = castRay(rayOrigin, rayDirection);
    FragColor = vec4(col, 1.0);
}
