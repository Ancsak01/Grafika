#include "camera.h"
#include "callbacks.h"

#include <GL/glut.h>

#include <math.h>

void init_camera(Camera *camera)
{
    camera->realPos.x = 0;
    camera->realPos.y = 0;
    camera->realPos.z = 1.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
    camera->rotation_speed.x = 0.0;
    camera->rotation_speed.y = 0.0;
    camera->rotation_speed.z = 0.0;

    camera->position.x = 0;
    camera->position.y = 0;
    camera->position.z = 1.0;

    is_preview_visible = FALSE;
}

void update_camera(Camera *camera, double time)
{
    double angle;
    double side_angle;

    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    vec3 newPos;
    newPos.x = camera->realPos.x + cos(angle) * camera->speed.y * time;
    newPos.y = camera->realPos.y + sin(angle) * camera->speed.y * time;
    newPos.x += cos(side_angle) * camera->speed.x * time;
    newPos.y += sin(side_angle) * camera->speed.x * time;

    newPos.z += camera->realPos.z + camera->speed.z * time;

    newPos.z += camera->realPos.z + camera->rotation_speed.z * time;
    if (check_collisions(newPos) == 0)
    {
        camera->realPos.x = newPos.x;
        camera->realPos.y = newPos.y;
    }

    camera->position.x = (camera->realPos.x);
    camera->position.y = (camera->realPos.y);
}

void set_view(const Camera *camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera *camera, double horizontal, double vertical)
{
    camera->rotation.z = -horizontal;
    camera->rotation.x = -vertical + 90;

    if (camera->rotation.z < 0)
    {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0)
    {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x > -270.0)
    {
        camera->rotation.x = -270.0;
    }

    if (camera->rotation.x < -450.0)
    {
        camera->rotation.x = -450.0;
    }
}

void set_camera_speed(Camera *camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera *camera, double speed)
{
    camera->speed.x = speed;
}

void set_camera_horizontal_rotation_speed(Camera *camera, double rot_speed)
{
    camera->rotation_speed.z = rot_speed;
}

void set_camera_vertical_speed(Camera *camera, double speed)
{
    camera->speed.z = speed;
}

int check_collisions(vec3 newPosition)
{
    /* --- INNER CIRCLE COLLISION DETECTION --- */

    /*
        Inner circle - nyílástól balra levő fal
    */
    if (calc_collision(newPosition, 0, -2.9, 2.9, 0.1))
    {
        return 1;
    }

    /*
        Inner circle - nyílásos fal
    */
    if (calc_collision(newPosition, -2.9, 0 + 0.7, 0.1, 2.9 - 0.7))
    {
        return 1;
    }

    /*
        Inner circle - nyílástól jobbra levő fal
    */
    if (calc_collision(newPosition, 0, 2.9, 2.9, 0.1))
    {
        return 1;
    }

    /*
        Inner circle - nyílástól szembe levő fal
    */
    if (calc_collision(newPosition, 2.9, 0, 0.1, 2.9))
    {
        return 1;
    }

    /* --- OUTER LVL3 CIRCLE COLLISION DETECTION --- */

    /*
        Outer lvl3 circle - nyílástól balra levő fal
    */
    if (calc_collision(newPosition, -4.4, 0, 0.1, 4.4))
    {
        return 1;
    }

    /*
        Outer lvl3 circle - nyílásos fal
    */
    if (calc_collision(newPosition, 0 + 0.7, 4.4, 4.4 - 0.7, 0.1))
    {
        return 1;
    }

    /*
        Outer lvl3 circle - nyílástól jobbra levő fal
    */
    if (calc_collision(newPosition, 4.4, 0, 0.1, 4.4))
    {
        return 1;
    }

    /*
        Outer lvl3 circle - nyílástól szembe levő fal
    */
    if (calc_collision(newPosition, 0, -4.4, 4.4, 0.1))
    {
        return 1;
    }

    /* --- OUTER LVL2 CIRCLE COLLISION DETECTION --- */

    /*
        Outer lvl2 circle - nyílástól balra levő fal
    */
    if (calc_collision(newPosition, 5.9, 0, 0.1, 5.9))
    {
        return 1;
    }

    /*
        Outer lvl2 circle - nyílásos fal
    */
    if (calc_collision(newPosition, 0 - 0.7, -5.9, 5.9 - 0.7, 0.1))
    {
        return 1;
    }

    /*
        Outer lvl2 circle - nyílástól jobbra levő fal
    */
    if (calc_collision(newPosition, -5.9, 0, 0.1, 5.9))
    {
        return 1;
    }

    /*
        Outer lvl2 circle - nyílástól szembe levő fal
    */
    if (calc_collision(newPosition, 0, 5.9, 5.9, 0.1))
    {
        return 1;
    }

    /* --- OUTER LVL1 CIRCLE COLLISION DETECTION --- */

    /*
        Outer lvl1 circle - nyílástól balra levő fal
    */
    if (calc_collision(newPosition, 0, 7.4, 7.4, 0.1))
    {
        return 1;
    }

    /*
        Outer lvl1 circle - nyílásos fal
    */
    if (calc_collision(newPosition, 7.4, 0 - 0.7, 0.1, 7.4 - 0.7))
    {
        return 1;
    }

    /*
        Outer lvl1 circle - nyílástól jobbra levő fal
    */
    if (calc_collision(newPosition, 0, -7.4, 7.4, 0.1))
    {
        return 1;
    }

    /*
        Outer lvl1 circle - nyílástól szembe levő fal
    */
    if (calc_collision(newPosition, -7.4, 0, 0.1, 7.4))
    {
        return 1;
    }

    /*
        WIN
    */
    if (calc_collision(newPosition, 8.6, -3, 0.7, 0.7))
    {
        glutMouseFunc(NULL);
		glutMotionFunc(NULL);
		glutKeyboardUpFunc(NULL);
		glutSpecialFunc(NULL);
        glutIdleFunc(NULL);

        update_win_var();

        return 1;
    }

    /*
        Map edges
   */
    if (calc_collision(newPosition, 0, 9.9, 9.9, 0.1))
    {
        return 1;
    }

    if (calc_collision(newPosition, 9.9, 0, 0.1, 9.9))
    {
        return 1;
    }

    if (calc_collision(newPosition, 0, -9.9, 9.9, 0.1))
    {
        return 1;
    }

    if (calc_collision(newPosition, -9.9, 0, 0.1, 9.9))
    {
        return 1;
    }

    // No collision found
    return 0;
}

int calc_collision(vec3 newPosition, float posX, float posY, float boxSizeX, float boxSizeY)
{

    if ((newPosition.x > posX - boxSizeX) && (newPosition.x < posX + boxSizeX))
        if ((newPosition.y > posY - boxSizeY) && (newPosition.y < posY + boxSizeY))
            return 1;
    return 0;
}