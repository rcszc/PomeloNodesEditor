// PomeloEditor.
/*
* by RCSZ [2023_07_20] version 1.0
* visual studio 2019 MSVC170 x64
* Libray: ImGui ImNodes GLFW RapidJSON
* Device: i7-13700HX DDR5 16GiB 4800MHz
*
* [2023.07.20] startup project.
*
* 2019 配置路径.
* external_library/graphics_library/glfw_libray/include/
* external_library/graphics_library/glew_libray/include/
* external_library/guiimgui_library/imgui_glfw/
* external_library/imnodes_master/
* external_library/RapidJSON/
* external_library/graphics_library/glfw_libray/lib-vc2019/
* external_library/graphics_library/glew_libray/lib/Release/x64/
* external_library/graphics_library/
*/

#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>

#include "node_editor_src/PomeloNodeEditor.h"
#include "node_editor_src/PomeloNodeEditor_External.h"
    
using namespace std;

/*
* Pomelo_NodeEditor Test RenderFunc.
* GLFW 3.0 OpenGL 460 shader.[TexErr]
* GLFW 2.0 OpenGL 330 shader.
*/
void GLFW_TEST_EVENTLOOP(PNE::EdiorGUI& pneobj) { // 循环.

    pneobj.RenderTick([&pneobj]()
        {
            pneobj.SystemCompMenu();
            pneobj.SystemNodeMenu();

            Ext_EditorFile::NodesEditor_FileMenu(pneobj);
        }
    );
}

PNE::EdiorGUI* GLFW_TEST_EVENTINIT(GLFWwindow* WinObj) { // 初始化.
    
    pne_datadefine::pne_init::imguiinit initparam =
        pne_datadefine::pne_init::imguiinit(
            "#version 330",                                                // OpenGL 着色器版本.
            "external_library/guiimgui_library/imgui_fonts/MSYH_Bold.ttf", // ImGui  字体路径.
            18.5f,                                                         // ImGui  字体大小.
            STYLE_TYPE_CYAN                                                // 颜色主题(基于 ImGui.Dark)
        );

    PNE::EdiorGUI* PneDemo = new PNE::EdiorGUI(WinObj, initparam);

    PneDemo->PushNodeComponent("components_json/demo_cmp_ass1.json");
    PneDemo->PushNodeComponent("components_json/demo_cmp_ass2.json");
    PneDemo->PushNodeComponent("components_json/demo_cmp_ass3.json");
    PneDemo->PushNodeComponent("components_json/demo_cmp_ass4.json");

    return PneDemo;
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

    // Init.
    PNE::EdiorGUI* objdata = GLFW_TEST_EVENTINIT(winobj);

    while (true) {
        glfwPollEvents();
        // glfwSetWindowShouldClose(_MainWinodwObj, PSA_TRUE);

        glfwGetFramebufferSize(winobj, &buffersize[0], &buffersize[1]);
        glViewport(0, 0, buffersize[0], buffersize[1]);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // main => oglwindow_eventloop => render_loop_event
        GLFW_TEST_EVENTLOOP(*objdata);

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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // 兼容模式.
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // glfw.version 3.2+ [profile].
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);             // glfw.version 3.0+ 
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Fixed window size.
        glfwWindowHint(GLFW_SAMPLES, 4);          // 4x Samples MSAA.
    }

    // create window.
    GLFWmonitor* CreateFlag = {};
    GLFWwindow* WindowObject = {};

    WindowObject = glfwCreateWindow(
        1680, 945,
        "Pomelo NodeEditor", CreateFlag, nullptr
    );

    // create context.
    glfwMakeContextCurrent(WindowObject);
    // Enable Async. 开启垂直同步.
    glfwSwapInterval(1);

    // main => oglwindow_eventloop
    oglwindow_eventloop(WindowObject);

    // opengl free window.
    glfwDestroyWindow(WindowObject);
    glfwTerminate();
}