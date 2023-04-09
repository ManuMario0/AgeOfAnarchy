//
//  world.h
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 10/03/2023.
//

#ifndef world_h
#define world_h

#include <stdio.h>

#include "window.h"

typedef struct {
    Model       sphere;
    Object *    planet;
} World;

World *loadWorld(Renderer *renderer);

#endif /* world_h */
