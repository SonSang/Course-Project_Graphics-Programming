#version 120

varying vec3 eye_pos;
varying vec3 eye_normal;
varying mat4 model_view_matrix;

// shadow map
uniform mat4 shadow_map_matrix[10];
varying vec4 shadow_map_coord[10];

void main(void)
{
	eye_normal			= normalize(gl_NormalMatrix * gl_Normal);
	eye_pos				= (gl_ModelViewMatrix * gl_Vertex).xyz;
	model_view_matrix	= gl_ModelViewMatrix;

	for (int i = 0; i < 10; i++)
		shadow_map_coord[i] = shadow_map_matrix[i] * gl_Vertex;

	gl_Position			= gl_ModelViewProjectionMatrix * gl_Vertex;	// [gl_Position] indicates screen space coordinate.
}