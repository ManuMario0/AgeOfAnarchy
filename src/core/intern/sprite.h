//
//  sprite.h
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 10/03/2023.
//

#ifndef sprite_h
#define sprite_h

#include <stdio.h>

#include "cglm/cglm.h"

#include "window.h"

typedef struct {
    Object      *obj;
    
    uint8_t     isSelected;
    vec3        target;
    vec3        front;
} Sprite;

Sprite *createSprite(Renderer *renderer, int model, vec3 front, vec3 pos, vec3 orientation);

#endif /* sprite_h */
