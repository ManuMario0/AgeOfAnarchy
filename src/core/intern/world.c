//
//  world.c
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 10/03/2023.
//

#include "world.h"

World *loadWorld(Renderer *renderer) {
    World *world = malloc(sizeof(World));
    world->sphere = addModel(renderer, "../Resources/sphere.3d", (vec4){1.f, 1.f, 6.f, 1.0f});
    world->planet = addObject(renderer, world->sphere, (vec3){0.f, 0.f, 0.f}, (vec3)GLM_VEC3_ZERO_INIT);
    return world;
}

void updateWorldState(Renderer *renderer) {
    
}
