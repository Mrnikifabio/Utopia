#version 440 core

uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 spotLightDirection;
uniform float spotLightCutoff;
uniform float spotLightLinearAttenuation;
uniform float spotLightQuadraticAttenuation;

// Material properties:
uniform vec3 matEmission;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;


uniform int nLightUsed;

in vec4 fragPosition;
in vec3 normal;
in vec2 texCoord;  

out vec4 fragOutput;

layout(binding = 0) uniform sampler2D texSampler;

float quadraticFunction(float x);

void main(void)
{
    // Texture element:
    vec4 texel = texture(texSampler, texCoord); 

    // Ambient term:
    vec3 fragColor=matEmission+matAmbient*lightAmbient;
    fragColor /= nLightUsed; 
    // Diffuse term:
    vec3 _normal=normalize(normal);
    vec3 lightDirection=normalize(lightPosition-fragPosition.xyz);
    float nDotL=dot(lightDirection,_normal);


    float angle = degrees(acos(abs(dot(-lightDirection,normalize(lightPosition-spotLightDirection)))));
    if(angle<=spotLightCutoff)
    {
        
        if(nDotL>0.f)
        {
            float distance=length(lightPosition-fragPosition.xyz);
            float attenuation=1.f/(1.f+spotLightLinearAttenuation*distance+spotLightQuadraticAttenuation*distance*distance);
            float sideAttenuation= quadraticFunction(angle/spotLightCutoff);
            if(sideAttenuation<0.f)
                sideAttenuation=0.f;
            fragColor+=matDiffuse*nDotL*lightDiffuse*attenuation*sideAttenuation;
            
            // Specular term:
            vec3 halfVector=normalize(lightDirection+normalize(-fragPosition.xyz));
            float nDotHV=dot(_normal,halfVector);
            fragColor+=matSpecular*pow(nDotHV,matShininess)*lightSpecular*sideAttenuation;
        }
    }
    
    // Final color:
    fragOutput=texel * vec4(fragColor,1.f);
}

float quadraticFunction(float x)
{
    return -x*x+1f;
}
