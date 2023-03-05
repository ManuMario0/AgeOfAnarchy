//
//  model.c
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 22/02/2023.
//

#include <string.h>

#include "model.h"

Model *createModel(Window *window, size_t vertexSize, void *vertices, size_t indexSize, void *indices) {
    Model *model = malloc(sizeof(Model));
    model->buffer = createModelBuffer(window, vertexSize, vertices, indexSize, indices);
    model->indexCount = (uint32_t) (indexSize / sizeof(uint16_t));
    return model;
}

void destroyModel(Window *window, Model *model) {
    destroyModelBuffer(window, model->buffer);
    free(model);
}

void setModelColor(Model *model, vec4 color) {
    memcpy(model->color, color, sizeof(vec4));
}
