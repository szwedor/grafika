#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec3 v_pos;
out vec3 v_norm;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    v_norm = mat3(transpose(inverse(model))) * normal;
    v_pos = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * model * vec4(position, 1.0);
}  