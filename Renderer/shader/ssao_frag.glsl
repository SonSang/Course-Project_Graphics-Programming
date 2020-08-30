#version 120

uniform sampler2D color;
uniform sampler2D linear_depth;
uniform sampler2D eye_pos;

uniform vec3 kernel[10];

uniform mat4 projection;

uniform bool use_ssao;
uniform float ssao_radius;

void main(void) {
	//gl_FragColor = texture2D(eye_pos, gl_TexCoord[0].st);
	vec4 original_color = texture2D(color, gl_TexCoord[0].st);

	float occlusion = 0.0;
	if (use_ssao) {
		for (int i = 0; i < 10; i++) {
			vec3 shoot = kernel[i];
			shoot = normalize(shoot);
			shoot = shoot * ssao_radius + texture2D(eye_pos, gl_TexCoord[0].st).xyz;

			vec4 offset = vec4(shoot, 1.0);

			float tz = offset.z;

			offset = projection * offset;
			offset.xy /= offset.w;
			offset.xy = offset.xy * 0.5 + 0.5;

			float sample_linear_depth = texture2D(linear_depth, offset.xy).r;
			float offset_linear_depth = (-0.001 - tz) / (-0.001 + 25);

			if (offset_linear_depth >= sample_linear_depth)
				occlusion += 0.1;
		}
	}

	gl_FragColor = (1 - occlusion) * original_color;
}