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

typedef struct {} Renderer;
typedef struct {} Object;
typedef int Model;
typedef struct {} UI;
typedef struct {} Rectangle;
typedef struct {} Button;
typedef struct {} Font;
typedef struct {} Text;

extern Renderer *createRenderer(void);
extern void destroyRenderer(Renderer *renderer);
extern void clearRenderer(Renderer *renderer);
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
extern void addTexture(UI *ui, Rectangle *rect, char filename[]);
extern void resizeRectangle(Rectangle *rect, int width, int height);
extern void placeRectangle(Rectangle *rect, int x, int y);
extern Button *addButton(UI *ui, int x, int y, int width, int height, char texture[]);
extern int isButtonPress(UI *ui, Button *button);
extern void outlineObject(Object *obj, uint8_t b);
extern Font *loadTextFont(UI *ui, char filename[]);
extern void destroyFont(UI *ui, Font *font);
extern Text *addText(UI *ui, Font *font, char text[], int x, int y, vec3 color, float scale);
extern void modifyText(Text *t, char content[]);
extern void printVulkanMemStat(void);

#endif /* window_h */
