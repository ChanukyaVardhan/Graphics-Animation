#version 130

in vec3 fNormal;
in vec4 fPosition;
in vec4 glpos;

out vec4 frag_color;

uniform mat3 normalMatrix;
uniform mat4 viewMatrix;

uniform int light1;
uniform int light2;
uniform int spotlight1;
uniform sampler2D texture;
varying vec2 tex;

void main () 
{
  vec4 light_component;

  vec4 texture_tex = texture2D(texture, tex);

  // Defining Materials
  vec4 diffuse = vec4(0.7, 0.7, 0.7, 1.0);
  vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
  vec4 specular = vec4(0.5, 0.5, 0.5, 1.0);
  float shininess = 0.9;
  vec4 spec1 = vec4(0.0);
  
  // Defining Light 1
  vec4 lightPos1 = vec4(0.0, 7.0, 100.0, 0.0); 
  vec3 lightDir1 = vec3(viewMatrix * lightPos1);
  lightDir1 = normalize(lightDir1);

  vec3 n = normalize(normalMatrix * normalize(fNormal));
  float dotProduct1 = dot(n, lightDir1);
  float intensity1 =  max( dotProduct1, 0.0);

  // Compute specular component only if light falls on vertex
  if(intensity1 > 0.0)
  {
	vec3 eye1 = normalize( vec3(-glpos));
	vec3 h1 = normalize(lightDir1 + eye1 );
   	float intSpec1 = max(dot(h1,n), 0.0);
    spec1 = specular * pow(intSpec1, shininess);
  }

  vec4 spec2 = vec4(0.0);
  
  vec4 lightPos2 = vec4(7.0, 7.0, -50.0, 0.0); 
  vec3 lightDir2 = vec3(viewMatrix * lightPos2);
  lightDir2 = normalize(lightDir2);

  n = normalize(normalMatrix * normalize(fNormal));
  float dotProduct2 = dot(n, lightDir2);
  float intensity2 =  max( dotProduct2, 0.0);

  // Compute specular component only if light falls on vertex
  if(intensity2 > 0.0)
  {
	vec3 eye2 = normalize( vec3(-glpos));
	vec3 h2 = normalize(lightDir2 + eye2 );
   	float intSpec2 = max(dot(h2,n), 0.0);
    spec2 = specular * pow(intSpec2, shininess);
  }

  vec4 spec3 = vec4(0.0);
  float intensity3 = 0.0;

  // Defining Spotlight
  // vec4 spotlightPos = vec4(0.0, 7.0, 0.0, 1.0);
  vec4 spotlightPos = vec4(-1.5, 7.0, 0.0, 1.0);
  vec3 ld = vec3(viewMatrix*spotlightPos - glpos);
  ld = normalize( vec3(ld));
  vec3 spotLightDir = vec3(viewMatrix*vec4(0.0, 1.0, 0.0, 0.0));
  spotLightDir = normalize(spotLightDir);

  if(dot(ld, spotLightDir) > 0.86) // Nearly 30 degrees
  {
  	n = normalize(normalMatrix * normalize(fNormal));
    float spotDotProduct1 = dot(n, ld);
  	intensity3 =  max( spotDotProduct1, 0.0 );

    if(intensity3 > 0.0)
    {
	  vec3 eye3 = normalize( vec3(-glpos));
	  vec3 h3 = normalize(ld + eye3 );
  	  float intSpec3 = max(dot(h3,n), 0.0);	
  	  spec3 = specular * pow(intSpec3, shininess);
    }
  }
  light_component = (light1 * (intensity1 * diffuse + spec1) + light2 * (intensity2 * diffuse + spec2) + spotlight1 * (intensity3 * diffuse + spec3) + ambient);

  frag_color = light_component * texture_tex;
}
