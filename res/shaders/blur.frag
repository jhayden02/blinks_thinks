#version 100

precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main() {
    vec2 texel_size = vec2(1.0/900.0, 1.0/600.0);
    vec4 color = vec4(0.0);

    // Box blur sampling.
    for(int x = -2; x <= 2; x++) {
        for(int y = -2; y <= 2; y++) {
            vec2 offset = vec2(float(x), float(y)) * texel_size;
            color += texture2D(texture0, fragTexCoord + offset);
        }
    }

    gl_FragColor = (color / 25.0) * colDiffuse * fragColor;
}
