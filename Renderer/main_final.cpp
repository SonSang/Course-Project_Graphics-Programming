#include "opengl/glew/glew.h"
#include "opengl/glew/wglew.h"
#include "opengl/freeglut/freeglut.h"

#include "header/camera.h"
#include "header/mouse.h"
#include "header/shader.h"
#include "header/io.h"
#include "header/light.h"
#include "header/parser.h"

#include "header/timer.h"

#include <iostream>
#include <random>

using namespace RENDERER;

#define INIT_WINDOW_WIDTH 1200
#define INIT_WINDOW_HEIGHT 800
#define INIT_WINDOW_POS_X 10
#define INIT_WINDOW_POS_Y 10

int window_width = INIT_WINDOW_WIDTH;
int window_height = INIT_WINDOW_HEIGHT;
int window_pos_x = INIT_WINDOW_POS_X;
int window_pos_y = INIT_WINDOW_POS_Y;

camera	main_camera;
mouse	main_mouse;

// shader
shader	floor_shader;
shader	wall_shader;
shader	model_shader;
shader	point_light_shader;

// model
parser_obj chicken;
parser_obj elk;
parser_obj boar;
parser_obj bobcat;
parser_obj chamois;
parser_obj goose;
parser_obj sheep;
parser_obj shire;
parser_obj tiger;

// texture
GLuint	floor_texture_diffuse_id;
GLuint	wall_texture_diffuse_id;

// light
bool		light_switch[10] = { true, true, false, false, false, false, false, false, false, false };
light_point	light0;
light_tube	light1;
light_point light2;
light_point light3;
light_point light4;
light_point light5;
light_point light6;
light_point light7;
light_point light8;
light_point light9;

// shadow map
GLuint	fbo_sm[10];			// same as number of lights.
GLuint	shadow_map[10];		// 0 : color, 1 : depth.
float	shadow_map_matrix[10][16];
GLsizei tex_width = 4096, tex_height = 4096;
shader	shadow_map_shader;

// ssao
GLuint	fbo_ssao;
GLuint	ssao_color, ssao_linear_depth, ssao_eye_pos, ssao_depth;
shader	ssao_shader;
bool	use_ssao = false;
float	ssao_radius = 0.01;
float	ssao_radius_unit = 0.01;

// deferred rendering
GLuint	fbo_defer;
GLuint	defer_diffuse_specular, defer_eye_pos, defer_eye_normal, defer_linear_depth, defer_depth;
shader	defer_shader_pass_1;
shader	defer_shader_pass_2;
bool	use_defer = false;

// FPS
int		frame_count = 0;
double	elapsed_time = 0;
double	cur_time = 0;
double	pre_time = 0;

// displaylist
bool			initdisplaylist = true;
GLuint			displaylist;

// renderer initialization.
void init_camera() {
	main_camera.update_view();
}
void init_light0() {
	// light0 : point.
	light0.set_position(1, 1, 5);
	light0.set_ambient(0.1, 0.1, 0.1);
	light0.set_diffuse(1.0, 1.0, 1.0);
	light0.set_specular(1.0, 1.0, 1.0);
}
void init_light1() {
	// light1 : tube.
	light1.set_beg(0, 3, 2);
	light1.set_end(10, 3, 2);
	light1.set_ambient(0.1, 0.1, 0.1);
	light1.set_diffuse(1.0, 1.0, 1.0);
	light1.set_specular(1.0, 1.0, 1.0);
}
void init_light2() {
	// light2 : point.
	light2.set_position(2, 1, 5);
	light2.set_ambient(0.1, 0.1, 0.1);
	light2.set_diffuse(1.0, 0, 0);
	light2.set_specular(1.0, 0, 0);
}
void init_light3() {
	// light3 : point.
	light3.set_position(3, 1, 5);
	light3.set_ambient(0.1, 0.1, 0.1);
	light3.set_diffuse(0, 1.0, 0);
	light3.set_specular(0, 1.0, 0);
}
void init_light4() {
	// light4 : point.
	light4.set_position(4, 1, 5);
	light4.set_ambient(0.1, 0.1, 0.1);
	light4.set_diffuse(0, 0, 1.0);
	light4.set_specular(0, 0, 1.0);
}
void init_light5() {
	// light5 : point.
	light5.set_position(5, 1, 5);
	light5.set_ambient(0.1, 0.1, 0.1);
	light5.set_diffuse(1.0, 1.0, 0);
	light5.set_specular(1.0, 1.0, 0);
}
void init_light6() {
	// light6 : point.
	light6.set_position(6, 1, 5);
	light6.set_ambient(0.1, 0.1, 0.1);
	light6.set_diffuse(1.0, 0, 1.0);
	light6.set_specular(1.0, 0, 1.0);
}
void init_light7() {
	// light7 : point.
	light7.set_position(7, 1, 5);
	light7.set_ambient(0.1, 0.1, 0.1);
	light7.set_diffuse(0, 1.0, 1.0);
	light7.set_specular(0, 1.0, 1.0);
}
void init_light8() {
	// light8 : point.
	light8.set_position(8, 1, 5);
	light8.set_ambient(0.1, 0.1, 0.1);
	light8.set_diffuse(1.0, 0.5, 0.2);
	light8.set_specular(1.0, 0.5, 0.2);
}
void init_light9() {
	// light9 : point.
	light9.set_position(9, 1, 5);
	light9.set_ambient(0.1, 0.1, 0.1);
	light9.set_diffuse(0.3, 0.7, 0.2);
	light9.set_specular(0.3, 0.7, 0.2);
}
void init_light() {
	init_light0();
	init_light1();
	init_light2();
	init_light3();
	init_light4();
	init_light5();
	init_light6();
	init_light7();
	init_light8();
	init_light9();
}

// shader initialization.
void init_floor_shader() {
	floor_shader.set_program("shader/floor_vert.glsl", "shader/floor_frag.glsl");
}
void init_wall_shader() {
	wall_shader.set_program("shader/floor_vert.glsl", "shader/floor_frag.glsl");
}
void init_model_shader() {
	model_shader.set_program("shader/model_vert.glsl", "shader/model_frag.glsl");
}
void init_point_light_shader() {
	point_light_shader.set_program("shader/point_light_vert.glsl", "shader/point_light_frag.glsl");
}
void init_shadow_map_shader() {
	shadow_map_shader.set_program("shader/shadow_map_vert.glsl", "shader/shadow_map_frag.glsl");
}
void init_ssao_shader() {
	ssao_shader.set_program("shader/ssao_vert.glsl", "shader/ssao_frag.glsl");
}
void init_defer_shader() {
	defer_shader_pass_1.set_program("shader/defer_shader_pass_one_vert.glsl", "shader/defer_shader_pass_one_frag.glsl");
	defer_shader_pass_2.set_program("shader/defer_shader_pass_two_vert.glsl", "shader/defer_shader_pass_two_frag.glsl");
}
void init_shader() {
	std::cout << "Initializing floor shader..." << std::endl;
	init_floor_shader();
	std::cout << "Initializing wall shader..." << std::endl;
	init_wall_shader();
	std::cout << "Initializing model shader..." << std::endl;
	init_model_shader();
	std::cout << "Initializing shadow map shader..." << std::endl;
	init_shadow_map_shader();
	std::cout << "Initializing SSAO shader..." << std::endl;
	init_ssao_shader();
	std::cout << "Initializing defer shader..." << std::endl;
	init_defer_shader();
}

// shader data transfer.
void send_floor_shader_data() {
	floor_shader.use();	// must call this before data transfer.

	// texture data.
	GLint
		loc;
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, floor_texture_diffuse_id);
		loc = floor_shader.get_uniform_location("tex");
		glUniform1i(loc, 0);

		loc = floor_shader.get_uniform_location("shininess");
		glUniform1f(loc, 1000);
	}
	

	// light data : point
	float
		tmp[3];
	{
		float
			tmp[3];
		loc = floor_shader.get_uniform_location("light0.position");
		light0.get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light0.ambient");
		light0.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light0.diffuse");
		light0.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light0.specular");
		light0.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light_switch[0]");
		glUniform1i(loc, light_switch[0]);
	}

	// light data : tube.
	{
		loc = floor_shader.get_uniform_location("light1.beg");
		light1.get_beg().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light1.end");
		light1.get_end().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light1.ambient");
		light0.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light1.diffuse");
		light0.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light1.specular");
		light0.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location("light_switch[1]");
		glUniform1i(loc, light_switch[1]);
	}

	const light_point
		*pointlights[8] = { &light2, &light3, &light4, &light5, &light6, &light7, &light8, &light9 };
	for(int i = 0; i < 8; i++) 
	{
		loc = floor_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".position"));
		pointlights[i]->get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".ambient"));
		pointlights[i]->get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".diffuse"));
		pointlights[i]->get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".specular"));
		pointlights[i]->get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = floor_shader.get_uniform_location(std::string("light_switch[") + std::to_string(i + 2) + std::string("]"));
		glUniform1i(loc, light_switch[i + 2]);
	}

	// shadow map data.
	{
		/*float
			result[16];
		GLfloat
			pos[3];
		light0.get_position().to_float_array(pos);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-10, 10, -10, 10, 0, 10);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, 0, 1, 0);

		double mv[16];
		double pr[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);
		glGetDoublev(GL_PROJECTION_MATRIX, pr);

		glLoadIdentity();
		glMultMatrixd(pr);
		glMultMatrixd(mv);
		glGetFloatv(GL_MODELVIEW_MATRIX, result);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		*/
		for (int i = 0; i < 10; i++) {
			loc = floor_shader.get_uniform_location(std::string("shadow_map_matrix[") + std::to_string(i) + std::string("]"));
			glUniformMatrix4fv(loc, 1, false, shadow_map_matrix[i]);

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0 + i + 1);
			glBindTexture(GL_TEXTURE_2D, shadow_map[i]);
			loc = floor_shader.get_uniform_location(std::string("shadow_map[") + std::to_string(i) + std::string("]"));
			glUniform1i(loc, i + 1);
		}
	}
	floor_shader.nouse();
}
void send_wall_shader_data() {
	wall_shader.use();	// must call this before data transfer.

	// texture data.
	glActiveTexture(GL_TEXTURE0 + 11);
	glBindTexture(GL_TEXTURE_2D, wall_texture_diffuse_id);
	GLint loc = wall_shader.get_uniform_location("tex");
	glUniform1i(loc, 11);

	loc = wall_shader.get_uniform_location("shininess");
	glUniform1f(loc, 1000);

	// light data : point
	float
		tmp[3];
	{
		loc = wall_shader.get_uniform_location("light0.position");
		light0.get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light0.ambient");
		light0.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light0.diffuse");
		light0.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light0.specular");
		light0.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light_switch[0]");
		glUniform1i(loc, light_switch[0]);
	}
	

	// light data : tube.
	{
		loc = wall_shader.get_uniform_location("light1.beg");
		light1.get_beg().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light1.end");
		light1.get_end().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light1.ambient");
		light0.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light1.diffuse");
		light0.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light1.specular");
		light0.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location("light_switch[1]");
		glUniform1i(loc, light_switch[1]);
	}

	const light_point
		*pointlights[8] = { &light2, &light3, &light4, &light5, &light6, &light7, &light8, &light9 };
	for (int i = 0; i < 8; i++)
	{
		loc = wall_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".position"));
		pointlights[i]->get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".ambient"));
		pointlights[i]->get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".diffuse"));
		pointlights[i]->get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".specular"));
		pointlights[i]->get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = wall_shader.get_uniform_location(std::string("light_switch[") + std::to_string(i + 2) + std::string("]"));
		glUniform1i(loc, light_switch[i + 2]);
	}	

	{
		for (int i = 0; i < 10; i++) {
			loc = wall_shader.get_uniform_location(std::string("shadow_map_matrix[") + std::to_string(i) + std::string("]"));
			glUniformMatrix4fv(loc, 1, false, shadow_map_matrix[i]);

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0 + i + 12);
			glBindTexture(GL_TEXTURE_2D, shadow_map[i]);
			loc = wall_shader.get_uniform_location(std::string("shadow_map[") + std::to_string(i) + std::string("]"));
			glUniform1i(loc, i + 12);
		}
	}

	wall_shader.nouse();
}
void send_model_shader_data() {
	model_shader.use();	// must call this before data transfer.

	// texture data.
	int
		loc = model_shader.get_uniform_location("shininess");
	glUniform1f(loc, 1000);

	// light data : point
	float
		tmp[3];
	{
		loc = model_shader.get_uniform_location("light0.position");
		light0.get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light0.ambient");
		light0.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light0.diffuse");
		light0.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light0.specular");
		light0.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light_switch[0]");
		glUniform1i(loc, light_switch[0]);
	}
	
	// light data : tube.
	{
		loc = model_shader.get_uniform_location("light1.beg");
		light1.get_beg().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light1.end");
		light1.get_end().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light1.ambient");
		light0.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light1.diffuse");
		light0.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light1.specular");
		light0.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location("light_switch[1]");
		glUniform1i(loc, light_switch[1]);
	}
	
	const light_point
		*pointlights[8] = { &light2, &light3, &light4, &light5, &light6, &light7, &light8, &light9 };
	for (int i = 0; i < 8; i++)
	{
		loc = model_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".position"));
		pointlights[i]->get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".ambient"));
		pointlights[i]->get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".diffuse"));
		pointlights[i]->get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".specular"));
		pointlights[i]->get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = model_shader.get_uniform_location(std::string("light_switch[") + std::to_string(i + 2) + std::string("]"));
		glUniform1i(loc, light_switch[i + 2]);
	}

	{
		for (int i = 0; i < 10; i++) {
			loc = model_shader.get_uniform_location(std::string("shadow_map_matrix[") + std::to_string(i) + std::string("]"));
			glUniformMatrix4fv(loc, 1, false, shadow_map_matrix[i]);

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0 + i + 23);
			glBindTexture(GL_TEXTURE_2D, shadow_map[i]);
			loc = model_shader.get_uniform_location(std::string("shadow_map[") + std::to_string(i) + std::string("]"));
			glUniform1i(loc, i + 23);
		}
	}

	model_shader.nouse();
}
void send_ssao_shader_data_static() {
	ssao_shader.use();

	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> die(-100, 100);

	for (int i = 0; i < 10; i++) {
		vec3d
			kernel(die(mersenne), die(mersenne), die(mersenne));
		kernel.normalize();
		float
			tmp[3];
		kernel.to_float_array(tmp);
		GLuint
			loc = ssao_shader.get_uniform_location(std::string("kernel[") + std::to_string(i) + std::string("]"));
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);
	}

	GLuint
		loc = ssao_shader.get_uniform_location("projection");
	GLfloat
		pr[16];
	glGetFloatv(GL_PROJECTION_MATRIX, pr);
	glUniformMatrix4fv(loc, 1, false, pr);

	ssao_shader.nouse();


}
void send_ssao_shader_data_dynamic() {
	ssao_shader.use();

	GLuint
		loc = ssao_shader.get_uniform_location("use_ssao");
	glUniform1i(loc, use_ssao);
	
	loc = ssao_shader.get_uniform_location("ssao_radius");
	glUniform1f(loc, ssao_radius);

	ssao_shader.nouse();
}
void send_defer_shader_pass_2_data_static() {
	defer_shader_pass_2.use();	// must call this before data transfer.

	GLuint
		loc;
	// light data : point
	float
		tmp[3];
	{
		loc = defer_shader_pass_2.get_uniform_location("light0.position");
		light0.get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light0.ambient");
		light0.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light0.diffuse");
		light0.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light0.specular");
		light0.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light_switch[0]");
		glUniform1i(loc, light_switch[0]);
	}

	// light data : tube.
	{
		loc = defer_shader_pass_2.get_uniform_location("light1.beg");
		light1.get_beg().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light1.end");
		light1.get_end().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light1.ambient");
		light1.get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light1.diffuse");
		light1.get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light1.specular");
		light1.get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location("light_switch[1]");
		glUniform1i(loc, light_switch[1]);
	}

	const light_point
		*pointlights[8] = { &light2, &light3, &light4, &light5, &light6, &light7, &light8, &light9 };
	for (int i = 0; i < 8; i++)
	{
		loc = defer_shader_pass_2.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".position"));
		pointlights[i]->get_position().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".ambient"));
		pointlights[i]->get_ambient().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".diffuse"));
		pointlights[i]->get_diffuse().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location(std::string("light") + std::to_string(i + 2) + std::string(".specular"));
		pointlights[i]->get_specular().to_float_array(tmp);
		glUniform3f(loc, tmp[0], tmp[1], tmp[2]);

		loc = defer_shader_pass_2.get_uniform_location(std::string("light_switch[") + std::to_string(i + 2) + std::string("]"));
		glUniform1i(loc, light_switch[i + 2]);
	}

	// shadow map data.
	{
		for (int i = 0; i < 10; i++) {
			loc = defer_shader_pass_2.get_uniform_location(std::string("shadow_map_matrix[") + std::to_string(i) + std::string("]"));
			glUniformMatrix4fv(loc, 1, false, shadow_map_matrix[i]);

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0 + i + 1);
			glBindTexture(GL_TEXTURE_2D, shadow_map[i]);
			loc = defer_shader_pass_2.get_uniform_location(std::string("shadow_map[") + std::to_string(i) + std::string("]"));
			glUniform1i(loc, i + 1);
		}
	}
	defer_shader_pass_2.nouse();
}
void send_shader_data() {
	send_floor_shader_data();
	send_wall_shader_data();
	send_model_shader_data();
	send_ssao_shader_data_static();
	send_ssao_shader_data_dynamic();
	send_defer_shader_pass_2_data_static();
}

void send_defer_shader_pass_1_data(bool use_texture, const vec3d &diffuse_color, GLuint texture_id, int texture_unit_id, float shininess) {
	defer_shader_pass_1.use();
	GLuint
		loc;
	loc = defer_shader_pass_1.get_uniform_location("use_texture");
	glUniform1i(loc, use_texture);

	if (use_texture) {
		loc = defer_shader_pass_1.get_uniform_location("tex");
		glActiveTexture(GL_TEXTURE0 + texture_unit_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glUniform1i(loc, texture_unit_id);
	}
	else {
		loc = defer_shader_pass_1.get_uniform_location("diffuse_color");
		glUniform3f(loc, (float)diffuse_color[0], (float)diffuse_color[1], (float)diffuse_color[2]);
	}

	loc = defer_shader_pass_1.get_uniform_location("shininess");
	glUniform1f(loc, shininess);
	defer_shader_pass_1.nouse();
}
void send_defer_shader_pass_2_data() {
	defer_shader_pass_2.use();
	GLuint
		loc;
	loc = defer_shader_pass_2.get_uniform_location("diffuse_specular");
	glActiveTexture(GL_TEXTURE0 + 70);
	glBindTexture(GL_TEXTURE_2D, defer_diffuse_specular);
	glUniform1i(loc, 70);

	loc = defer_shader_pass_2.get_uniform_location("eye_pos");
	glActiveTexture(GL_TEXTURE0 + 71);
	glBindTexture(GL_TEXTURE_2D, defer_eye_pos);
	glUniform1i(loc, 71);

	loc = defer_shader_pass_2.get_uniform_location("eye_normal");
	glActiveTexture(GL_TEXTURE0 + 72);
	glBindTexture(GL_TEXTURE_2D, defer_eye_normal);
	glUniform1i(loc, 72);

	loc = defer_shader_pass_2.get_uniform_location("linear_depth");
	glActiveTexture(GL_TEXTURE0 + 73);
	glBindTexture(GL_TEXTURE_2D, defer_linear_depth);
	glUniform1i(loc, 73);

	/*loc = defer_shader_pass_2.get_uniform_location("model_view_matrix");
	GLfloat
		mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	glUniform4fv(loc, 1, mv);*/

	defer_shader_pass_2.nouse();
}

// model initialization.
void load_model_chicken() {
	chicken.parse("resources/obj/chicken/chicken_01.obj");
	chicken.scale(0.012);
	chicken.translate(1, 0, 1.5);
	//chicken.set_normal();
}
void load_model_elk() {
	elk.parse("resources/obj/elk/big elk 01.obj");
	elk.scale(0.01);
	elk.translate(2, 0, 1.5);
}
void load_model_shire() {
	shire.parse("resources/obj/shire/SHIRE_01.obj");
	shire.scale(0.007);
	shire.translate(3, 0, 1.5);
}
void load_model_bobcat() {
	bobcat.parse("resources/obj/bobcat/lynx_bobcat_01.obj");
	bobcat.scale(0.015);
	bobcat.translate(4, 0, 1.5);
}
void load_model_sheep() {
	sheep.parse("resources/obj/sheep/sheep_01.obj");
	sheep.scale(0.015);
	sheep.translate(5, 0, 1.5);
}
void load_model_boar() {
	boar.parse("resources/obj/boar/wild_boar_scrofa_01.obj");
	boar.scale(0.01);
	boar.translate(6, 0, 1.5);
}
void load_model_chamois() {
	chamois.parse("resources/obj/chamois/chamois_01.obj");
	chamois.scale(0.015);
	chamois.translate(7, 0, 1.5);
}
void load_model_tiger() {
	tiger.parse("resources/obj/tiger/tyger_siberian_01.obj");
	tiger.scale(0.01);
	tiger.translate(8, 0, 2.5);
}
void load_model_goose() {
	goose.parse("resources/obj/goose/goose_01.obj");
	goose.scale(0.08);
	goose.translate(9, 0, 1.5);
}
void load_model() {
	load_model_chicken();
	load_model_elk();
	load_model_shire();
	load_model_bobcat();
	load_model_sheep();
	load_model_boar();
	load_model_chamois();
	load_model_tiger();
	load_model_goose();
}

// texture initialization.
void gl_load_floor_texture() {
	// 1. diffuse
	unsigned char
		*bits = 0;
	unsigned int
		width, height;
	read_image("resources/texture/4K-floor_plank/4K-floor_plank-diffuse.jpg", &bits, width, height);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &floor_texture_diffuse_id);
	glBindTexture(GL_TEXTURE_2D, floor_texture_diffuse_id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
void gl_load_wall_texture() {
	// 1. diffuse
	unsigned char
		*bits = 0;
	unsigned int
		width, height;
	read_image("resources/texture/4K-metal_rusty_2/4K-metal_rusty_2-diffuse.jpg", &bits, width, height);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &wall_texture_diffuse_id);
	glBindTexture(GL_TEXTURE_2D, wall_texture_diffuse_id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
void gl_load_texture() {
	gl_load_floor_texture();
	gl_load_wall_texture();
}

// shadow map initialization.
void init_shadow_map() {
	for (int i = 0; i < 10; i++) {
		glGenTextures(1, &shadow_map[i]);
		glBindTexture(GL_TEXTURE_2D, shadow_map[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, tex_width, tex_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// FBO 
		glGenFramebuffersEXT(1, &fbo_sm[i]);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_sm[i]);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, shadow_map[i], 0);
		glDrawBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);

		GLenum status;
		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
			std::cout << "FBO is not ok." << std::endl;
			switch (status) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				std::cout << "1" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				std::cout << "2" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				std::cout << "3" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				std::cout << "4" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				std::cout << "5" << std::endl;
				break;
			default:
				break;
			}
		}
		else
			std::cout << "FBO is ok." << std::endl;

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

// ssao initialization.
void init_ssao() {
	// color
	glGenTextures(1, &ssao_color);
	glBindTexture(GL_TEXTURE_2D, ssao_color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// linear_depth
	glGenTextures(1, &ssao_linear_depth);
	glBindTexture(GL_TEXTURE_2D, ssao_linear_depth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// eye_pos
	glGenTextures(1, &ssao_eye_pos);
	glBindTexture(GL_TEXTURE_2D, ssao_eye_pos);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height, 0, GL_RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// depth
	glGenTextures(1, &ssao_depth);
	glBindTexture(GL_TEXTURE_2D, ssao_depth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// FBO 
	glGenFramebuffersEXT(1, &fbo_ssao);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_ssao);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, ssao_color, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, ssao_linear_depth, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D, ssao_eye_pos, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, ssao_depth, 0);
	GLenum
		buf[3] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glDrawBuffers(3, buf);/*
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);*/
	/*GLenum
		buf[2] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	glDrawBuffers(2, buf);*/

	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		std::cout << "FBO is not ok." << std::endl;
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			std::cout << "1" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "2" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "3" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "4" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "5" << std::endl;
			break;
		default:
			break;
		}
	}
	else
		std::cout << "FBO is ok." << std::endl;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

// defer initialization.
void init_defer() {
	// diffuse_specular
	glGenTextures(1, &defer_diffuse_specular);
	glBindTexture(GL_TEXTURE_2D, defer_diffuse_specular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// eye_pos
	glGenTextures(1, &defer_eye_pos);
	glBindTexture(GL_TEXTURE_2D, defer_eye_pos);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height, 0, GL_RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// eye_normal
	glGenTextures(1, &defer_eye_normal);
	glBindTexture(GL_TEXTURE_2D, defer_eye_normal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height, 0, GL_RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// linear_depth
	glGenTextures(1, &defer_linear_depth);
	glBindTexture(GL_TEXTURE_2D, defer_linear_depth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// depth
	glGenTextures(1, &defer_depth);
	glBindTexture(GL_TEXTURE_2D, defer_depth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// FBO 
	glGenFramebuffersEXT(1, &fbo_defer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_defer);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, defer_diffuse_specular, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, defer_eye_pos, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D, defer_eye_normal, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_TEXTURE_2D, defer_linear_depth, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, defer_depth, 0);
	GLenum
		buf[4] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
	glDrawBuffers(4, buf);

	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		std::cout << "FBO is not ok." << std::endl;
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			std::cout << "1" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "2" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "3" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "4" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "5" << std::endl;
			break;
		default:
			break;
		}
	}
	else
		std::cout << "FBO is ok." << std::endl;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

// shadow map creation.
void gl_draw_axis();
void gl_draw_floor();
void gl_draw_wall();
void gl_draw_point_light();
void gl_draw_tube_light();
void gl_draw_light();

void gl_draw_model_chicken();
void gl_draw_model_elk();
void gl_draw_model();

//void make_shadow_map0() {
//	glViewport(0, 0, tex_width, tex_height);
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_sm);
//
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho(-10, 10, -10, 10, 0, 10);
//	{
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//		glLoadIdentity();
//		float
//			light_pos[3];
//		light0.get_position().to_float_array(light_pos);
//		gluLookAt(light_pos[0], light_pos[1], light_pos[2], 0, 0, 0, 0, 1, 0);
//
//		glClearColor(1, 1, 1, 1);
//		glClear(GL_DEPTH_BUFFER_BIT);
//		glEnable(GL_DEPTH_TEST);
//
//		shadow_map_shader.use();
//
//		gl_draw_floor();
//		gl_draw_wall();
//		gl_draw_model();
//
//		shadow_map_shader.nouse();
//
//		glPopMatrix();
//	}
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//
//	glViewport(0, 0, window_width, window_height);
//	glClearColor(0, 0, 0, 1);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
void make_shadow_map() {
	const light_point
		*pointlights[9] = { &light0, &light2, &light3, &light4, &light5, &light6, &light7, &light8, &light9 };
	for (int i = 0; i < 10; i++) {
		glViewport(0, 0, tex_width, tex_height);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_sm[i]);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-10, 10, -10, 10, 0, 10);
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			float
				light_pos[3];
			if (i != 1) {
				if (i == 0)
					light0.get_position().to_float_array(light_pos);
				else
					pointlights[i - 1]->get_position().to_float_array(light_pos);
				gluLookAt(light_pos[0], light_pos[1], light_pos[2], 0, 0, 0, 0, 1, 0);
			}
			else {
				vec3d
					middle = (light1.get_beg() + light1.get_end()) * 0.5;
				middle.to_float_array(light_pos);
				gluLookAt(light_pos[0], light_pos[1], light_pos[2], light_pos[0], 0, light_pos[2], 0, 0, -1);
			}

			glClearColor(1, 1, 1, 1);
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			shadow_map_shader.use();

			gl_draw_floor();
			gl_draw_wall();
			gl_draw_model();

			shadow_map_shader.nouse();

			// get shadow matrix.
			double mv[16];
			double pr[16];
			glGetDoublev(GL_MODELVIEW_MATRIX, mv);
			glGetDoublev(GL_PROJECTION_MATRIX, pr);

			glLoadIdentity();
			glMultMatrixd(pr);
			glMultMatrixd(mv);
			glGetFloatv(GL_MODELVIEW_MATRIX, shadow_map_matrix[i]);

			glPopMatrix();
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		glViewport(0, 0, window_width, window_height);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

// basic elements on the canvas.
void gl_draw_axis() {
	vec3d
		origin(0, 0, 0), xend(1e+10, 0, 0), yend(0, 1e+10, 0), zend(0, 0, 1e+10);
	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	origin.gl_draw_vertex();
	xend.gl_draw_vertex();
	glEnd();

	glColor3d(0, 1, 0);
	glBegin(GL_LINES);
	origin.gl_draw_vertex();
	yend.gl_draw_vertex();
	glEnd();

	glColor3d(0, 0, 1);
	glBegin(GL_LINES);
	origin.gl_draw_vertex();
	zend.gl_draw_vertex();
	glEnd();
}
void gl_draw_floor() {
	vec3d
		a(0, 0, 0),
		b(10, 0, 0),
		c(10, 0, 5),
		d(0, 0, 5);
	vec3d
		normal(0, 1, 0);
	floor_shader.use();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	normal.gl_draw_normal(); a.gl_draw_vertex();
	glTexCoord2f(2, 0);
	normal.gl_draw_normal(); b.gl_draw_vertex();
	glTexCoord2f(2, 1);
	normal.gl_draw_normal(); c.gl_draw_vertex();
	glTexCoord2f(0, 1);
	normal.gl_draw_normal(); d.gl_draw_vertex();
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	floor_shader.nouse();
}
void gl_draw_wall() {
	vec3d
		a(0, 0, 0),
		b(10, 0, 0),
		c(10, 2, 0),
		d(0, 2, 0);
	vec3d
		normal(0, 0, 1);
	wall_shader.use();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	normal.gl_draw_normal(); a.gl_draw_vertex();
	glTexCoord2f(10, 0);
	normal.gl_draw_normal(); b.gl_draw_vertex();
	glTexCoord2f(10, 2);
	normal.gl_draw_normal(); c.gl_draw_vertex();
	glTexCoord2f(0, 2);
	normal.gl_draw_normal(); d.gl_draw_vertex();
	glEnd();
	glDisable(GL_TEXTURE_2D);
	wall_shader.nouse();
}
void gl_draw_point_light() {
	// do not use shader...
	const light_point
		*pointlights[9] = { &light0, &light2, &light3, &light4, &light5, &light6, &light7, &light8, &light9 };
	for (int i = 0; i < 9; i++) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslated(pointlights[i]->get_position()[0],
			pointlights[i]->get_position()[1],
			pointlights[i]->get_position()[2]);		// point light location.
		glColor3d(1, 1, 1);
		glutSolidCube(0.01);
		glPopMatrix();
	}
}
void gl_draw_tube_light() {
	// do not use shader...
	glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	light1.get_beg().gl_draw_vertex();
	light1.get_end().gl_draw_vertex();
	glEnd();
}
void gl_draw_light() {
	gl_draw_point_light();
	gl_draw_tube_light();
}

void gl_draw_model_chicken() {
	model_shader.use();
	chicken.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_elk() {
	model_shader.use();
	elk.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_boar() {
	model_shader.use();
	boar.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_bobcat() {
	model_shader.use();
	bobcat.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_chamois() {
	model_shader.use();
	chamois.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_goose() {
	model_shader.use();
	goose.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_sheep() {
	model_shader.use();
	sheep.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_shire() {
	model_shader.use();
	shire.gl_draw();
	model_shader.nouse();
}
void gl_draw_model_tiger() {
	model_shader.use();
	tiger.gl_draw();
	model_shader.nouse();
}
void gl_draw_model() {
	gl_draw_model_chicken();
	gl_draw_model_elk();
	gl_draw_model_boar();
	gl_draw_model_bobcat();
	gl_draw_model_chamois();
	gl_draw_model_goose();
	gl_draw_model_sheep();
	gl_draw_model_shire();
	gl_draw_model_tiger();
}

void gl_draw_ssao() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_ssao);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_draw_floor();
	gl_draw_wall();
	gl_draw_light();
	gl_draw_model();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	ssao_shader.use();

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 50);
	glBindTexture(GL_TEXTURE_2D, ssao_color);
	GLuint
		loc = ssao_shader.get_uniform_location("color");
	glUniform1i(loc, 50);

	glActiveTexture(GL_TEXTURE0 + 51);
	glBindTexture(GL_TEXTURE_2D, ssao_linear_depth);
	loc = ssao_shader.get_uniform_location("linear_depth");
	glUniform1i(loc, 51);

	glActiveTexture(GL_TEXTURE0 + 52);
	glBindTexture(GL_TEXTURE_2D, ssao_eye_pos);
	loc = ssao_shader.get_uniform_location("eye_pos");
	glUniform1i(loc, 52);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2d(-1, -1);
	glTexCoord2f(1, 0); glVertex2d(1, -1);
	glTexCoord2f(1, 1); glVertex2d(1, 1);
	glTexCoord2f(0, 1); glVertex2d(-1, 1);
	glEnd();

	ssao_shader.nouse();
}

void gl_draw_defer() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_defer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 1. floor
	send_defer_shader_pass_1_data(true, vec3d(), floor_texture_diffuse_id, 60, 1000);
	{
		vec3d
			a(0, 0, 0),
			b(10, 0, 0),
			c(10, 0, 5),
			d(0, 0, 5);
		vec3d
			normal(0, 1, 0);
		defer_shader_pass_1.use();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		normal.gl_draw_normal(); a.gl_draw_vertex();
		glTexCoord2f(2, 0);
		normal.gl_draw_normal(); b.gl_draw_vertex();
		glTexCoord2f(2, 1);
		normal.gl_draw_normal(); c.gl_draw_vertex();
		glTexCoord2f(0, 1);
		normal.gl_draw_normal(); d.gl_draw_vertex();
		glEnd();
		defer_shader_pass_1.nouse();
	}

	// 2. wall
	send_defer_shader_pass_1_data(true, vec3d(), wall_texture_diffuse_id, 61, 1000);
	{
		vec3d
			a(0, 0, 0),
			b(10, 0, 0),
			c(10, 2, 0),
			d(0, 2, 0);
		vec3d
			normal(0, 0, 1);
		defer_shader_pass_1.use();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		normal.gl_draw_normal(); a.gl_draw_vertex();
		glTexCoord2f(10, 0);
		normal.gl_draw_normal(); b.gl_draw_vertex();
		glTexCoord2f(10, 2);
		normal.gl_draw_normal(); c.gl_draw_vertex();
		glTexCoord2f(0, 2);
		normal.gl_draw_normal(); d.gl_draw_vertex();
		glEnd();
		defer_shader_pass_1.nouse();
	}

	// 3. models
	send_defer_shader_pass_1_data(false, vec3d(0.5, 0.5, 0.5), 0, 0, 1000);
	{
		defer_shader_pass_1.use();
		chicken.gl_draw();
		elk.gl_draw();
		boar.gl_draw();
		bobcat.gl_draw();
		chamois.gl_draw();
		goose.gl_draw();
		sheep.gl_draw();
		shire.gl_draw();
		tiger.gl_draw();
		defer_shader_pass_1.nouse();
	}

	// 4. lights
	gl_draw_light();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	send_defer_shader_pass_2_data();
	{
		defer_shader_pass_2.use();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2d(-1, -1);
		glTexCoord2f(1, 0); glVertex2d(1, -1);
		glTexCoord2f(1, 1); glVertex2d(1, 1);
		glTexCoord2f(0, 1); glVertex2d(-1, 1);
		glEnd();
		defer_shader_pass_2.nouse();
	}
}

void gl_draw_basic_elements() {
	if (initdisplaylist) {
		make_shadow_map();
		send_shader_data();
	}
	initdisplaylist = false;

	if (!use_defer) {
		gl_draw_ssao();
	}
	else {
		gl_draw_defer();
	}
}

// gl callback.
void light_switch_toggle(int i) {
	light_switch[i] = !light_switch[i];
	std::cout << "Light " << i << " is toggled." << std::endl;
	send_shader_data();
	glutPostRedisplay();
}
void keyboard_button_callback(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		light_switch_toggle(0);
		break;
	case '2':
		light_switch_toggle(2);
		break;
	case '3':
		light_switch_toggle(3);
		break;
	case '4':
		light_switch_toggle(4);
		break;
	case '5':
		light_switch_toggle(5);
		break;
	case '6':
		light_switch_toggle(6);
		break;
	case '7':
		light_switch_toggle(7);
		break;
	case '8':
		light_switch_toggle(8);
		break;
	case '9':
		light_switch_toggle(9);
		break;
	case '0':
		light_switch_toggle(1);
		break;
	case 'm': case 'M': // SSAO toggle
		if (use_defer) {
			std::cout << "Not on Deferred rendering mode..." << std::endl;
			break;
		}
		use_ssao = !use_ssao;
		send_ssao_shader_data_dynamic();
		std::cout << "SSAO toggle : " << (use_ssao ? "ON" : "OFF") << std::endl;
		glutPostRedisplay();
		break;
	case 'b': case 'B': // SSAO radius up
		if (use_defer) {
			std::cout << "Not on Deferred rendering mode..." << std::endl;
			break;
		}
		if (use_ssao)
			ssao_radius += ssao_radius_unit;
		send_ssao_shader_data_dynamic();
		std::cout << "SSAO radius : " << ssao_radius << std::endl;
		glutPostRedisplay();
		break;
	case 'n': case 'N': // SSAO radius down
		if (use_defer) {
			std::cout << "Not on Deferred rendering mode..." << std::endl;
			break;
		}
		if (use_ssao)
			ssao_radius = (ssao_radius > ssao_radius_unit) ? ssao_radius - ssao_radius_unit : ssao_radius;
		if (ssao_radius <= ssao_radius_unit)
			ssao_radius = ssao_radius_unit;
		std::cout << "SSAO radius : " << ssao_radius << std::endl;
		send_ssao_shader_data_dynamic();
		glutPostRedisplay();
		break;
	case 'v': case 'V': // Deferred rendering toggle
		use_defer = !use_defer;
		std::cout << "Deferred rendering toggle : " << (use_defer ? "ON" : "OFF") << std::endl;
		glutPostRedisplay();
		break;
	case 'w': case 'W':	// Forward
		main_camera.move_back_forth(1);
		break;
	case 's': case 'S':	// Backward
		main_camera.move_back_forth(-1);
		break;
	case 'a': case 'A':		// Left
		main_camera.move_left_right(-1);
		break;
	case 'd': case 'D':		// Right
		main_camera.move_left_right(1);
		break;
	case (27):				// ESC : End program.
		exit(0);
		break;
	default: break;
	}
}
void mouse_button_callback(GLint button, GLint action, GLint x, GLint y) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)  // Left button pressed
		main_mouse.set_lpressed(true);
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) // Right button pressed
		main_mouse.set_rpressed(true);
	if (button == GLUT_MIDDLE_BUTTON && action == GLUT_DOWN) // Middle button pressed
		main_mouse.set_mpressed(true);

	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)  // Left button released
		main_mouse.set_lpressed(false);
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_UP) { // Right button released
		main_mouse.set_rpressed(false);
		main_mouse.set_first_mouse(true);
	}
	if (button == GLUT_MIDDLE_BUTTON && action == GLUT_UP) // Middle button released
		main_mouse.set_mpressed(false);
}
void mouse_move_callback(GLint x, GLint y) {
	if (main_mouse.get_rpressed()) {
		main_mouse.move_camera(main_camera, x, y);	// mouse right button to change camera angle.
	}
}
void reshape_callback(GLint width, GLint height) {
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(25, width / (double)height, 0.001, 25);
}
void display_callback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl_draw_basic_elements();

	glutSwapBuffers();

	frame_count++;
	cur_time = timer::get_time();
	elapsed_time += cur_time - pre_time;
	if (elapsed_time > 1) { // 1 sec
		std::cout << (elapsed_time / frame_count) * 1000 << "ms / frame" << std::endl;
		frame_count = 0;
		elapsed_time = 0;
	}
	pre_time = cur_time;
}
void idle_callback() {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	// initialize glut.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(window_pos_x, window_pos_y);
	glutCreateWindow("Renderer");

	// initialize glew.
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW failed : " << glewGetErrorString(glewInit()) << std::endl;
		exit(-1);
	}		

	// deactivate v-sync.
	if (wglewIsSupported("WGL_EXT_swap_control")) {
		// disable vertical sync
		// the same as setVSync(0) 
		std::cout << "Disable v-sync..." << std::endl;
		wglSwapIntervalEXT(0);
	}

	// callback registration.
	glutKeyboardFunc(keyboard_button_callback);
	glutMouseFunc(mouse_button_callback);
	glutMotionFunc(mouse_move_callback);
	glutReshapeFunc(reshape_callback);
	glutDisplayFunc(display_callback);
	glutIdleFunc(idle_callback);

	// rendering settings.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// initialize renderer settings. 
	init_camera();
	init_light();

	// initialize shader.
	init_shader();

	// load models.
	load_model();

	// initialize shadow map.
	std::cout << "Initializing shadow map fbo..." << std::endl;
	init_shadow_map();

	// initialize ssao.
	std::cout << "Initializing SSAO fbo..." << std::endl;
	init_ssao();

	// initialize defer.
	std::cout << "Initializing deferred rendering fbo..." << std::endl;
	init_defer();

	// load textures.
	gl_load_texture();

	// transfer shader data.
	//send_shader_data();

	cur_time = timer::get_time();
	pre_time = timer::get_time();
	glutMainLoop();
	return 0;
}