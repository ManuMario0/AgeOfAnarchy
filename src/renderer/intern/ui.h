//
//  ui.h
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 04/03/2023.
//

#ifndef ui_h
#define ui_h

#include <stdio.h>

#include "renderer.h"
#include "cglm/cglm.h"

typedef struct {
    VkRect2D    rect;
    vec4        color;
    vec4        borderColor;
} Rectangle;

typedef struct {
    Model       *model;
    
    int         rectCount;
    Rectangle   *rect;
} UI;

UI *createUI(Window *window);
void destroyUI(Window *window, UI *ui);
Rectangle *addRectangle(UI *ui, int x, int y, int width, int height, vec4 color, vec4 borderColor);
void recordUICommandBuffer(UI *ui, Window *window, VkCommandBuffer buffer);

#endif /* ui_h */
