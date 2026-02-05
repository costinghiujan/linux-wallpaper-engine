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

// Helper to rotate the pattern
mat2 rotate(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}

void main() {
    vec2 uv = qt_TexCoord0;
    
    // Fix aspect ratio
    float aspect = resolution.x / resolution.y;
    uv.x *= aspect;
    
    // Zoom out slightly
    uv *= 3.0;

    // --- Mouse Interaction ---
    // Distort the water towards the mouse
    vec2 m = mouse;
    m.x *= aspect;
    vec2 offset = (uv - m * 3.0) * 0.05; 

    // --- Caustic Effect Generation ---
    vec3 color = vec3(0.0);
    
    // We stack 3 layers of waves
    for (float i = 0.0; i < 3.0; i++) {
        uv = rotate(time * 0.1 + i) * uv + offset;
        
        // This math creates the "electric" grid lines
        float wave = sin(uv.x * 5.0 + time) + sin(uv.y * 5.0 + time);
        
        // Sharpen the lines
        wave = smoothstep(0.8, 1.0, abs(wave));
        
        // Add Cyan/Blue light
        color += vec3(0.0, 0.7, 1.0) * wave * 0.5;
    }

    // --- Background ---
    vec3 bg = vec3(0.0, 0.1, 0.5); // Deep Blue
    
    fragColor = vec4((bg + color) * qt_Opacity, 1.0);
}