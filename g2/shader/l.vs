#version 400

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec3 v_pos;
out vec3 v_norm;
out vec2 f_texcoord;
out vec3 v_pos_or;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(position, 1.0f);
    v_norm = normalize(mat3(transpose(inverse(model))) * normal);
    f_texcoord = texCoords;

    vec4 viewModelPosition = view* model* vec4(position,1);
    v_pos = viewModelPosition.xyz/viewModelPosition.w;
    v_pos_or = position;
}