#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "model.h"

typedef struct Scene
{
    Camera camera;
    Model apple;
    Material material;
    GLuint texture_id_ground;
    GLuint texture_id_wall;
    GLuint texture_id_sky;
    GLuint texture_id_apple;
} Scene;

/* Scene initialization */
void init_scene(Scene *scene);

float threshold_value(float val);

/**
 * Set the lighting of the scene.
 */
void set_lighting(float r, float g, float b, float a);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Draw the scene objects.
 */
void draw_scene(const Scene *scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

void draw_sky(const Scene* scene);

void draw_wall(const Scene* scene, float height, float width);

void draw_labirynth(const Scene* scene);

void draw_floor(const Scene *scene);

#endif /* SCENE_H */
