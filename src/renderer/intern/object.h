//
//  object.h
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 25/02/2023.
//

#ifndef object_h
#define object_h

#include <stdio.h>

#include "cglm/cglm.h"

/*
 Reminder: add several resolutions options and chunk systems for optimized rendering
 */
typedef struct {
    int     model;
    
    uint8_t    outline;
    
    vec3    pos;
    vec3    orientation;
} Object;

Object *createObject(int model, vec3 pos, vec3 orientation);
void outlineObject(Object *obj, uint8_t b);
void destroyObject(Object *obj);
void getObjectMatrix(Object *obj, mat4 dest);

#endif /* object_h */
