# PomeloNodesEditor
基于 ImGui 和 ImNodes 改进的节点编辑器.

编辑器初始化 ImGui 和 ImNodes
```cpp
PNEinit::initialization_imgui(
        PNEinit::imguiinit(
            const char*,      // OpenGL 着色器版本 ("#version 330", "#version 460"). 
            const char*,      // ImGui  字体路径 (.ttf).
            float,            // ImGui  字体大小.
            STYLE_TYPE_CYAN   // 编辑器颜色主题(基于 ImGui.Dark) STYLE_TYPE_ORANGE, STYLE_TYPE_PINK
            ), 
        GLFWwindow*           // GLFW 窗口对象.
);

PNEinit::initialization_imnodes(void);
```
孩子不懂事写着玩大佬勿喷.
