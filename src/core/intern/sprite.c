//
//  sprite.c
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 10/03/2023.
//

#include <string.h>

#include "sprite.h"

// TODO: orientation can be given using only an angle since we already know the front vec of the object
Sprite *createSprite(Renderer *renderer, int model, vec3 front, vec3 pos, vec3 orientation) {
    Sprite *sprite = malloc(sizeof(Sprite));
    sprite->obj = addObject(renderer, model, pos, orientation);
    memcpy(sprite->target, pos, sizeof(vec3));
    memcpy(sprite->front, front, sizeof(vec3));
    sprite->isSelected = 0;
    
    return sprite;
}

// TODO: path finding on the world map (but latter since I don't know for now the format of the map nor the algorithms)
void updateSpritePosition(Sprite *sprite) {
    return;
}

void changeSpriteTarget(Sprite *sprite, vec3 target) {
    memcpy(sprite->target, target, sizeof(vec3));
}

void changeSpritePosition(Sprite *sprite, vec3 pos) {
    placeObject(sprite->obj, pos);
}

void selectSprite(Sprite *sprite, uint8_t isSelected) {
    sprite->isSelected = isSelected;
    outlineObject(sprite->obj, isSelected);
}
