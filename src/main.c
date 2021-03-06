#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <model.h>
#include <draw.h>
#include <camera.h>
#include <SOIL/SOIL.h>
#include <time.h>
#include <init.h>


int mouse_x, mouse_y;


GLfloat light_position[] = {200.0f, 300.0f, 100.0f, 0.0f};
GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};


struct Camera camera;

struct Action action;
struct Action
{
	int move_forward;
	int move_backward;
	int step_left;
	int step_right;
	int increase_light;
	int decrease_light;
	int move_wheelbarrow_forward;
	int move_wheelbarrow_backward;
};

int previous_time;
typedef GLubyte Pixel;
int WINDOW_WIDTH;
int WINDOW_HEIGHT;

World world;
Move move;

int help, help_on = 0;

float speed = 30;
float angle = 135;

void update_camera_position(struct Camera* camera, double elapsed_time)
{
	double distance;

	distance = elapsed_time * CAMERA_SPEED*speed;

	if (action.move_forward == TRUE) {
		move_camera_forward(camera, distance);
	}

	if (action.move_backward == TRUE) {
		move_camera_backward(camera, distance);
	}

	if (action.step_left == TRUE) {
		step_camera_left(camera, distance);
	}

	if (action.step_right == TRUE) {
		step_camera_right(camera, distance);
	}
	if (action.increase_light == TRUE) {
		if (light_ambient[0] < 1)
			light_ambient[0] = light_ambient[1] = light_ambient[2] += 0.001;
	}

	if (action.decrease_light == TRUE) {
		if (light_ambient[0] > -0.51)
			light_ambient[0] = light_ambient[1] = light_ambient[2] -= 0.001;
	}

}


/**
  *Movement of the wheelbarrow
  */

void movement(Move* move){
if(action.move_wheelbarrow_forward== TRUE && move->wheelbarrow.z<=500){
	move-> wheelbarrow.z+=10;
}
if(action.move_wheelbarrow_backward== TRUE && move->wheelbarrow.z>-5){
	move-> wheelbarrow.z-=10;
}
}

double calc_elapsed_time()
{
	int current_time;
	double elapsed_time;

	current_time = glutGet(GLUT_ELAPSED_TIME);
	elapsed_time = (double)(current_time - previous_time) / 1000.0;
	previous_time=current_time;

	return elapsed_time;
}

void specialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		if (help_on) {
			help_on = 0;

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
		}
		else {
			help_on = 1;

			GLfloat ones[] = { 1, 1, 1, 1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ones);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ones);
		}
	}

}

void reshape(GLsizei width, GLsizei height) {
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (!help_on)
		gluPerspective(50.0, (GLdouble)width / (GLdouble)height, 0.1, 20000.0);
	else
		gluOrtho2D(0, width, height, 0);
}

/**
  *Show the help menu
  */

void draw_help() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, help);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1, 0);
	glVertex3f(WINDOW_WIDTH, 0, 0);

	glTexCoord2f(1, 1);
	glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	glTexCoord2f(0, 1);
	glVertex3f(0, WINDOW_HEIGHT, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutSwapBuffers();
}

void display() {
	if (!help_on) {
		double elapsed_time;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		

		elapsed_time = calc_elapsed_time();
		update_camera_position(&camera, elapsed_time);
		
		glLightfv(GL_LIGHT1, GL_POSITION, light_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		
		glMaterialfv(GL_FRONT, GL_SPECULAR, light_ambient);

		glEnable(GL_LIGHT1);
		
		
		draw_environment(world, move); 
		draw_entities(world, move);
		movement(&move);
		reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutSwapBuffers();
		set_view_point(&camera);
	}
	else {
		draw_help();
	}
}

void mouse_handler(int button, int state, int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}

void motion_handler(int x, int y)
{
	double horizontal, vertical;

	horizontal = mouse_x - x;
	vertical = mouse_y - y;

	rotate_camera(&camera, horizontal, vertical);

	mouse_x = x;
	mouse_y = y;

	glutPostRedisplay();
}

void key_handler(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		action.move_forward = TRUE;
		break;
	case 's':
		action.move_backward = TRUE;
		break;
	case 'a':
		action.step_left = TRUE;
		break;
	case 'd':
		action.step_right = TRUE;
		break;
	case '+':
		action.increase_light = TRUE;
		break;
	case '-':
		action.decrease_light = TRUE;
		break;
	case 'e':
		action.move_wheelbarrow_forward=TRUE;
		break;
	case 'q':
		action.move_wheelbarrow_backward=TRUE;
		break;
	}

	glutPostRedisplay();
}

void key_up_handler(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		action.move_forward = FALSE;
		break;
	case 's':
		action.move_backward = FALSE;
		break;
	case 'a':
		action.step_left = FALSE;
		break;
	case 'd':
		action.step_right = FALSE;
		break;
	case '+':
		action.increase_light = FALSE;
		break;
	case '-':
		action.decrease_light = FALSE;
		break;
	case 'e':
		action.move_wheelbarrow_forward= FALSE;
		break;
	case 'q':
		action.move_wheelbarrow_backward= FALSE;
		break;
	}

	glutPostRedisplay();
}

void idle()
{
	glutPostRedisplay();
}

GLuint load_texture(const char* filename ) {
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	GLuint texture_name;
	Pixel* image;
	glGenTextures(1, &texture_name);

	int width;
	int height;

	image = (Pixel*)SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

	glBindTexture(GL_TEXTURE_2D, texture_name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (Pixel*)image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	SOIL_free_image_data(image);



	return texture_name;
}

void set_callbacks() {
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_handler);
	glutKeyboardUpFunc(key_up_handler);
	glutMouseFunc(mouse_handler);
	glutMotionFunc(motion_handler);
	glutIdleFunc(idle);
	glutSpecialFunc(specialFunc);

}

int main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 1024);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int window = glutCreateWindow("OBZY4J beadando feladat");
	glutSetWindow(window);

	init();
	action.move_wheelbarrow_forward= FALSE;
	glutMainLoop();

	return 0;
}

