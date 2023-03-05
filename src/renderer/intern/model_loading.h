//
//  model_loading.h
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 19/02/2023.
//

#ifndef model_loading_h
#define model_loading_h

#include "init.h"
#include "buffer.h"
#include "vulkan/vulkan.h"


typedef struct {
    Buffer          vertexBuffer;
    BufferMemory    *vertexBufferMemory;
    Buffer          indexBuffer;
    BufferMemory    *indexBufferMemory;
} ModelBuffer;

ModelBuffer *createModelBuffer(Window *window, size_t vertexSize, void *vertices, size_t indexSize, void *indices);
void destroyModelBuffer(Window *window, ModelBuffer *model);

#endif /* model_loading_h */
