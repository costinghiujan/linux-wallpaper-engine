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
    // 1. Setup Coordinates
    vec2 uv = qt_TexCoord0;
    
    // Fix aspect ratio so waves aren't stretched
    float aspect = resolution.x / resolution.y;
    vec2 p = uv * 6.0; // Zoom out a bit (higher number = see more waves)
    p.x *= aspect;

    // 2. Mouse Interaction (Soft & Subtle)
    // We gently push the water pattern based on mouse position
    vec2 m = mouse;
    m.x *= aspect;
    p += (m - p) * 0.05; // Very subtle warping towards mouse

    // 3. Iterative Wave Generation (The "Pleasant" part)
    // Instead of sharp lines, we loop and add sine waves on top of each other
    // This creates that "oil on water" or "swimming pool" look.
    float brightness = 1.0;
    float speed = time * 0.5; // Slow, relaxing speed

    for(int i = 1; i <= 4; i++) {
        // This magic math distorts the coordinates in a loop
        // It creates complex, organic shapes from simple sine waves
        float n = float(i);
        p.x += 0.6 / n * sin(n * p.y + speed + 0.3 * n);
        p.y += 0.6 / n * cos(n * p.x + speed + 0.3 * n + 1.5);
    }

    // 4. Color Mapping
    // We use the distorted coordinates to pick colors
    // r = wave height, g = wave depth, b = constant blue base
    
    // Base Deep Blue
    vec3 deepWater = vec3(0.05, 0.2, 0.5);
    
    // Tropical Teal/Cyan Highlight
    vec3 tropical = vec3(0.0, 0.6, 0.8);
    
    // Calculate the wave intensity (0.0 to 1.0)
    float waveIntensity = sin(p.x + p.y) * 0.5 + 0.5;

    // Mix the colors smoothly
    vec3 finalColor = mix(deepWater, tropical, waveIntensity);
    
    // Add a little extra brightness to the peaks (sunlight sparkles)
    finalColor += vec3(0.1, 0.1, 0.2) * pow(waveIntensity, 4.0);

    // 5. Output
    fragColor = vec4(finalColor * qt_Opacity, 1.0);
}