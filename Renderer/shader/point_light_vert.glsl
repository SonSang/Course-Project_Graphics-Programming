#version 120

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;	// [gl_Position] indicates screen space coordinate.
}