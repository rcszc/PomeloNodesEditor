## ⚠️ 项目已废弃 ⚠️ 

> __请前往最新重构项目:__ [PSAnodeEditor](https://github.com/rcszc/PSAnodeEditor)

---

# PomeloNodesEditor

基于 ImGui(OpenGL GLFW) 和 ImNodes 完善的节点编辑器. 

[B站视频](https://www.bilibili.com/video/BV1J8411X7es/)

### 2023-07-21 version 1.0.0 ahpla
- 相对于老版本重写了框架.
- 新增了文件的保存和加载以及编辑器自定义主题.
- 保留了极简步骤.

### 2023-08-29 version 1.1.0 ahpla
- 新增节点单曲线采样器.
- 新增节点数据导出处理类, 过滤掉不必要的数据,  以及提供一些处理工具, 方便后台解析开发.
- 文件命名改为蛇形标注法.

![IMG](https://github.com/rcszc/PomeloNodesEditor/blob/main/pomelo_nodeeditor.png)

## 1.0.0A

1.0.0 编辑器初始化参数结构体
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

1.0.0 初始化添加组件配置, json 文件数据结构参考 demo_cmp_json. 返回成功标志.
```cpp
bool EdiorGUI::PushNodeComponent(const char*)

PNE::EdiorGUI* PneDemo = new PNE::EdiorGUI(WinObj, initparam);
PneDemo->PushNodeComponent("components_json/demo_cmp_ass1.json");
```
1.0.0 渲染 Tick, 在事件循环中调用, 渲染 ImGui & ImNodes 帧, 传入外部其他 ImGui 渲染组件.
```cpp
void EdiorGUI::RenderTick(function<void()> render_gui)
```
1.0.0 默认组件, 节点操作菜单, 鼠标右键, 断连 删除 复制等. 在渲染 Tick 中调用.
```cpp
void EdiorGUI::SystemNodeMenu()
```
1.0.0 默认组件, 节点组件菜单, 固定编辑器窗口选项, 节点创建, 自定义编辑器主题颜色等. 在渲染 Tick 中调用.
```cpp
void EdiorGUI::SystemCompMenu()
```
1.0.0 默认扩展组件, 文件操作菜单, 节点数据 保存 & 加载, 参数输入 PNE::EdiorGUI& 使用EdiorGUI中的数据解析函数. 文件数据需要解析创建成节点源数据. 在渲染 Tick 中调用.
```cpp
void Ext_EditorFile::NodesEditor_FileMenu(PNE::EdiorGUI& editobj)
```
```cpp
 pnedemo.RenderTick([&pnedemo]()
        {
            pneobj.SystemCompMenu();
            pneobj.SystemNodeMenu();

            Ext_EditorFile::NodesEditor_FileMenu(pnedemo);
        }
);
```

1.0.0 获取节点编辑器 节点 & 互连 源数据. 返回源数据组结构体.
1.1.0 结构体去除不必要的重载函数.
```cpp
IODATA::NodesLines EdiorGUI::GetNodesLinesData()
```
```cpp
struct NodesExport {

	std::vector<pne_datadefine::node_attribute> nodes;
	std::unordered_map<
		int32_t,
		pne_datadefine::node_link_line
	> lines;

	size_t data_size;
};
```

1.0.0 读取解析文件加载 节点 & 互连 数据. 读取解析 => 搜索组件 => 创建节点编辑器源数据. 注意源数据和保存数据是两种数据结构, 保存文件数据只保留了重要连接部分.
```cpp
void EdiorGUI::ReadNodeLinesData(nodes..., lines...);
void EdiorGUI::ReadNodeLinesData(
        std::vector<IODATA::SaveNodeData>& nodes,
        std::vector<IODATA::SaveLineData>& lines
);
```
1.0.0 节点编辑器渲染标志, False: 不会渲染计算 ImNodes 部分以纯 ImGui 使用.
```cpp
bool EdiorGUI::RenderEditor = true
PneDemo.RenderEditor = false;
```

## 1.1.0A

1.1.0 新增导出类, 以简化源数据, 以及方便以后的工具. 构造时传入导出的源数据.
```cpp
PNE::DEC::EditorExport;
PNE::DEC::EditorExport(const IODATA::NodesExport& InDat);
// 依旧使用 1.0.0 的 GetNodesLinesData() 函数.
PNE::DEC::EditorExport Process(Test.GetNodesLinesData());
```
1.1.0 获取节点总数(编辑器所有节点的数量)
```cpp
size_t EditorExport::GetNodesNumber();
```
1.1.0 获取对应节点索引: 输入连接点(参数)数量 & 输出连接点(返回)数量.
```cpp
// node_index < EditorExport::GetNodesNumber();
void EditorExport::GetNodeIONumber(const size_t& node_index, size_t& input_num, size_t& output_num);
```
1.1.0 获取对应节点索引: 节点属性(数据结构).
```cpp
ExpNodeAttr EditorExport::GetNodeAttr(const size_t& node_index);
struct ExpNodeAttr {

	int32_t   node_unique; // 节点唯一标识 全局不会重复.
	NODE_TYPE node_type;   // 节点类型 自定义类型标识(size_t).

	uint32_t TextureHandle; // 节点纹理句柄(GL) 可实时修改节点显示的纹理.

	bool   SampleSwitch; // 节点采样开关 是否进行采样器渲染.
	float* SampleValue;  // 节点采样指向单个采样值的float指针.
};
```
1.1.0 获取对应节点索引 => 输入连接点索引: 连接点属性(数据结构).
```cpp
ExpNodePointIn EditorExport::GetNodeInPoint(const size_t& node_index, const size_t& point_in_index);
struct ExpNodePointIn {

	int32_t    point_unique;    // 输入点标识 全局唯一(包括输入输出).
	PARAM_TYPE point_type;      // 输入点类型 自定义参数类型(uint16_t).

        // 连接线标识(key) 连接点到此点的连接线全局唯一标识.
	int32_t point_link_line; // 输入只允许单连接.
};
```
1.1.0 获取对应节点索引 => 输出连接点索引: 连接点属性(数据结构).
```cpp
ExpNodePointOut EditorExport::GetNodeOutPoint(const size_t& node_index, const size_t& point_out_index);
struct ExpNodePointOut {

	int32_t              point_unique;    // 输出点标识 全局唯一.
	PARAM_TYPE           point_type;      // 输出点类型 自定义参数类型(uint16_t).

        // 如果一个节点的输出连接到另一个节点的输入, 那么这两个节点的连接点"point_link_line" 应该相同.
        // 普通参数输出可以连接多个, 但是执行路径只能连接一个(见后json).
	std::vector<int32_t> point_link_line; // 连接线标识(key), 输出可以多连接.
};
```
1.1.0 查询工具: 通过节点类型查询所有此类型节点的索引.
```cpp
// 因为同一类型节点可能有多个, 所以使用 vector.
std::vector<size_t> EditorExport::FindTypeNode(const NODE_TYPE& type);
```
1.1.0 查询工具: 通过输入连接点索引查询对应"上"一个节点连接点的标识(输出).
```cpp
int32_t EditorExport::FindInLineNode(const size_t& node_index, const size_t& point_in_index);
```
1.1.0 查询工具: 通过输出连接点索引查询对应"下"一个节点连接点的标识(输入).
```cpp
// 因为一个输出可能对应多个输入, 所以使用 vector.
std::vector<int32_t> EditorExport::FindOutLineNode(const size_t& node_index, const size_t& point_out_index);
```
1.1.0 查询工具: 通过 输入连接点 & 输出连接点 唯一标识查询自身所属节点的索引. 因为输入输出表示都是统一计数器分配, 所以全局不会重复.
```cpp
size_t EditorExport::FindIOUniqueNodeIndex(const int32_t& unique);
```
1.1.0 (测试) 执行路径搜索工具: 通过执行脉冲节点(通常全局只有一个), 和一个指定节点连接点参数类型, 因为执行路径通常贯穿节点, 目前默认如果节点开启路径选项(json配置),输入连接点第一个(index:0)与输出第一个, 为执行路径. 开始寻路查找完整执行路径. 返回为路径中各个节点的索引, Begin(0) - End. 同时会在控制台打印.
```cpp
std::vector<size_t> EditorExport::SearchEXEpath(const size_t& node_index, const int32_t& type);
```
以后可以不需要关心"pomelo_nodeeditor_define.h"中源数据结构是如何定义的.

1.1.0 新增采样节点只需要在配置文件中开启就可以使用. 目前位置整个配置更像一个状态机, 因为底层gui毕竟不是qt那种. 
以下是json配置模板:
如果开启 "[EXE]" 输入 & 输出 必须为同类型参数(贯穿执行路径), 并且都需要为输入输出连接点的第一个(方便工具检测执行路径).
目前启用执行路径以后样式还没有搞出来(像UE那种), 以至于看起来有点~~搞笑~~.
```cpp
{
  "node color": [ 0.7, 0.0, 1.0, 0.5 ],             // 节点常态颜色 RGBA.
  "node color click": [ 0.7, 0.0, 1.0, 0.7 ],       // 节点点击选择后颜色 RGBA.
  "node title": "FxBlend",                          // 节点标题文本.
  "node width": 200.0,                              // 节点宽度(高度由内容决定).
  "node image": "components_json/demo_texture.png", // 预先加载纹理图片路径.
  "node image size": [ 210.0, 105.0 ],              // 纹理渲染尺寸(ImGui).
  "node sample switch": false,                      // 节点采样器开关.
  "node sample timer": 200,                         // 节点采样器采样时间间隔(ms)
  "node execute path switch": true,                 // [EXE] 执行路径贯穿节点(执行类节点)
  "input point": [ // 输入连接点数组, 按需配置数量.
    {
      "point color": [ 0.92, 0.92, 0.92, 1.0 ], // 连接点文本颜色 RGBA.
      "point text": "In.EXE",                   // 连接点文本(自动缩进).
      "point param type": 1                     // 连接点自定义参数类型(编号).
    },
    {
      "point color": [ 1.0, 0.2, 0.0, 1.0 ],
      "point text": "In.Fx1",
      "point param type": 2
    },
    {
      "point color": [ 1.0, 0.2, 0.0, 1.0 ],
      "point text": "In.Fx2",
      "point param type": 2
    }
  ],
  "output point": [ // 输出连接点数组, 按需配置数量.
    {
      "point color": [ 0.92, 0.92, 0.92, 1.0 ], // (与输入相同).
      "point text": "Out.EXE",
      "point param type": 1
    },
    {
      "point color": [ 0.0, 1.0, 0.8, 0.82 ],
      "point text": "Out.Blend",
      "point param type": 3
    }
  ],
  "node type": 1 // 节点自定义类型编号.
}
```
如果想深入研究可以看注释, 很多用中文写的.

第三方依赖库:
- OpenGL GLFW3
- ImGui
- ImNodes
- RapidJSON

| 总行数:  | 测试设备(CPU)  | IDE   |
| :---:    | :---:         | :---:   |
|  2149    | i7-13700HX    | VisualStudio2022    |

## 1.0.0 附加
具体数据结构参考 PomeloNodeEditor_DataDefine.h 文件.
在 PomeloNodeEditor.cpp PomeloNodeEditor_External.cpp 中使用了windows库.
目前不能传入纹理句柄, 以及节点字体缩进存在问题.
持续更新中... 2023.07.31 暂时停更1个月.

## 1.1.0 附加
编辑器字体缩进问题解决;
开学了开始卷室友tnnd肝, 恢复更新!
后台解析部分暂时不考虑给出解决方案, 但是会尽量会提供工具.
因为解析部分太多种形式(~~我菜~~ 可以是像shader那样无执行路径注重数据流的形式, 也可以像UE那样有执行路径)

（我是菜鸟大佬勿喷.
