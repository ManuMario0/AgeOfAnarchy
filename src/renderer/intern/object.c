//
//  object.c
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 25/02/2023.
//

#include <stdlib.h>
#include <string.h>

#include "object.h"

Object *createObject(int model, vec3 pos, vec3 orientation) {
    Object *obj = malloc(sizeof(Object));
    obj->model = model;
    memcpy(obj->pos, pos, sizeof(vec3));
    memcpy(obj->orientation, orientation, sizeof(vec3));
    return obj;
}

void destroyObject(Object *obj) {
    free(obj);
}

float *getObjectPos(Object *obj) {
    return obj->pos;
}

void placeObject(Object *obj, vec3 pos) {
    memcpy(obj->pos, pos, sizeof(vec3));
}

void moveObject(Object *obj, vec3 displacment) {
    glm_vec3_add(obj->pos, displacment, obj->pos);
}

void rotateObject(Object *obj, vec3 rotation) {
    
}

void getObjectMatrix(Object *obj, mat4 dest) {
    mat4 _dest =  GLM_MAT4_IDENTITY_INIT;
    glm_rotate(_dest, obj->orientation[0], (vec3){1.f, 0.f, 0.f});
    glm_rotate(_dest, obj->orientation[1], (vec3){0.f, 1.f, 0.f});
    glm_rotate(_dest, obj->orientation[2], (vec3){0.f, 0.f, 1.f});
    glm_translate(_dest, obj->pos);
    memcpy(dest, _dest, sizeof(mat4));
}
