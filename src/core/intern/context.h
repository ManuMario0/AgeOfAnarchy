//
//  context.h
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 17/03/2023.
//

#ifndef context_h
#define context_h

#include "window.h"
#include "world.h"
#include "player.h"

typedef struct {
    Renderer *  renderer;
    
    Player *    player;
    Font *      defaultFont;
    Text *      gold;
    
    World *     world;
} Context;

Context *createContext(Renderer *renderer);
void destroyContext(Context *context);

#endif /* context_h */
