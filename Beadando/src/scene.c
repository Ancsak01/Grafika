#include "scene.h"
#include "load.h"
#include "draw.h"
#include "model.h"

#include <GL/glut.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct
{
    float Red;
    float Green;
    float Blue;
    float Opacity;
} diffLight;

void init_scene(Scene *scene)
{
    load_model(&(scene->apple), "data/obj/cube.obj");

    scene->texture_id_wall = load_texture("data/texture/padlo.jpg");
    scene->texture_id_ground = load_texture("data/texture/fal.png");
    scene->texture_id_sky = load_texture("data/texture/sky.jpg");
    scene->texture_id_apple = load_texture("data/texture/cube.png");

    diffLight.Red = 0.8f;
    diffLight.Green = 0.8f;
    diffLight.Blue = 0.8f;
    diffLight.Opacity = 1.0f;
}

float threshold_value(float val)
{
    if (val > 10.0f)
    {
        val = 10.0f;
    }
    else if (val < 0.0f)
    {
        val = 0.0f;
    }
    return val;
}

void set_lighting(float r, float g, float b, float a)
{
    printf("r: %f g: %f b: %f a: %f\n", diffLight.Red, diffLight.Green, diffLight.Blue, diffLight.Opacity);
    diffLight.Red = threshold_value(diffLight.Red + r);
    diffLight.Green = threshold_value(diffLight.Green + g);
    diffLight.Blue = threshold_value(diffLight.Blue + b);
    diffLight.Opacity = threshold_value(diffLight.Opacity + a);

    float ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float diffuse_light[] = {diffLight.Red, diffLight.Green, diffLight.Blue, diffLight.Opacity};
    float specular_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float position[] = {5.0f, 5.0f, 10.0f, 5.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material *material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue};

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue};

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void draw_scene(const Scene *scene)
{
    set_lighting(0, 0, 0, 0);

    glBindTexture(GL_TEXTURE_2D, scene->texture_id_apple);
	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(0, 0, 10);
    draw_model(&(scene->apple));
	glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, scene->texture_id_apple);
	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(8.6, -3, 0.5);
    draw_model(&(scene->apple));
	glPopMatrix();	

    glPushMatrix();
    glTranslatef(0, 0, 0);
    draw_floor(scene);
    draw_sky(scene);
    draw_wall(scene, 4 * -1, 5);
    glPopMatrix();

    glPushMatrix();
    draw_labirynth(scene);
    glPopMatrix();
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}

void draw_sky(const Scene *scene)
{
    glBindTexture(GL_TEXTURE_2D, scene->texture_id_sky);
    glBegin(GL_QUADS);
    glVertex3f(-10, -10, 10); /* - - + */
    glVertex3f(10, -10, 10); /* + - + */
    glVertex3f(10, 10, 10); /* + + + */
    glVertex3f(-10, 10, 10); /* - + + */
    glEnd();
}

void draw_wall(const Scene *scene, float height, float width)
{
    glBindTexture(GL_TEXTURE_2D, scene->texture_id_wall);
    glBegin(GL_QUADS);

    glVertex3f(-10, 10, -10); /* - + - */
    glVertex3f(10, 10, -10); /* + + - */
    glVertex3f(10, 10, 10); /* + + + */
    glVertex3f(-10, 10, 10); /* - + + */

    glVertex3f(-10, -10, -10); /* - - - */
    glVertex3f(10, -10, -10); /* + - - */
    glVertex3f(10, -10, 10); /* + - + */
    glVertex3f(-10, -10, 10); /* - - + */

    glVertex3f(10, 10, 10); /* + + + */
    glVertex3f(10, -10, 10); /* + - + */
    glVertex3f(10, -10, -10); /* + - - */
    glVertex3f(10, 10, -10); /* + + - */

    glVertex3f(-10, 10, 10); /* - + + */
    glVertex3f(-10, -10, 10); /* - - + */
    glVertex3f(-10, -10, -10); /* - - - */
    glVertex3f(-10, 10, -10); /* - + - */

    glEnd();
}

void draw_labirynth(const Scene* scene){
    glBindTexture(GL_TEXTURE_2D, scene->texture_id_wall);
    /* Outer circle Level 1 */
    glBegin(GL_QUADS);

    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-7.5, 7.5, -10); /* - + - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(7.5, 7.5, -10); /* + + - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(7.5, 7.5, 10); /* + + + */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-7.5, 7.5, 10); /* - + + */

    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-7.5, -7.5, -10); /* - - - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(7.5, -7.5, -10); /* + - - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(7.5, -7.5, 10); /* + - + */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-7.5, -7.5, 10); /* - - + */

    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(7.5, 6, 10); /* + + + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(7.5, -7.5, 10); /* + - + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(7.5, -7.5, -10); /* + - - */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(7.5, 6, -10); /* + + - */

    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-7.5, 7.5, 10); /* - + + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-7.5, -7.5, 10); /* - - + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-7.5, -7.5, -10); /* - - - */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-7.5, 7.5, -10); /* - + - */

    glEnd();

    /* Outer circle Level 2 */
    glBegin(GL_QUADS);

    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-6, 6, -10); /* - + - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(6, 6, -10); /* + + - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(6, 6, 10); /* + + + */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-6, 6, 10); /* - + + */

    /* Nyílásos fal */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-6, -6, -10); /* - - - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(4.5, -6, -10); /* + - - */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(4.5, -6, 10); /* + - + */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-6, -6, 10); /* - - + */

    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(6, 6, 10); /* + + + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(6, -6, 10); /* + - + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(6, -6, -10); /* + - - */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(6, 6, -10); /* + + - */

    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-6, 6, 10); /* - + + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-6, -6, 10); /* - - + */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-6, -6, -10); /* - - - */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-6, 6, -10); /* - + - */

    glEnd();

    /* Outer circle Level 3 */
    glBegin(GL_QUADS);

    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(-3, 4.5, -10); /* - + - */
    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(4.5, 4.5, -10); /* + + - */
    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(4.5, 4.5, 10); /* + + + */
    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(-3, 4.5, 10); /* - + + */

    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(-4.5, -4.5, -10); /* - - - */
    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(4.5, -4.5, -10); /* + - - */
    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(4.5, -4.5, 10); /* + - + */
    glTexCoord2f(-3.5f, 3.1f);
    glVertex3f(-4.5, -4.5, 10); /* - - + */

    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(4.5, 4.5, 10); /* + + + */
    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(4.5, -4.5, 10); /* + - + */
    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(4.5, -4.5, -10); /* + - - */
    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(4.5, 4.5, -10); /* + + - */

    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(-4.5, 4.5, 10); /* - + + */
    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(-4.5, -4.5, 10); /* - - + */
    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(-4.5, -4.5, -10); /* - - - */
    glTexCoord2f(-3.1f, 3.5f);
    glVertex3f(-4.5, 4.5, -10); /* - + - */

    glEnd();

    /* Inner circle */
    glBegin(GL_QUADS);

    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-3, 3, -10); /* - + - */
    glTexCoord2f(0.1f, 0.0f);
    glVertex3f(3, 3, -10); /* + + - */
    glTexCoord2f(0.1f, 0.0f);
    glVertex3f(3, 3, 10); /* + + + */
    glTexCoord2f(-0.1f, 0.0f);
    glVertex3f(-3, 3, 10); /* - + + */

    glTexCoord2f(-0.1f, -0.0f);
    glVertex3f(-3, -3, -10); /* - - - */
    glTexCoord2f(0.1f, -0.0f);
    glVertex3f(3, -3, -10); /* + - - */
    glTexCoord2f(0.1f, -0.0f);
    glVertex3f(3, -3, 10); /* + - + */
    glTexCoord2f(-0.1f, -0.0f);
    glVertex3f(-3, -3, 10); /* - - + */

    glTexCoord2f(0.0f, 0.1f);
    glVertex3f(3, 3, 10); /* + + + */
    glTexCoord2f(0.0f, -0.1f);
    glVertex3f(3, -3, 10); /* + - + */
    glTexCoord2f(0.0f, -0.1f);
    glVertex3f(3, -3, -10); /* + - - */
    glTexCoord2f(0.0f, 0.1f);
    glVertex3f(3, 3, -10); /* + + - */

    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-3, 3, 10); /* - + + */
    glTexCoord2f(-0.0f, -0.1f);
    glVertex3f(-3, -1.5, 10); /* - - + */
    glTexCoord2f(-0.0f, -0.1f);
    glVertex3f(-3, -1.5, -10); /* - - - */
    glTexCoord2f(-0.0f, 0.1f);
    glVertex3f(-3, 3, -10); /* - + - */

    glEnd();
}

void draw_floor(const Scene *scene)
{
    glBindTexture(GL_TEXTURE_2D, scene->texture_id_ground);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 50.0f);
    glVertex2i(-10.0f, 10.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(-10.0f, -10.0f);

    glTexCoord2f(50.0f, 0.0f);
    glVertex2i(10.0f, -10.0f);

    glTexCoord2f(50.0f, 50.0f);
    glVertex2i(10.0f, 10.0f);
    glEnd();
}