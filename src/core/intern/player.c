//
//  player.c
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 17/03/2023.
//

#include <stdlib.h>

#include "context.h"
#include "player.h"

Player *createPlayer(int gold) {
    Player *player = malloc(sizeof(Player));
    player->gold = gold;
    return player;
}

void makePlayerCurrent(Context *context, Player *player) {
    context->player = player;
}
