//
//  main_loop.c
//  AgeOfAnarchyXCode
//
//  Created by Emmanuel Mera on 10/03/2023.
//

#include "main_loop.h"
#include "loading.h"
#include "player.h"
#include "context.h"

double xpos, ypos, rxpos, rypos;
Rectangle *rect;

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Renderer *renderer = glfwGetWindowUserPointer(window);
    float shift = tanh(yoffset)/10.f+1.f;
    moveGlobalCamera(renderer, (vec3){shift, 0.f, 0.f});
}

static void cursor_position_callback(GLFWwindow* window, double nxpos, double nypos) {
    Renderer *renderer = glfwGetWindowUserPointer(window);
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (state == GLFW_PRESS | state == GLFW_REPEAT)
        moveGlobalCamera(renderer, (vec3){1.f, (nypos-ypos)/50.f, (nxpos-xpos)/50.f}); // some mistakes here
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        resizeRectangle(rect, ((int)xpos-rxpos), ((int)ypos-rypos));
    xpos = nxpos;
    ypos = nypos;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Renderer *renderer = glfwGetWindowUserPointer(window);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        rxpos = xpos;
        rypos = ypos;
        placeRectangle(rect, (int)xpos, (int)ypos);
        resizeRectangle(rect, 0, 0);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        placeRectangle(rect, -5, -5);
        resizeRectangle(rect, 1, 1);
    }
    
    // TODO: change the position of this code + mostlikely will change because did it with some stupid geometry
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

void startGame(Renderer *renderer) {
    clearRenderer(renderer);
    Context *context = createContext(renderer);
    
    loadResources(context);
    
    Player *player = createPlayer(1000);
    
    UI *ui = getUI(renderer);
    GLFWwindow *window = getGLFWWindow(renderer);
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    rect = addRectangle(ui, -1, 0, 0, 0, (vec4){.5f, .5f, .5f, .1f}, (vec4){0.0f, 0.0f, 0.0f, 0.0f});
    
    glfwSetScrollCallback(window, scroll_callback);
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    while (!windowShouldClose(renderer)) {
        float time = glfwGetTime();
        deltaTime = time-lastFrame;
        lastFrame = time;
        drawFrame(renderer);
        glfwPollEvents();
        // printf("%f\n", 1/deltaTime);
    }
    
    destroyContext(context);
}
