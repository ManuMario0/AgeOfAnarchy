//
//  loading.c
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 10/03/2023.
//

#include "loading.h"
#include "world.h"

void loadResources(Context *context) {
    context->world = loadWorld(context->renderer);
}
