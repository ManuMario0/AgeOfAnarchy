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

// lib

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

double xpos, ypos, rxpos, rypos;
Object *obj;
Rectangle *rect;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Renderer *renderer = glfwGetWindowUserPointer(window);
    float shift = tanh(yoffset)/10.f+1.f;
    moveGlobalCamera(renderer, (vec3){shift, 0.f, 0.f});
}

static void cursor_position_callback(GLFWwindow* window, double nxpos, double nypos) {
    Renderer *renderer = glfwGetWindowUserPointer(window);
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (state == GLFW_PRESS | state == GLFW_REPEAT)
        moveGlobalCamera(renderer, (vec3){1.f, (nypos-ypos)/50.f, (nxpos-xpos)/50.f});
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        resizeRectangle(rect, 2*((int)xpos-rxpos), 2*((int)ypos-rypos));
    xpos = nxpos;
    ypos = nypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Renderer *renderer = glfwGetWindowUserPointer(window);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        rxpos = xpos;
        rypos = ypos;
        placeRectangle(rect, 4*(int)xpos, 4*(int)ypos); // don't know why there must be a coef here but hey
        resizeRectangle(rect, 0, 0);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        placeRectangle(rect, -5, -5);
        resizeRectangle(rect, 1, 1);
    }
    /*
    vec3 pos;
    vec4 npos;
    memcpy(pos, getGlobalCameraPosition(renderer), sizeof(vec3));
    vec3 up;
    memcpy(up, getGlobalCameraUp(renderer), sizeof(vec3));
    mat4 temp = GLM_MAT4_IDENTITY_INIT;
    vec3 right;
    glm_vec3_cross(pos, up, right);
    
    double _xpos = xpos;
    double _ypos = ypos;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double yh = (_xpos - (double)width/2.)/(double)height * 2. * tan(M_PI/8.);
        double xh = (_ypos - (double)height/2.)/(double)height * 2. * tan(M_PI/8.);
        
        vec3 relObjPos;
        double n = sqrt(pow(yh, 2.) + pow(xh, 2.));
        
        double d = glm_vec3_norm(pos);
        double angle = M_PI - atan(n) + asin(d*sin(atan(n))/10.01);
        
        vec3 axe;
        glm_normalize(right);
        glm_vec3_scale(right, yh, right);
        glm_vec3_scale(up, -xh, up);
        glm_vec3_add(right, up, relObjPos);
        glm_normalize(pos);
        glm_vec3_scale(pos, -1., pos);
        glm_vec3_add(relObjPos, pos, relObjPos);
        glm_vec3_cross(relObjPos, pos, axe);
        glm_normalize(axe);
        glm_rotate_at(temp, (vec3){0.f, 0.f, 0.f}, -angle, axe);
        glm_mat4_mulv(temp, (vec4){pos[0], pos[1], pos[2], 1.}, npos);
        glm_vec3_scale(npos, 10.01/glm_vec3_norm(npos), npos);
        placeObject(obj, npos);
    }*/
}

int main(int argc, char *argv[]) {
    Renderer *renderer = createRenderer();
    GLFWwindow *window = getGLFWWindow(renderer);
    int model = addModel(renderer, "../Resources/sphere.3d", (vec4){1.f, 1.f, 6.f, 1.0f});
    int cube = addModel(renderer, "../Resources/cube.3d", (vec4){5.f, 1.f, .1f, 1.f});
    Object *sphere = addObject(renderer, model, (vec3){0.f, 0.f, 0.f}, (vec3)GLM_VEC3_ZERO_INIT);
    obj = addObject(renderer, cube, (vec3){10.02f, 0.f, 0.f}, (vec3)GLM_VEC3_ZERO_INIT);
    
    UI *ui = getUI(renderer);
    
    rect = addRectangle(ui, -1, 0, 0, 0, (vec4){.5f, .5f, .5f, .1f}, (vec4){0.0f, 0.0f, 0.0f, 0.0f});
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    glfwSetScrollCallback(window, scroll_callback);
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    while (!windowShouldClose(renderer)) {
        drawFrame(renderer);
        glfwPollEvents();
    }
    
    destroyRenderer(renderer);
    return 0;
}
