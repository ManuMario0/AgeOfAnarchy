//
//  player.h
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 17/03/2023.
//

#ifndef player_h
#define player_h

typedef struct {
    int     gold; // just an example for now
} Player;

#include "window.h"

Player *createPlayer(int gold);

#endif /* player_h */
