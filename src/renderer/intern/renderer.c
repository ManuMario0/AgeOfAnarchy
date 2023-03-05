//
//  renderer.c
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 06/02/2023.
//

#include "renderer.h"
#include "headers/common.h"
#include "wavefront.h"

#define CGLM_FORCE_LEFT_HANDED
#define CGLM_FORCE_DEPTH_ZERO_TO_ONE
#include "cglm/cglm.h"

#include <stdlib.h>
#include <string.h>

//-----------------------------------------------//
//-     Local functions                         -//
//-----------------------------------------------//

static void print_error(char str[]);
static void print_warning(char str[]);

//-----------------------------------------------//
//-     Implementation                          -//
//-----------------------------------------------//

Renderer *createRenderer(void) {
    Renderer *renderer = malloc(sizeof(Renderer));
    memset(renderer, 0, sizeof(Renderer));
    renderer->window = createWindow();
    if (!renderer->window) {
        print_error("unabled to create the renderer");
        free(renderer);
        return NULL;
    }
    glfwSetWindowUserPointer(renderer->window->glfwWindow, renderer);
    selectPhysicalDevice(renderer->window);
    createLogicalDevice(renderer->window);
    setupSwapchain(renderer->window);
    createImageViews(renderer->window);
    createDepthResources(renderer->window);
    createRenderpass(renderer->window);
    setupPipeline(renderer->window);
    createFramebuffers(renderer->window);
    
    glm_perspective_default((float)renderer->window->swapchainSize.width/(float)renderer->window->swapchainSize.height, renderer->proj);
    
    renderer->cam = createCamera((vec3){30.f, 0.f, 0.f});
    
    createCommandPool(renderer->window, renderer->window->graphicQueueFamily, &renderer->renderPool);
    
    createCommand(renderer->window, renderer->renderPool, &renderer->commands[0]);
    createCommand(renderer->window, renderer->renderPool, &renderer->commands[1]);
    createSync(renderer->window);
    
    renderer->ui = createUI(renderer->window);
    
    return renderer;
}

void destroyRenderer(Renderer *renderer) {
    if (!renderer) {
        return;
    }
    vkDeviceWaitIdle(renderer->window->device);
    destroyUI(renderer->window, renderer->ui);
    destroyCommandPool(renderer->window, renderer->renderPool);
    for (int i=0; i<renderer->objectCount; i++) {
        destroyObject(renderer->objects[i]);
    }
    for (int i=0; i<renderer->modelCount; i++) {
        destroyModel(renderer->window, renderer->models[i]);
    }
    destroyWindow(renderer->window);
    free(renderer);
}

GLFWwindow *getGLFWWindow(Renderer *renderer) {
    return renderer->window->glfwWindow;
}

int windowShouldClose(Renderer *renderer) {
    return glfwWindowShouldClose(renderer->window->glfwWindow);
}

// TODO: move this to vertex.c and improve vert structure
int recordCommandBuffer(Renderer *renderer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo;
    memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(renderer->commands[renderer->window->currentFrame], &beginInfo) != VK_SUCCESS) {
        print_error("unabled to begin command recording");
        return AOA_FALSE;
    }
    
    VkRenderPassBeginInfo renderPassInfo;
    memset(&renderPassInfo, 0, sizeof(VkRenderPassBeginInfo));
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderer->window->renderPass;
    renderPassInfo.framebuffer = renderer->window->framebuffers[renderer->window->currentFrame];
    VkOffset2D offset = {0, 0};
    VkExtent2D extent = renderer->window->swapchainSize;
    renderPassInfo.renderArea.offset = offset;
    renderPassInfo.renderArea.extent = extent;
    
    VkClearValue clearVal = {0.3f, 0.3f, 0.3f, 1.f};
    VkClearValue depthVal = {1.0f, 0};
    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = (VkClearValue[2]){clearVal, depthVal};
    
    vkCmdBeginRenderPass(renderer->commands[renderer->window->currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    vkCmdBindPipeline(renderer->commands[renderer->window->currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->window->pipeline);
    
    VkViewport viewport;
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = (float)renderer->window->swapchainSize.width;
    viewport.height = (float)renderer->window->swapchainSize.height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;
    vkCmdSetViewport(renderer->commands[renderer->window->currentFrame], 0, 1, &viewport);
    
    VkRect2D scissor;
    scissor.offset = offset;
    scissor.extent = extent;
    vkCmdSetScissor(renderer->commands[renderer->window->currentFrame], 0, 1, &scissor);
    
    for (int i=0; i<renderer->modelCount; i++) {
        Model *model = renderer->models[i];
        
    }
    
    for (int i=0; i<renderer->objectCount; i++) {
        Object  *obj = renderer->objects[i];
        Model *model = renderer->models[obj->model];
        
        VkDeviceSize bufferOffset = 0;
        vkCmdBindVertexBuffers(renderer->commands[renderer->window->currentFrame], 0, 1, &model->buffer->vertexBuffer.buffer, &bufferOffset);
        vkCmdBindIndexBuffer(renderer->commands[renderer->window->currentFrame], model->buffer->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
        
        mat4 view;
        mat4 object;
        mat4 matTemp;
        mat4 transform;
        
        getCameraMatrix(renderer->cam, view);
        
        getObjectMatrix(obj, object);
        glm_mat4_mul(view, object, matTemp);
        glm_mat4_mul(renderer->proj, matTemp, transform);
        
        vkCmdPushConstants(renderer->commands[renderer->window->currentFrame], renderer->window->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mat4), transform);
        
        vkCmdPushConstants(renderer->commands[renderer->window->currentFrame], renderer->window->layout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(mat4), sizeof(vec4), model->color);
        
        vkCmdDrawIndexed(renderer->commands[renderer->window->currentFrame], model->indexCount, 1, 0, 0, 0);
    }
    
    recordUICommandBuffer(renderer->ui, renderer->window, renderer->commands[renderer->window->currentFrame]);
    
    vkCmdEndRenderPass(renderer->commands[renderer->window->currentFrame]);
    
    if (vkEndCommandBuffer(renderer->commands[renderer->window->currentFrame]) != VK_SUCCESS) {
        print_error("unabled to end command recording");
        return AOA_FALSE;
    }
    
    return AOA_TRUE;
}

int drawFrame(Renderer *renderer) {
    vkWaitForFences(renderer->window->device, 1, &renderer->window->inFlightFences[renderer->window->currentFrame], VK_TRUE, UINT64_MAX);
    
    uint32_t imageIndex;
    if (vkAcquireNextImageKHR(renderer->window->device, renderer->window->swapchain, UINT64_MAX, renderer->window->imageAvailableSemaphores[renderer->window->currentFrame], VK_NULL_HANDLE, &imageIndex) != VK_SUCCESS) {
        print_error("unabled to acquire next image");
        return AOA_FALSE;
    }
    
    vkResetFences(renderer->window->device, 1, &renderer->window->inFlightFences[renderer->window->currentFrame]);
    
    vkResetCommandBuffer(renderer->commands[renderer->window->currentFrame], 0);
    recordCommandBuffer(renderer, imageIndex);
    
    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &renderer->window->imageAvailableSemaphores[renderer->window->currentFrame];
    VkPipelineStageFlags waitFlag = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submitInfo.pWaitDstStageMask = &waitFlag;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderer->commands[renderer->window->currentFrame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderer->window->renderFinishedSemaphores[renderer->window->currentFrame];
    
    if (vkQueueSubmit(renderer->window->graphicQueueFamily.queues[0], 1, &submitInfo, renderer->window->inFlightFences[renderer->window->currentFrame]) != VK_SUCCESS) {
        print_error("unabled to run the commands");
        return AOA_FALSE;
    }
    
    VkPresentInfoKHR presentInfo;
    memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderer->window->renderFinishedSemaphores[renderer->window->currentFrame];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &renderer->window->swapchain;
    presentInfo.pImageIndices = &imageIndex;
    
    if (vkQueuePresentKHR(renderer->window->graphicQueueFamily.queues[0], &presentInfo) != VK_SUCCESS) {
        print_error("unabled to present to the window");
        return AOA_FALSE;
    }
    
    renderer->window->currentFrame = (renderer->window->currentFrame+1) % MAX_FRAME_IN_FLIGHT;
    
    return AOA_TRUE;
}

int addModel(Renderer *renderer, char filename[], vec4 color) {
    float *vertices;
    uint16_t *indices;
    int indicesSize;
    int verticesSize;
    acquireData(&vertices, &indices, &verticesSize, &indicesSize, filename);
    Model *model = createModel(renderer->window, verticesSize, vertices, indicesSize*sizeof(uint16_t), indices);
    free(vertices);
    free(indices);
    setModelColor(model, color);
    
    renderer->models = realloc(renderer->models, (renderer->modelCount+1)*sizeof(Model*));
    renderer->models[renderer->modelCount] = model;
    renderer->modelCount += 1;
    return renderer->modelCount - 1;
}

Object *addObject(Renderer *renderer, int model, vec3 pos, vec3 orientation) {
    Object *obj = createObject(model, pos, orientation);
    
    renderer->objects = realloc(renderer->objects, (renderer->objectCount+1)*sizeof(Object*));
    renderer->objects[renderer->objectCount] = obj;
    renderer->objectCount++;
    return obj;
}

void moveGlobalCamera(Renderer *renderer, vec3 displacment) {
    moveCamera(renderer->cam, displacment);
}

float *getGlobalCameraPosition(Renderer *renderer) {
    return renderer->cam->pos;
}

float *getGlobalCameraUp(Renderer *renderer) {
    return renderer->cam->up;
}

UI *getUI(Renderer *renderer) {
    return renderer->ui;
}

//-----------------------------------------------//
//-     Local functions                         -//
//-----------------------------------------------//

void print_error(char str[]) {
    fprintf(stderr, "[ \033[31mERROR\033[29m ] : initialization : %s\n", str);
}

void print_warning(char str[]) {
#ifdef DEBUG
    fprintf(stderr, "[ \033[32mWARNING\033[29m ] : initialization : %s\n", str);
#endif
}
