#version 150
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;
uniform float alpha;
void main(){
    vec4 col = texture(skybox, TexCoords);
    col.w = alpha;
    color = col;
}