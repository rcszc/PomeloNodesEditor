# PomeloNodesEditor
基于 ImGui 和 ImNodes 完善的节点编辑器.

相对于老版本重写了框架.
新增了文件的保存和加载以及编辑器自定义主题.

2023-07-21 version 1.0.0 ahpla

![IMG](https://github.com/rcszc/PomeloNodesEditor/blob/main/profile/PomeloEditor.png)

1.0.0 初始化参数结构体
```cpp
pne_datadefine::pne_init::imguiinit initparam =
        pne_datadefine::pne_init::imguiinit(
            "#version 330",                                                // OpenGL 着色器版本.
            "external_library/guiimgui_library/imgui_fonts/MSYH_Bold.ttf", // ImGui  字体路径.
            18.5f,                                                         // ImGui  字体大小.
            STYLE_TYPE_CYAN                                                // 初始颜色主题(基于 ImGui.Dark)
        );
```

1.0.0 编辑器类构造参数. GLFW窗口对象, GUI初始化参数结构.
```cpp
PNE::EdiorGUI(GLFWwindow*, pne_datadefine::pne_init::imguiinit = {});
```

### EdiorGUI::PushNodeComponent(const char*)
1.0.0 初始化添加组件配置, json 文件数据结构参考 demo_cmp_json.
```cpp
PNE::EdiorGUI* PneDemo = new PNE::EdiorGUI(WinObj, initparam);
PneDemo->PushNodeComponent("components_json/demo_cmp_ass1.json");
```
