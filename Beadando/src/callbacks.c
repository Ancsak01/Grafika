#include "callbacks.h"

#define VIEWPORT_RATIO (4.0 / 3.0)
#define VIEWPORT_ASPECT 50.0

int draw_help = 0;
unsigned char helpText[] = "Goal: Get out of the labirynth, and find the open cube!\n\nControls\n-----\nUse the WASD keys to move.\nWith mouse you can look around.\n\n+: Increase lighting intensity\n-: Decrease lighting intensity\nF1: Toggle help\nEsc: Exit game";

int draw_win = 0;
unsigned char win_text[] = "You escaped the labirynth!\nYou can quit with Esc key";

void update_win_var()
{
    draw_win = 1;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    set_view(&camera);
    draw_scene(&scene);

    glPopMatrix();

    if (draw_help)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_FOG);
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glRasterPos2f(-0.99f, 0.9f);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, helpText);
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }

    if (draw_win)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_FOG);
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glRasterPos2f(-0.1f, 0.7f);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, win_text);
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO)
    {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else
    {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (GLdouble)width / (GLdouble)height, 0.01, 10000.0);
}

void motion(int x, int y)
{
    rotate_camera(&camera, x * 0.85, y * 0.85);
    glutPostRedisplay();
}

void keyboard_special(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F1:
        draw_help = !draw_help;
        break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    float position;
    const float speed = 2.5;

    switch (key)
    {
    case 'w':
        set_camera_speed(&camera, speed);
        break;
    case 's':
        set_camera_speed(&camera, -speed);
        break;
    case 'a':
        set_camera_side_speed(&camera, speed);
        break;
    case 'd':
        set_camera_side_speed(&camera, -speed);
        break;
    case '+':
        set_lighting(0.2, 0.2, 0.2, 0);
        break;
    case '-':
        set_lighting(-0.2, -0.2, -0.2, 0);
        break;
    case 27: // Escape
        exit(0);
        break;
    }

    glutPostRedisplay();
}

void keyboard_up(unsigned char key, int x, int y)
{
    float position;

    switch (key)
    {
    case 'w':
    case 's':
        set_camera_speed(&camera, 0.0);
        break;
    case 'a':
    case 'd':
        set_camera_side_speed(&camera, 0.0);
        break;
    }

    glutPostRedisplay();
}

void idle()
{
    static int last_frame_time = 0;
    int current_time;
    double elapsed_time;

    current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = (double)(current_time - last_frame_time) / 1000;
    last_frame_time = current_time;

    update_camera(&camera, elapsed_time);

    glutPostRedisplay();
}
