
	   #version 440 core

	   // Uniforms:
	   uniform mat4 projection;
	   uniform mat4 modelview;
	   uniform mat3 normalMatrix;

	   // Attributes:
	   layout(location = 0) in vec3 in_Position;
	   layout(location = 1) in vec3 in_Normal;

	   // Varying:
	   out vec4 fragPosition;
	   out vec3 normal;   

	   void main(void)
	   {
		  fragPosition = modelview * vec4(in_Position, 1.0f);
		  gl_Position = projection * fragPosition;      
		  normal = normalMatrix * in_Normal;
	   }