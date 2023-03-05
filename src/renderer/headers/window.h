//
//  window.h
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 04/02/2023.
//

#ifndef window_h
#define window_h

#include "headers/common.h"
#include "cglm/cglm.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct Renderer {} Renderer;
typedef struct Object {} Object;
typedef struct Model {} Model;
typedef struct UI {} UI;
typedef struct Rectangle {} Rectangle;

extern Renderer *createRenderer(void);
extern void destroyRenderer(Renderer *renderer);
extern int windowShouldClose(Renderer *renderer);
extern int drawFrame(Renderer *renderer);
extern int addModel(Renderer *renderer, char filename[], vec4 color);
extern Object *addObject(Renderer *renderer, int model, vec3 pos, vec3 orientation);
extern void placeObject(Object *obj, vec3 pos);
extern void moveGlobalCamera(Renderer *renderer, vec3 displacment);
extern GLFWwindow *getGLFWWindow(Renderer *renderer);
extern float *getGlobalCameraPosition(Renderer *renderer);
extern float *getGlobalCameraUp(Renderer *renderer);
extern void placeObject(Object *obj, vec3 pos);
extern float *getObjectPos(Object *obj);
extern UI *getUI(Renderer *renderer);
extern Rectangle *addRectangle(UI *ui, int x, int y, int width, int height, vec4 color, vec4 borderColor);
extern void resizeRectangle(Rectangle *rect, int width, int height);
extern void placeRectangle(Rectangle *rect, int x, int y);

#endif /* window_h */
