#version 120

uniform bool		use_texture;
uniform vec3		diffuse_color;
uniform sampler2D	tex;
uniform	float		shininess;

varying vec3 eye_pos;
varying vec3 eye_normal;

void main(void)
{
	// 1. diffuse_specular
	vec3 diffuse;
	if (use_texture)
		diffuse = texture2D(tex, gl_TexCoord[0].st).xyz;
	else
		diffuse = diffuse_color;
	gl_FragData[0] = vec4(diffuse, shininess);

	// 2. eye_pos
	gl_FragData[1] = vec4(eye_pos, 1);

	// 3. eye_normal
	gl_FragData[2] = vec4(eye_normal, 1);

	// 4. linear_depth
	float lineardepth = (-0.001 - eye_pos.z) / (-0.001 + 25);
	gl_FragData[3] = vec4(vec3(lineardepth), 1);
}