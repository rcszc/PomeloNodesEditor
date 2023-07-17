// pomelo_nodeeditor.
/*
* by RCSZ [2023_07_15] version 1.0
* visual studio 2019 MSVC170 x64
* Libray: ImGui ImNodes GLFW RapidJSON
* Device: i7-13700HX DDR5 16GiB 4800MHz
*/

/*
* [2023.07.15] startup project.
*/

/* 2019 配置路径.
* external_library/graphics_library/glfw_libray/include/
* external_library/graphics_library/glew_libray/include/
* external_library/guiimgui_library/imgui_glfw/
* external_library/imnodes_master/
* external_library/RapidJSON/
* external_library/graphics_library/glfw_libray/lib-vc2019/
* external_library/graphics_library/glew_libray/lib/Release/x64/
* 忽略: msvcrt.lib
*/

#include <iostream>
#include <windows.h>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>

#include "pomelo_node_editor/pomelo_node_editor.h"

using namespace std;

namespace DemoGlobal {

    bool             WindowNoClose = false;
    ImGuiWindowFlags WindowFlags = NULL;
}

/*
* Pomelo_NodeEditor Test RenderFunc.
* GLFW 3.0 OpenGL 460 shader.
*/
void GLFW_TEST_EVENTLOOP() { // 循环.

    // NodeEditorGui 绘制标志.
    static bool drawflag = true;

    PNErender::rendertick_gui(drawflag, []() {

        PNEpreset::component_guipanel();
    });
}

void GLFW_TEST_EVENTINIT(GLFWwindow* WinObj) { // 初始化.
    
    PNEinit::initialization_imgui(
        PNEinit::imguiinit(
            "#version 460",                                                // OpenGL 着色器版本.
            "external_library/guiimgui_library/imgui_fonts/MSYH_Bold.ttf", // ImGui  字体路径.
            18.5f,                                                         // ImGui  字体大小.
            STYLE_TYPE_CYAN                                                // 颜色主题(基于 ImGui.Dark)
            ), 
        WinObj
    );

    PNEinit::initialization_imnodes();

    // 加载组件配置.
    PNEpreset::component_load("pomelo_node_editor/components_json/demo_cmp_ass1.json");
    PNEpreset::component_load("pomelo_node_editor/components_json/demo_cmp_ass2.json");
    PNEpreset::component_load("pomelo_node_editor/components_json/demo_cmp_ass3.json");
    PNEpreset::component_load("pomelo_node_editor/components_json/demo_cmp_ass4.json");
}

// ************************************************ GLFW Render Test ************************************************
// 推!
// 推!
// 推!
// 推!
// 推!
// 推!
// 推!
// 推!

static void GLFW_ErrorCallback(int error, const char* description) {
    // GlFW Callback ErrorPrint.
    fprintf(stderr, "[GLfwSystem]: GLFW Error %d: %s\n", error, description);
}

void oglwindow_eventloop(GLFWwindow* winobj) {
    int buffersize[2] = {};

    while (true) {
        glfwPollEvents();
        // glfwSetWindowShouldClose(_MainWinodwObj, PSA_TRUE);

        glfwGetFramebufferSize(winobj, &buffersize[0], &buffersize[1]);
        glViewport(0, 0, buffersize[0], buffersize[1]);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // main => oglwindow_eventloop => render_loop_event
        GLFW_TEST_EVENTLOOP();

        glfwMakeContextCurrent(winobj);
        glfwSwapBuffers(winobj);
    }
}

int main() {
    // glfw init create.
    glfwSetErrorCallback(GLFW_ErrorCallback);
    if (!glfwInit()) {
        cout << "GLFW ObjectInit Failed." << endl;
    }

    // OpenGL Window Config.
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // 兼容模式.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // glfw.version 3.2+ [profile].
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);             // glfw.version 3.0+ 
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Fixed window size.
        glfwWindowHint(GLFW_SAMPLES, 4);          // 4x Samples MSAA.
    }

    // create window.
    GLFWmonitor* CreateFlag = {};
    GLFWwindow* WindowObject = {};

    WindowObject = glfwCreateWindow(
        1440, 810,
        "Pomelo NodeEditor", CreateFlag, nullptr
    );

    // create context.
    glfwMakeContextCurrent(WindowObject);
    // Enable Async. 开启垂直同步.
    glfwSwapInterval(1);

    // Init.
    GLFW_TEST_EVENTINIT(WindowObject);

    // main => oglwindow_eventloop
    oglwindow_eventloop(WindowObject);

    // opengl free window.
    glfwDestroyWindow(WindowObject);
    glfwTerminate();
}