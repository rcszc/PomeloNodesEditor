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

### bool EdiorGUI::PushNodeComponent(const char*)
1.0.0 初始化添加组件配置, json 文件数据结构参考 demo_cmp_json. 返回成功标志.
```cpp
PNE::EdiorGUI* PneDemo = new PNE::EdiorGUI(WinObj, initparam);
PneDemo->PushNodeComponent("components_json/demo_cmp_ass1.json");
```

### void EdiorGUI::RenderTick(function<void()> render_gui)
1.0.0 渲染 Tick, 在事件循环中调用, 渲染 ImGui & ImNodes 帧, 传入外部其他 ImGui 渲染组件.
### void EdiorGUI::SystemNodeMenu()
1.0.0 默认组件, 节点操作菜单, 鼠标右键, 断连 删除 复制等. 在渲染 Tick 中调用.
### void EdiorGUI::SystemCompMenu()
1.0.0 默认组件, 节点组件菜单, 固定编辑器窗口选项, 节点创建, 自定义编辑器主题颜色等. 在渲染 Tick 中调用.
### void Ext_EditorFile::NodesEditor_FileMenu(PNE::EdiorGUI& editobj)
1.0.0 默认扩展组件, 文件操作菜单, 节点数据 保存 & 加载, 参数输入 PNE::EdiorGUI& 使用EdiorGUI中的数据解析函数. 文件数据需要解析创建成节点源数据. 在渲染 Tick 中调用.
```cpp
 pnedemo.RenderTick([&pnedemo]()
        {
            pneobj.SystemCompMenu();
            pneobj.SystemNodeMenu();

            Ext_EditorFile::NodesEditor_FileMenu(pnedemo);
        }
);
```

