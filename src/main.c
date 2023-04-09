/*
 * main.c
 * This file is part of Age of Anarchy
 *
 * Copyright (C) 2021 Mera Emmanuel <emmanuel.mera@live.fr>
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

// std library

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// modules

#include "renderer/headers/window.h"
#include "core/headers/core.h"
#include "memory/MEM_alloc.h"

// lib

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main(int argc, char *argv[]) {
    MEM_use_secured_allocator();
    
    Renderer *renderer = createRenderer();
    
    MEM_print_memstats();
    
    launchTitleScreen(renderer);
    
    MEM_print_memstats();
    
    // printVulkanMemStat();
    
    destroyRenderer(renderer);
    
    MEM_print_memstats();
    
    return 0;
}
