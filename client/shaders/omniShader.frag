#version 440 core

in vec4 fragPosition;
in vec3 normal;

out vec4 fragOutput;

// Material properties:
uniform vec3 matEmission;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

// Light properties:
uniform vec3 omniLightPosition;
uniform vec3 omniLightAmbient;
uniform vec3 omniLightDiffuse;
uniform vec3 omniLightSpecular;

void main(void)
{
   // Ambient term:
   vec3 fragColor=matEmission+matAmbient*omniLightAmbient;
   
   // Diffuse term:
   vec3 _normal=normalize(normal);
   vec3 lightDirection=normalize(omniLightPosition-fragPosition.xyz);
   float nDotL=dot(lightDirection,_normal);
   if(nDotL>0.f)
   {
      fragColor+=matDiffuse*nDotL*omniLightDiffuse;
      
      // Specular term:
      vec3 halfVector=normalize(lightDirection+normalize(-fragPosition.xyz));
      float nDotHV=dot(_normal,halfVector);
      fragColor+=matSpecular*pow(nDotHV,matShininess)*omniLightSpecular;
   }
   
   // Final color:
   fragOutput=vec4(fragColor,1.f);
}