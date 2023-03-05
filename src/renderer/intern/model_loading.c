//
//  model_loading.c
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 19/02/2023.
//

#include "model_loading.h"

#include <stdlib.h>
#include <string.h>

ModelBuffer *createModelBuffer(Window *window, size_t vertexSize, void *vertices, size_t indexSize, void *indices) {
    ModelBuffer *model = malloc(sizeof(ModelBuffer));
    
    VkCommandPoolCreateInfo poolInfo;
    memset(&poolInfo, 0, sizeof(VkCommandPoolCreateInfo));
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = window->transferQueueFamily.family;
    VkCommandPool pool;
    vkCreateCommandPool(window->device, &poolInfo, NULL, &pool);
    
    Buffer stagingBuffer;
    createBuffer(window, vertexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &stagingBuffer);
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(window->device, stagingBuffer.buffer, &memRequirements);
    BufferMemory *stagingBufferMem =  allocateDeviceMemory(window, memRequirements.size,
                                                          memRequirements.memoryTypeBits,
                                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                                          | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    bindBuffer(window, stagingBuffer, stagingBufferMem);
    
    void* data;
    vkMapMemory(window->device, stagingBufferMem->memory, 0, vertexSize, 0, &data);
        memcpy(data, vertices, vertexSize);
    vkUnmapMemory(window->device, stagingBufferMem->memory);
    
    createBuffer(window, vertexSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &model->vertexBuffer);
    vkGetBufferMemoryRequirements(window->device, model->vertexBuffer.buffer, &memRequirements);
    model->vertexBufferMemory = allocateDeviceMemory(window, memRequirements.size,
                                                        memRequirements.memoryTypeBits,
                                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    bindBuffer(window, model->vertexBuffer, model->vertexBufferMemory);
    
    copyBuffer(window, &window->transferQueueFamily, pool, stagingBuffer.buffer, model->vertexBuffer.buffer, vertexSize);
    
    destroyBuffer(window, stagingBuffer);
    freeDeviceMemory(window, stagingBufferMem);
    
    
    
    createBuffer(window, indexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &stagingBuffer);
    vkGetBufferMemoryRequirements(window->device, stagingBuffer.buffer, &memRequirements);
    stagingBufferMem =  allocateDeviceMemory(window, memRequirements.size,
                                             memRequirements.memoryTypeBits,
                                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                             | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    bindBuffer(window, stagingBuffer, stagingBufferMem);
    
    vkMapMemory(window->device, stagingBufferMem->memory, 0, indexSize, 0, &data);
        memcpy(data, indices, indexSize);
    vkUnmapMemory(window->device, stagingBufferMem->memory);
    
    createBuffer(window, indexSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &model->indexBuffer);
    vkGetBufferMemoryRequirements(window->device, model->vertexBuffer.buffer, &memRequirements);
    model->indexBufferMemory = allocateDeviceMemory(window, memRequirements.size,
                                                        memRequirements.memoryTypeBits,
                                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    bindBuffer(window, model->indexBuffer, model->indexBufferMemory);
    
    copyBuffer(window, &window->transferQueueFamily, pool, stagingBuffer.buffer, model->indexBuffer.buffer, indexSize);
    
    destroyBuffer(window, stagingBuffer);
    freeDeviceMemory(window, stagingBufferMem);
    
    vkDestroyCommandPool(window->device, pool, NULL);
    
    return model;
}

void destroyModelBuffer(Window *window, ModelBuffer *model) {
    destroyBuffer(window, model->vertexBuffer);
    destroyBuffer(window, model->indexBuffer);
    freeDeviceMemory(window, model->vertexBufferMemory);
    freeDeviceMemory(window, model->indexBufferMemory);
}
