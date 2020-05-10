#include <init.h>
#include <camera.h>
#include <draw.h>
#include <GL/glut.h>

struct Camera camera;
World world;
Move move;
int help;

void init(){
	set_callbacks();
	init_camera(&camera);
	init_entities(&world, move);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	help = load_texture("textures//help.jpg");
}