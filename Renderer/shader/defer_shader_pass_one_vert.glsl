#version 120

varying vec3 eye_pos;
varying vec3 eye_normal;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	eye_normal = normalize(gl_NormalMatrix * gl_Normal);
	eye_pos = (gl_ModelViewMatrix * gl_Vertex).xyz;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;	// [gl_Position] indicates screen space coordinate.
}