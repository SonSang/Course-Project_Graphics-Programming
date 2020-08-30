#version 120

//varying vec3 eye_pos;

void main(void)
{
	//eye_pos = (gl_ModelViewMatrix * gl_Vertex).xyz;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;	// [gl_Position] indicates screen space coordinate.
}