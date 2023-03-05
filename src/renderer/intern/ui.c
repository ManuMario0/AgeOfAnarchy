//
//  ui.c
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 04/03/2023.
//

#include <string.h>

#include "ui.h"

//-----------------------------------------------//
//-     Local functions                         -//
//-----------------------------------------------//

static uint32_t *get_source_code(const char *file, size_t *size);

//-----------------------------------------------//
//-     Implementation                          -//
//-----------------------------------------------//

UI *createUI(Window *window) {
    UI *ui = malloc(sizeof(UI));
    memset(ui, 0, sizeof(UI));
    
    float vert[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };
    uint16_t indices[] = {
        0, 1, 2, 1, 3, 2
    };
    ui->model = createModel(window, sizeof(vert), vert, sizeof(indices), indices);
    
    return ui;
}

void destroyUI(Window *window, UI *ui) {
    destroyModel(window, ui->model);
    free(ui);
}

Rectangle *addRectangle(UI *ui, int x, int y, int width, int height, vec4 color, vec4 borderColor) {
    ui->rect = realloc(ui->rect, (ui->rectCount+1)*sizeof(Rectangle));
    ui->rect[ui->rectCount].rect.offset.x = x;
    ui->rect[ui->rectCount].rect.offset.y = y;
    ui->rect[ui->rectCount].rect.extent.width = width;
    ui->rect[ui->rectCount].rect.extent.height = height;
    memcpy(ui->rect[ui->rectCount].color, color, sizeof(vec4));
    memcpy(ui->rect[ui->rectCount].borderColor, borderColor, sizeof(vec4));
    ui->rectCount++;
    return &ui->rect[ui->rectCount-1];
}

void recordUICommandBuffer(UI *ui, Window *window, VkCommandBuffer buffer) {
    VkDeviceSize bufferOffset = 0;
    vkCmdBindVertexBuffers(buffer, 0, 1, &ui->model->buffer->vertexBuffer.buffer, &bufferOffset);
    vkCmdBindIndexBuffer(buffer, ui->model->buffer->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
    for (int i=0; i<ui->rectCount; i++) {
        mat4 transform = GLM_MAT4_IDENTITY_INIT;
        
        glm_scale(transform, (vec3){(float)ui->rect[i].rect.extent.width/(float)window->swapchainSize.width, 1.0f, 1.0f});
        glm_scale(transform, (vec3){1.0f, (float)ui->rect[i].rect.extent.height/(float)window->swapchainSize.height, 1.0f});
        
        transform[3][0] = (float)ui->rect[i].rect.offset.x/(float)window->swapchainSize.width - 1. + (float)ui->rect[i].rect.extent.width/(float)window->swapchainSize.width;
        transform[3][1] = (float)ui->rect[i].rect.offset.y/(float)window->swapchainSize.height - 1. + (float)ui->rect[i].rect.extent.height/(float)window->swapchainSize.height;
        
        vkCmdPushConstants(buffer, window->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mat4), transform);
        vkCmdPushConstants(buffer, window->layout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(mat4), sizeof(vec4), ui->rect[i].color);
        
        vkCmdDrawIndexed(buffer, ui->model->indexCount, 1, 0, 0, 0);
    }
}

void resetRectangle(Rectangle *rect, int x, int y, int width, int height, vec4 color, vec4 borderColor) {
    rect->rect.offset.x = x;
    rect->rect.offset.y = y;
    rect->rect.extent.width = width;
    rect->rect.extent.height = height;
    memcpy(rect->color, color, sizeof(vec4));
    memcpy(rect->borderColor, borderColor, sizeof(vec4));
}

void resizeRectangle(Rectangle *rect, int width, int height) {
    rect->rect.extent.width = width;
    rect->rect.extent.height = height;
}

void placeRectangle(Rectangle *rect, int x, int y) {
    rect->rect.offset.x = x;
    rect->rect.offset.y = y;
}

void addText(void) {
    
}

void addDynamicText(void) {
    
}

//-----------------------------------------------//
//-     Local functions                         -//
//-----------------------------------------------//

uint32_t *get_source_code(const char *file, size_t *size) {
    FILE *f = fopen(file, "rb");
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    uint32_t *content = calloc(*size, sizeof(char));
    fread(content, 1, *size, f);
    
    fclose(f);
    return content;
}
