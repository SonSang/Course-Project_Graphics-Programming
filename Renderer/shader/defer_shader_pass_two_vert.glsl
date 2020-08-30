#version 120

varying mat4 model_view_matrix;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	model_view_matrix = gl_ModelViewMatrix;
	gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);	// [gl_Position] indicates screen space coordinate.
}