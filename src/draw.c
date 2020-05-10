#include <draw.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

double skybox_size = 500;
GLuint displayList1;
GLuint displayList2;
double rotateAngle = 0;

void draw_triangles(const struct Model* model)
{
	int i, k;
	int vertex_index, texture_index, normal_index;
	double x, y, z, normal_x, normal_y, normal_z, u, v;

	glBegin(GL_TRIANGLES);

	for (i = 0; i < model->n_triangles; ++i) {
		for (k = 0; k < 3; ++k) {
			normal_index = model->triangles[i].points[k].normal_index;
			normal_x = model->normals[normal_index].x;
			normal_y = model->normals[normal_index].y;
			normal_z = model->normals[normal_index].z;
			glNormal3d(normal_x, normal_y, normal_z);

			vertex_index = model->triangles[i].points[k].vertex_index;
			x = model->vertices[vertex_index].x;
			y = model->vertices[vertex_index].y;
			z = model->vertices[vertex_index].z;
            switch(k){
            case 0:
                glTexCoord2f(0, 0);
                break;
            case 1:
                glTexCoord2f(0.1*z, 0);
                break;
            case 2:
                glTexCoord2f(0, 0.01);
                break;
            }
			glVertex3d(x, y, z);

		}
	}

	glEnd();
}


void draw_quads(const struct Model* model)
{
    int i, k;
    int vertex_index, texture_index, normal_index;
    double x, y, z, u, v;

	glBegin(GL_QUADS);

    for (i = 0; i < model->n_quads; ++i) {
        for (k = 0; k < 4; ++k) {

            texture_index = model->quads[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            // NOTE: The 1-v is model file specific!
            glTexCoord2f(u, 1-v);

            vertex_index = model->quads[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3d(x, y, z);
        }
    }    

    glEnd();
}

void draw_model(const struct Model* model)
{
	draw_triangles(model);
	draw_quads(model);
}


void draw_normals(const struct Model* model, double length)
{
	int i;
	double x1, y1, z1, x2, y2, z2;

	glColor3f(0, 0, 1);

	glBegin(GL_LINES);

	for (i = 0; i < model->n_vertices; ++i) {
		x1 = model->vertices[i].x;
		y1 = model->vertices[i].y;
		z1 = model->vertices[i].z;
		x2 = x1 + model->normals[i].x * length;
		y2 = y1 + model->normals[i].y * length;
		z2 = z1 + model->normals[i].z * length;
		glVertex3d(x1, y1, z1);
		glVertex3d(x2, y2, z2);
	}
	glEnd();
}

void draw_ground(int ground) {
	glBindTexture(GL_TEXTURE_2D, ground);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);
	int x, z;
	for (x = -skybox_size; x < skybox_size; x += 50) {
		for (z = -skybox_size; z < skybox_size; z += 50) {
			glTexCoord2f(0.0, 0.0);
			glNormal3f(0, -1, 0);
			glVertex3f(x, 0, z);
			glTexCoord2f(1.0, 0.0);
			glNormal3f(0, -1, 0);
			glVertex3f(x + 50, 0, z);
			glTexCoord2f(1.0, 1.0);
			glNormal3f(0, -1, 0);
			glVertex3f(x + 50, 0, z + 50);
			glTexCoord2f(0.0, 1.0);
			glNormal3f(0, -1, 0);
			glVertex3f(x, 0, z + 50);
		}
	}
	glEnd();
}
void draw_skybox_left(Skybox skybox) {
	glBindTexture(GL_TEXTURE_2D, skybox.left);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-skybox_size, 0, -skybox_size);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-skybox_size, skybox_size, -skybox_size);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-skybox_size, skybox_size, skybox_size);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-skybox_size, 0, skybox_size);
	glEnd();
}
void draw_skybox_right(Skybox skybox) {
	glBindTexture(GL_TEXTURE_2D, skybox.right);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(skybox_size, 0, skybox_size);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(skybox_size, skybox_size, skybox_size);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(skybox_size, skybox_size, -skybox_size);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(skybox_size, 0, -skybox_size);
	glEnd();
}
void draw_skybox_front(Skybox skybox) {
	glBindTexture(GL_TEXTURE_2D, skybox.front);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-skybox_size, 0, -skybox_size);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(skybox_size, 0, -skybox_size);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(skybox_size, skybox_size, -skybox_size);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-skybox_size, skybox_size, -skybox_size);
	glEnd();
}
void draw_skybox_back(Skybox skybox) {
	glBindTexture(GL_TEXTURE_2D, skybox.back);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-skybox_size, 0, skybox_size);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-skybox_size, skybox_size, skybox_size);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(skybox_size, skybox_size, skybox_size);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(skybox_size, 0, skybox_size);
	glEnd();
}
void draw_skybox_top(Skybox skybox) {
	glBindTexture(GL_TEXTURE_2D, skybox.top);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(skybox_size, skybox_size, skybox_size);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(skybox_size, skybox_size, -skybox_size);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-skybox_size, skybox_size, -skybox_size);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-skybox_size, skybox_size, skybox_size);


	glEnd();
}
void draw_environment(World world, Move move) {
	glEnable(GL_TEXTURE_2D);
    displayList1 = glGenLists(1);
	glNewList(displayList1, GL_COMPILE);
		GLfloat material_specular[] = {1, 1, 1, 1};
        GLfloat material_ambient[] = {0.9, 0.9, 0.9, 1};
		GLfloat material_diffuse[] = {1, 1, 1, 1};
		GLfloat material_shininess[] = { 100.0 };
		
		glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);

        glPushMatrix();
        	glTranslatef(move.wheelbarrow.x, move.wheelbarrow.y, move.wheelbarrow.z);
		glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
		glBindTexture(GL_TEXTURE_2D, world.wheelbarrow.texture);
		glScalef(3.0f, 3.0f, 3.0f);
    		draw_model(&world.wheelbarrow.model);
	glPopMatrix();
		

	glEndList();
	
	displayList2 = glGenLists(1);
	glNewList(displayList2, GL_COMPILE);
	
	GLfloat material_ambient_2[] = {0.5, 0.5, 0.0, 1};
	GLfloat material_specular_2[] = {0.5, 0.5, 0.0, 1};
	GLfloat material_diffuse1[] = {0.5, 0.5, 0, 1};

	    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_2);
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_2);
        glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse1);
		
	
		
	glEndList();

	
	GLfloat zeros[] = { 0, 0, 0 };
	GLfloat ones[] = { 1, 1, 1 };
	
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ones);
	draw_ground(world.ground);

	

	

	draw_skybox_left(world.skybox);
	draw_skybox_right(world.skybox);
	draw_skybox_front(world.skybox);
	draw_skybox_back(world.skybox);
	draw_skybox_top(world.skybox);

	glDisable(GL_TEXTURE_2D);

}
void load_skybox(Skybox* skybox) {
	skybox->front = load_texture("textures//fence.jpg");
	skybox->left = load_texture("textures//fence.jpg");
	skybox->right = load_texture("textures//fence.jpg");
	skybox->back = load_texture("textures//fence.jpg");
	skybox->top = load_texture("textures//sky.jpg");
}

void draw_wheelbarrow(World world) 
{

			glPushMatrix();
				glTranslatef(180, 0, -250);

				glCallList(displayList1);
				glPushMatrix();
					
					glCallList(displayList2);
				glPopMatrix();
			glPopMatrix();
}

void draw_entities(World world, Move move) {
	glEnable(GL_TEXTURE_2D);
	draw_wheelbarrow(world);

	
	glDisable(GL_TEXTURE_2D);

}







