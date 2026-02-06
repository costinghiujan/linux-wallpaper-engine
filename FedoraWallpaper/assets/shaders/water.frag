#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float time;
    vec2 resolution;
    vec2 mouse;
};

void main() {
    vec2 uv = qt_TexCoord0;
    
    // Aspect Ratio Fix
    float aspect = resolution.x / resolution.y;
    vec2 p = uv * 6.0; 
    p.x *= aspect;

    // Mouse Interaction
    vec2 m = mouse;
    m.x *= aspect;
    p += (m - p) * 0.03; 

    float speed = time * 0.3;

    // Efficient 3-iteration loop
    for(int i = 1; i <= 3; i++) {
        float n = float(i);
        vec2 d = vec2(sin(n * p.y + speed + 0.3 * n), cos(n * p.x + speed + 0.3 * n + 1.5));
        p += 0.6 / n * d;
    }

    // Calculate Wave Height
    float wave = sin(p.x + p.y);
    float intensity = wave * 0.5 + 0.5;

    // --- COLOR PALETTE ---
    vec3 blackBase = vec3(0.0, 0.01, 0.03); 
    vec3 darkBlue = vec3(0.05, 0.15, 0.4);
    
    vec3 color = mix(blackBase, darkBlue, intensity * 0.8);
    
    // Highlights (Cyan)
    float sparkle = pow(intensity, 12.0);
    color += vec3(0.02, 0.3, 0.4) * sparkle;

    fragColor = vec4(color * qt_Opacity, 1.0);
}