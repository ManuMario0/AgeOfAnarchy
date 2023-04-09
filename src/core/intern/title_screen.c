//
//  title_screen.c
//  AgeOfAnarchy
//
//  Created by Emmanuel Mera on 09/03/2023.
//

#include "title_screen.h"
#include "main_loop.h"

static int stop = 0;
Button *startButton;
static Font *font = VK_NULL_HANDLE;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Renderer *renderer = glfwGetWindowUserPointer(window);
    UI *ui = getUI(renderer);
    
    if (isButtonPress(ui, startButton)) {
        startGame(renderer);
    }
}

void launchTitleScreen(Renderer *renderer) {
    clearRenderer(renderer);
    
    UI *ui = getUI(renderer);
    GLFWwindow *window = getGLFWWindow(renderer);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    startButton = addButton(ui, width/2 - 70, height/2 - 100, 140, 80, "../Resources/test.png");
    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    while (!windowShouldClose(renderer) && !stop) {
        drawFrame(renderer);
        glfwPollEvents();
    }
    //destroyFont(ui, font);
}
