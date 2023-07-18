# PomeloNodesEditor
### 基于 ImGui 和 ImNodes 改进的节点编辑器.

- 依赖图形库:     OpenGL GLFW（3.2+） GLEW
- 依赖GUI库:      ImGui ImNodes
- 依赖Json解析库: RapidJSON

[2023.07.18] 修改了输入允许多个连接的bug.

编辑器初始化 ImGui 和 ImNodes
```cpp
void PNEinit::initialization_imgui(
        PNEinit::imguiinit(
            const char*,      // OpenGL 着色器版本 ("#version 330", "#version 460"). 
            const char*,      // ImGui  字体路径 (.ttf).
            float,            // ImGui  字体大小.
            STYLE_TYPE_CYAN   // 编辑器颜色主题(基于 ImGui.Dark) STYLE_TYPE_ORANGE, STYLE_TYPE_PINK
            ), 
        GLFWwindow*           // GLFW 窗口对象.
);

void PNEinit::initialization_imnodes(void);
```

加载节点组件配置文件
```cpp
bool PNEpreset::component_load(const char*); // json 配置文件路径.
```

绘制GUI, 如果还有其他ImGui的窗口需要绘制, 可直接lambda中编写无需ImGui帧绘制函数.
"drawflag" 为绘制节点编辑器(ImNodes)标志 FALSE 即不会绘制编辑器.
```cpp
PNErender::rendertick_gui(drawflag, []() {

        PNEpreset::component_guipanel();

	// ImGui::Begin("Test");
	// ImGui::Text("Hello World");
	// ImGui::End();

    });
```
提供了一个简易的组件面板, 以便测试.
```cpp
void PNEpreset::component_guipanel();
```

编辑器ImGui和ImNodes的释放.
```cpp
void PNEclose::close_imgui();
void PNEclose::close_imnodes();
```

获取节点编辑器 节点数据 互联数据.
nodes data vector, links data hash_map.
具体参考 pomelo_node_editor.h 92-140 lins
```cpp
void PNEGET_node_link_data(
	std::vector<pne_node_component::node_attribute>&                 node_data,
	std::unordered_map<int32_t, pne_node_component::node_link_line>& link_data
);
```

持续更新中...

孩子不懂事写着玩大佬勿喷()
