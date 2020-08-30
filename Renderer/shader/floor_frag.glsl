#version 120
struct light_point {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct light_tube {
	vec3 beg;
	vec3 end;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D	tex;
uniform	float		shininess;

uniform bool light_switch[10];
uniform light_point light0;
uniform light_tube	light1;
uniform light_point light2;
uniform light_point light3;
uniform light_point light4;
uniform light_point light5;
uniform light_point light6;
uniform light_point light7;
uniform light_point light8;
uniform light_point light9;

varying vec3 eye_pos;
varying vec3 eye_normal;
varying mat4 model_view_matrix;

// shadow map
uniform sampler2D	shadow_map[10];
varying vec4		shadow_map_coord[10];

bool is_in_shadow(in int shadow_map_id);
vec3 proc_light_point(in vec3 dcolor, in vec3 scolor, in light_point light, in int shadow_map_id);
vec3 proc_light_tube(in vec3 dcolor, in vec3 scolor, in light_tube light, in int shadow_map_id);

void main(void)
{
	vec3 result = vec3(0.0);

	vec3 dcolor = texture2D(tex, gl_TexCoord[0].st).xyz;
	vec3 scolor = vec3(0.8);

	if (light_switch[0]) {
		result += proc_light_point(dcolor, scolor, light0, 0);
	}
	if (light_switch[1]) {
		result += proc_light_tube(dcolor, scolor, light1, 1);
	}
	if (light_switch[2]) {
		result += proc_light_point(dcolor, scolor, light2, 2);
	}
	if (light_switch[3]) {
		result += proc_light_point(dcolor, scolor, light3, 3);
	}
	if (light_switch[4]) {
		result += proc_light_point(dcolor, scolor, light4, 4);
	}
	if (light_switch[5]) {
		result += proc_light_point(dcolor, scolor, light5, 5);
	}
	if (light_switch[6]) {
		result += proc_light_point(dcolor, scolor, light6, 6);
	}
	if (light_switch[7]) {
		result += proc_light_point(dcolor, scolor, light7, 7);
	}
	if (light_switch[8]) {
		result += proc_light_point(dcolor, scolor, light8, 8);
	}
	if (light_switch[9]) {
		result += proc_light_point(dcolor, scolor, light9, 9);
	}

	float lineardepth = (-0.001 - eye_pos.z) / (-0.001 + 25);
	//result = vec3(lineardepth);

	//gl_FragColor = vec4(result, 1.0);
	gl_FragData[0] = vec4(result, 1.0);	
	gl_FragData[1] = vec4(vec3(lineardepth), 1.0);
	gl_FragData[2] = vec4(eye_pos, 1.0);
}

bool is_in_shadow(in int shadow_map_id) {
	vec4 nshadow_map_coord = shadow_map_coord[shadow_map_id] / shadow_map_coord[shadow_map_id].w;	
	nshadow_map_coord = nshadow_map_coord * 0.5 + 0.5;
	nshadow_map_coord.z -= 0.005;// Prevent z-fighting...
		
	float z_from_light = texture2D(shadow_map[shadow_map_id], nshadow_map_coord.xy).r;
		
	if (shadow_map_coord[shadow_map_id].w > 0.0)
	 	return (z_from_light < nshadow_map_coord.z);
	return false;
}

vec3 proc_light_point(in vec3 dcolor, in vec3 scolor, in light_point light, in int shadow_map_id) {
	vec3 eye_light_pos = (model_view_matrix * vec4(light.position, 1.0)).xyz;

	// ambient.
	vec3 ambient = light.ambient * dcolor;

	// diffuse.
	vec3 normal = normalize(eye_normal);
	vec3 lightdirraw = eye_light_pos - eye_pos;
	vec3 lightdir = normalize(lightdirraw);
	float diffuse_factor = (max(dot(normal, lightdir), 0.0));
	vec3 diffuse = light.diffuse * (diffuse_factor * dcolor);

	// specular.
	vec3 viewdir = normalize(-eye_pos); // in eye space, camera is at (0, 0, 0).
	vec3 reflectdir = reflect(-lightdir, normal);
	float specular_factor = pow(max(dot(viewdir, reflectdir), 0.0), shininess);
	vec3 specular = light.specular * (specular_factor * scolor);

	float shadow = 1.0;
	if(is_in_shadow(shadow_map_id)) {
		shadow = 0.2;
	}

	return shadow * (ambient + diffuse + specular);
}

// Reference : http://dx.doi.org/10.1109/38.124286
vec3 proc_light_tube(in vec3 dcolor, in vec3 scolor, in light_tube light, in int shadow_map_id) {
	vec3 eye_beg_pos = (model_view_matrix * vec4(light.beg, 1.0)).xyz;
	vec3 eye_end_pos = (model_view_matrix * vec4(light.end, 1.0)).xyz;

	// ambient.
	vec3 ambient = light.ambient * dcolor;

	// diffuse.
	vec3 normal = normalize(eye_normal);
	vec3 l_0 = eye_beg_pos - eye_pos;
	vec3 l_d = eye_end_pos - eye_beg_pos;
	float coef_0 = dot(l_0, normal);
	float coef_1 = dot(l_d, normal);
	float coef_2 = dot(l_0, l_0);
	float coef_3 = 2.0 * dot(l_d, l_0);
	float coef_4 = dot(l_d, l_d);
	float diffuse_factor = 2.0 * coef_3 * sqrt(coef_2) * (coef_0 - coef_1);
	diffuse_factor += 4.0 * coef_0 * coef_4 * sqrt(coef_2);
	diffuse_factor += 4.0 * coef_1 * coef_2 * (sqrt(coef_2 + coef_3 + coef_4) - sqrt(coef_2));
	diffuse_factor += -2.0 * coef_0 * coef_3 * sqrt(coef_2 + coef_3 + coef_4);
	diffuse_factor /= sqrt(coef_2 + coef_3 + coef_4);
	diffuse_factor /= (4.0 * pow(sqrt(coef_2), 3) * coef_4 - coef_3 * coef_3 * sqrt(coef_2));
	vec3 diffuse = light.diffuse * (diffuse_factor * dcolor);

	// specular.
	float specular_factor = 0.0;
	vec3 specular = vec3(0.0);
	vec3 viewdir = normalize(-eye_pos); // in eye space, camera is at (0, 0, 0).
	vec3 reflectdir = reflect(-viewdir, normal);
	float denominator = dot(l_0, l_d) * dot(reflectdir, l_d) - dot(l_d, l_d) * dot(reflectdir, l_0);
	if(denominator != 0.0) {
		float nominator = dot(l_0, l_d) * dot(reflectdir, l_0) - dot(l_0, l_0) * dot(reflectdir, l_d);
		float t_smallest = nominator / denominator;
		if(t_smallest < 0.0)
			t_smallest = 0.0;
		else if(t_smallest > 1.0)
			t_smallest = 1.0;
		vec3 l_smallest = normalize(l_0 + l_d * t_smallest);
		specular_factor = pow(max(dot(reflectdir, l_smallest), 0.0), shininess);
		specular = light.specular * (specular_factor * scolor);
	}

	float shadow = 1.0;
	if (is_in_shadow(shadow_map_id)) {
		shadow = 0.2;
	}

	return shadow * (diffuse + ambient + specular);
}

// Reference : http://dx.doi.org/10.1109/38.124286
//vec3 proc_light_tube(in vec3 dcolor, in vec3 scolor, in light_tube light) {
//	vec3 eye_beg_pos = (model_view_matrix * vec4(light.beg, 1.0)).xyz;
//	vec3 eye_end_pos = (model_view_matrix * vec4(light.end, 1.0)).xyz;
//
//	// ambient.
//	vec3 ambient = light.ambient * dcolor;
//
//	// diffuse.
//	vec3 normal = normalize(eye_normal);
//	vec3 l_0 = eye_beg_pos - eye_pos;
//	vec3 l_d = eye_end_pos - eye_beg_pos;
//	float coef_0 = dot(l_0, normal);
//	float coef_1 = dot(l_d, normal);
//	float coef_2 = dot(l_0, l_0);
//	float coef_3 = 2.0 * dot(l_d, l_0);
//	float coef_4 = dot(l_d, l_d);
//	float diffuse_factor = 2.0 * coef_3 * sqrt(coef_2) * (coef_0 - coef_1);
//	diffuse_factor += 4.0 * coef_0 * coef_4 * sqrt(coef_2);
//	diffuse_factor += 4.0 * coef_1 * coef_2 * (sqrt(coef_2 + coef_3 + coef_4) - sqrt(coef_2));
//	diffuse_factor += -2.0 * coef_0 * coef_3 * sqrt(coef_2 + coef_3 + coef_4);
//	diffuse_factor /= sqrt(coef_2 + coef_3 + coef_4);
//	diffuse_factor /= (4.0 * pow(sqrt(coef_2), 3) * coef_4 - coef_3 * coef_3 * sqrt(coef_2));
//	vec3 diffuse = light.diffuse * (diffuse_factor * dcolor);
//
//	// specular.
//	float specular_factor = 0.0;
//	vec3 specular = vec3(0.0);
//	vec3 viewdir = normalize(-eye_pos); // in eye space, camera is at (0, 0, 0).
//	vec3 reflectdir = reflect(-viewdir, normal);
//	float denominator = dot(l_0, l_d) * dot(reflectdir, l_d) - dot(l_d, l_d) * dot(reflectdir, l_0);
//	if(denominator != 0.0) {
//		float nominator = dot(l_0, l_d) * dot(reflectdir, l_0) - dot(l_0, l_0) * dot(reflectdir, l_d);
//		float t_smallest = nominator / denominator;
//		if(t_smallest < 0.0)
//			t_smallest = 0.0;
//		else if(t_smallest > 1.0)
//			t_smallest = 1.0;
//		vec3 l_smallest = normalize(l_0 + l_d * t_smallest);
//		specular_factor = pow(max(dot(reflectdir, l_smallest), 0.0), shininess);
//		specular = light.specular * (specular_factor * scolor);
//	}
//	return (diffuse + ambient);
//}