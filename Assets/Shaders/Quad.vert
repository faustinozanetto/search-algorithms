#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 aInstanceMatrix;
layout (location = 5) in vec3 aColor;

uniform mat4 mvp;

out vec3 color;

void main() {
    gl_Position = mvp * aInstanceMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aColor;
}
