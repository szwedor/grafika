#version 400

in vec3 v_pos;
in vec3 v_norm;
in vec2 f_texcoord;
in vec3 v_pos_or;

out vec4 out_Color;
struct Light {
 vec3 position;
 vec3 color;
 vec3 direction;
 float ambientIntensity;
 float diffuseIntensity;
 float coneAngle;
 float type;
 
};
struct Material{
  vec4 color;
  sampler2D texture;
  bool hasTexture;
};
/* Light type
    Point = 0,
    Spot = 1,
    Directional = 2
*/


uniform vec3 viewPos;
uniform samplerCube skybox;
uniform vec3 cameraPos;

uniform Light lights[50];
uniform Material ambient;
uniform Material diffuse;
uniform Material specular;
uniform float material_specExponent;
uniform float alpha;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
  out_Color = vec4(0,0,0,1);
  vec2 flipped_texcoord = vec2(f_texcoord.x, 1.0 - f_texcoord.y);

  vec4 ambientColor = ambient.color;
  if(ambient.hasTexture)
    ambientColor =  texture2D(ambient.texture, f_texcoord);
  vec4 diffuseColor = diffuse.color;
  if(diffuse.hasTexture)
   diffuseColor =  texture2D(diffuse.texture, f_texcoord); 
  vec4 specularColor = specular.color;
  if(specular.hasTexture)
 specularColor =  texture2D(specular.texture, flipped_texcoord);

 for(int i = 0; i < 3; i++){
  
  if(lights[i].color == vec3(0,0,0))
  {
   continue;
  }
  vec4 lightcolor = vec4(0,0,0,1);


  vec4 light_ambient = lights[i].ambientIntensity * vec4(lights[i].color, 0.0);
  vec4 light_diffuse = lights[i].diffuseIntensity * vec4(lights[i].color, 0.0);



 vec3 normal =v_norm;
    vec3 surfacePos = vec3(model * vec4(v_pos_or, 1));
   
 //   vec3 surfacePos2 = vec3(ViewMatrix*ModelMatrix * vec4(v_pos_or, 1));
 vec3 surfaceToCamera = normalize(cameraPos - surfacePos);
 vec3 surfaceToLight=normalize(lights[i].position - surfacePos);

 if(lights[i].type == 2){
   surfaceToLight = normalize(lights[i].direction);
  }

   vec4 ambient = light_ambient * ambientColor;


  bool inCone = false;
  if( degrees(acos(dot(-surfaceToLight, normalize(lights[i].direction)))) < lights[i].coneAngle)
  {
   inCone = true;

   }

    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec4 diffuse = diffuseCoefficient * diffuseColor*light_diffuse;

    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material_specExponent);
    vec4 specular = specularCoefficient * specularColor;

     if(lights[i].type != 1 || inCone){
 lightcolor= (diffuse + specular);
     }
 out_Color = out_Color + lightcolor +ambient;
 }
 //out_Color = ambientColor;
 out_Color =vec4(vec3(out_Color),alpha);

}