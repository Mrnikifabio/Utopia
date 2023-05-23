//directional fragment shader
#version 440 core

in vec4 fragPosition;
in vec3 normal;
in vec2 texCoord;

out vec4 fragOutput;

layout(binding = 0) uniform sampler2D texSampler;

// Material properties:
uniform vec3 matEmission;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

uniform int nLightUsed;

// Directional light properties:
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

void main(void)
{
   vec3 lightDirection = normalize(vec3(0.0, -1.0, 0.0));

   // Texture element:
   vec4 texel = texture(texSampler, texCoord);

   // Ambient term:
   vec3 fragColor = matEmission + matAmbient * lightAmbient;

   // Diffuse term:
   vec3 _normal = normalize(normal);
   float nDotL = dot(-lightDirection, _normal);
   if (nDotL > 0.0)
   {
      fragColor += matDiffuse * nDotL * lightDiffuse;

      // Specular term:
      vec3 viewDirection = normalize(-fragPosition.xyz);
      vec3 reflectDirection = reflect(lightDirection, _normal);
      float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), matShininess);
      fragColor += matSpecular * specularFactor * lightSpecular / nLightUsed;
   }

   // Final color:
   fragOutput = texel * vec4(fragColor, 1.0) * 1.25;
}