//
//  context.c
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 17/03/2023.
//

#include "context.h"

/* We will put the ui elements directly in the context for now but must move at some points*/
Context *createContext(Renderer *renderer) {
    Context *context = malloc(sizeof(Context));
    context->renderer = renderer;
    
    UI *ui = getUI(renderer);
    
    context->defaultFont = loadTextFont(ui, "../Resources/Helvetica.ttc");
    context->gold = addText(ui, context->defaultFont, "Gold:0", 50, 150, (vec4){0}, 0.5);
    
    return context;
}

void destroyContext(Context *context) {
    UI *ui = getUI(context->renderer);
    
    destroyFont(ui, context->defaultFont);
    free(context);
}

// not sure yet since we'll most likely have only one context, but let it there so I don't forget
void makeContextCurrent(Renderer *renderer) {
    
}
