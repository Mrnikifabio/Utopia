uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 spotLightDirection;
uniform float spotLightCutoff;
uniform float spotLightAttenuation;

// Material properties:
uniform vec3 matEmission;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

in vec4 fragPosition;
in vec3 normal;

out vec4 fragOutput;

void main(void)
{
    // Ambient term:
    vec3 fragColor = matEmission + matAmbient * lightAmbient;
    
    // Diffuse term:
    vec3 _normal = normalize(normal);
    vec3 lightDirection = normalize(lightPosition - fragPosition.xyz);
    float nDotL = dot(lightDirection, _normal);
    if (nDotL > 0.0 && dot(-lightDirection, spotLightDirection) < cos(radians(spotLightCutoff)))
    {
        // Attenuation term:
        float distance = length(lightPosition - fragPosition.xyz);
        float attenuation = 1.0 / (1.0 + spotLightAttenuation * distance + spotLightAttenuation * distance * distance);
        
        fragColor += matDiffuse * nDotL * lightDiffuse * attenuation;
        
        // Specular term:
        vec3 halfVector = normalize(lightDirection + normalize(-fragPosition.xyz));
        float nDotHV = dot(_normal, halfVector);
        fragColor += matSpecular * pow(nDotHV, matShininess) * lightSpecular * attenuation;
    }
    
    // Final color:
    fragOutput = vec4(fragColor, 1.0);
}