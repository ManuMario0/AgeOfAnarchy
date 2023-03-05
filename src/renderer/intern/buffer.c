//
//  buffer.c
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 16/02/2023.
//

#include "buffer.h"
#include "command_buffer.h"
#include "headers/common.h"

#include <stdio.h>
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

BufferMemory *allocateDeviceMemory(Window *window, size_t size, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    BufferMemory *mem = malloc(sizeof(BufferMemory));
    
    VkMemoryAllocateInfo allocInfo;
    memset(&allocInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = size;
    
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(window->physicalDevice, &memProperties);
    for (int i=0; i<memProperties.memoryTypeCount; i++) {
        if (typeFilter & (i << 1) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            allocInfo.memoryTypeIndex = i;
            break;
        }
    }
    
    mem->size = size;
    mem->offset = 0;

    if (vkAllocateMemory(window->device, &allocInfo, NULL, &mem->memory) != VK_SUCCESS) {
        print_error("unabled to allocate buffer memory");
        free(mem);
        return NULL;
    }
    
    return mem;
}

int freeDeviceMemory(Window *window, BufferMemory *mem) {
    vkFreeMemory(window->device, mem->memory, NULL);
    free(mem);
    return AOA_TRUE;
}

int createBuffer(Window *window, size_t size, VkBufferUsageFlags usage, Buffer *buffer) {
    VkBufferCreateInfo bufferInfo;
    memset(&bufferInfo, 0, sizeof(VkBufferCreateInfo));
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
    bufferInfo.queueFamilyIndexCount = 2;
    uint32_t indices[] = {window->transferQueueFamily.family, window->graphicQueueFamily.family};
    bufferInfo.pQueueFamilyIndices = indices;
    
    if (vkCreateBuffer(window->device, &bufferInfo, NULL, &buffer->buffer) != VK_SUCCESS) {
        print_error("unabled to create the buffer");
        return AOA_FALSE;
    }
    
    buffer->size = size;
    
    return AOA_TRUE;
}

int destroyBuffer(Window *window, Buffer buffer) {
    vkDestroyBuffer(window->device, buffer.buffer, NULL);
    return AOA_TRUE;
}

int bindBuffer(Window *window, Buffer buffer, BufferMemory *mem) {
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(window->device, buffer.buffer, &memRequirements);
    
    if (mem->size - mem->offset < memRequirements.size) {
        print_error("unabled to bind the buffer : ran out of memory");
        return AOA_FALSE;
    }
    /*
    if ((mem->properties | memRequirements.memoryTypeBits) != mem->properties) {
        print_error("not the right type of memory for this buffer");
        return AOA_FALSE;
    }*/
    
    vkBindBufferMemory(window->device, buffer.buffer, mem->memory, mem->offset);
    mem->offset += memRequirements.size;
    
    return AOA_TRUE;
}

int copyBuffer(Window *window, QueueFamily *family, VkCommandPool pool, VkBuffer srcBuffer, VkBuffer dstBuffer, size_t size) {
    VkCommandBuffer commandBuffer;
    createCommand(window, pool, &commandBuffer);
    
    VkCommandBufferBeginInfo beginInfo;
    memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        print_error("unabled to record buffer");
        return AOA_FALSE;
    }
    
    VkBufferCopy copyRegion;
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        print_error("unabled to record buffer");
        return AOA_FALSE;
    }
    
    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    
    vkQueueSubmit(family->queues[0], 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(family->queues[0]);
    
    vkFreeCommandBuffers(window->device, pool, 1, &commandBuffer);
    
    return AOA_TRUE;
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
