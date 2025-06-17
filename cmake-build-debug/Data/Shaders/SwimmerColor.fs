#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main(){
    vec4 texelColor = texture(texture0, fragTexCoord) * colDiffuse * fragColor;

    // Keep only red channel, zero out green and blue
    float red = texelColor.r;

    finalColor = vec4(red, 0.0, 0.0, texelColor.a);
}
