#version 120

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);	// [gl_Position] indicates screen space coordinate.
}