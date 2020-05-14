#include <init.h>
#include <camera.h>
#include <draw.h>
#include <GL/glut.h>

struct Camera camera;
World world;
Move move;
int help;

void init(){
	
    GLfloat light_position[] = {200.0f, 300.0f, 100.0f, 0.0f};
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
	set_callbacks();
	init_camera(&camera);
	init_entities(&world, move);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	help = load_texture("textures//help.jpg");
}
