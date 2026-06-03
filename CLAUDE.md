# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

Windows 桌面应用（C++17 + Qt 6.7.3 Widgets），驱动两台 DVP/度申线扫相机，实时显示袋子画面。
正面、背面相机采集的图像会被拼接后显示；程序同时统计产量信息（计数、丢帧），并将相机/产品参数
持久化到 XML 配置文件。

本项目仅支持 Windows（MSVC、`windeployqt`、基于 Win32 的进程/单实例检查），无法在 Linux/macOS
上构建或运行。

## 构建

工具链：CMake + vcpkg + MSVC，通过 `CMakePresets.json` 配置。构建前必须能解析三个外部依赖：

- `VCPKG_ROOT` 环境变量 → vcpkg 工具链（提供 OpenCV）。
- `QT_ROOT` 环境变量 → Qt，构建时会拼接 `/6.7.3/msvc2022_64`。未设置时回退到
  `C:/Development/Qt/6.7.3/msvc2019_64`。
- **RWUL** —— 公司内部自研库，根 `CMakeLists.txt` 中硬编码路径为 `D:/Development/RWUL`。通过
  `find_package(RWUL CONFIG REQUIRED COMPONENTS core oso hoecd rqwu imgcore imgqt)` 查找。
  大部分领域功能（相机 I/O `rqwc`/`hoec`、`oso` 序列化框架、Qt 工具 `rqwu`）都在这个库里，
  **不在本仓库中**。

常用命令（在 VS / Developer 命令行的仓库根目录下执行）：

```sh
# 使用 Visual Studio 生成器配置 + 构建
cmake --preset vs2022
cmake --build --preset vs2022-release   # 或 vs2022-debug

# Ninja 方式
cmake --preset windows-ninja-release
cmake --build --preset windows-ninja-release-build
```

另有 `*-NoHardware` 预设（`vs2022-debug-NoHardware` 等），会设置 `BUILD_WITHOUT_HARDWARE`
缓存变量，用于在没有连接相机时构建。但该标志目前只在预设中定义，**CMake 和源码中尚未引用**。

`Q_INIT_RESOURCE` + `windeployqt` 作为 POST_BUILD 步骤执行，因此构建出的 `.exe` 目录是自包含的。
项目没有测试套件（`testPresets` 为空）。

## 架构

`BagsRealTimeDisplay/` 下采用严格的分层结构，每个叶子目录都是独立的 CMake 目标（每个文件夹
`add_subdirectory`，通过 `file(GLOB ...)` 扫描 `src/` 与 `include/`）：

- `UI/` —— `main.cpp`、`AppRuntime`（生命周期控制器），以及 `UIModule`（主窗口
  `BagsRealTimeDisplay` 加对话框 `DlgProductSet`、`DlgCloseForm`）。`.ui` 文件通过
  `qt_wrap_ui` 编译为 `ui_*.h`。
- `Modules/` —— 硬件/配置模块：`CameraModule`、`ConfigModule`、单例聚合器 `Modules`，以及
  `IModule` 接口。
- `BusinessLayer/` —— 处理逻辑：`ImageStitch`（一个 `QThread`）、`AsynchronousThread`
  （UI 刷新线程 + 统计）。
- `Global/Utility` —— 全局路径/配置单例。
- `Tools/` —— 可复用控件/辅助类：`PanZoomLabel`、`RunEnvCheck`。

### 模块生命周期（`IModule`）

`Modules/IModule/include/IModule.hpp` 定义了每个模块都要实现的契约：`build()` / `destroy()`
（资源创建/释放）与 `start()` / `stop()`（运行时启停）。`Modules` 是一个**单例**
（`Modules::getInstance()`），持有每个具体模块的一个实例，并按顺序把生命周期调用分发给所有模块。

`AppRuntime::initialize()` 是标准的启动流程，也是理解整体接线的最佳入口：

1. `Modules::check()` —— 启动前检查：单实例锁、若厂商相机软件（`MVS.exe`、`BasedCam3.exe`）
   正在运行则拒绝启动、确保配置目录存在。
2. `_modules.build()` —— 构造相机、加载配置。
3. 构造主窗口，按引用注入 `ConfigModule&` 和 `CameraModule&`。
4. `_modules.connect()` + `AppRuntime::build_connect()` —— 跨层连接 Qt 信号/槽。
5. `_modules.start()`。

`shutdown()`（挂在 `aboutToQuit` 上）先调用 `stop()` 再调用 `destroy()`；配置正是在
`ConfigModule::destroy()` 中存回磁盘，所以**销毁顺序很重要**。

### 跨层数据流（Qt 信号/槽）

各层之间解耦，仅通过信号连接，连接在 `Modules::connect()` 与 `AppRuntime::build_connect()`
中建立：

```
CameraModule::onCameraCapture ──► ImageStitch::onFrameCaptured   (Modules::connect)
ImageStitch::imageReady       ──► BagsRealTimeDisplay::onCameraDisplay
CameraModule::onCameraStateChanged ──► updateCameraLabelState
RefreshUIThread::emit_RefreshUI ──► onUpdateStatisticalInfoUI / DlgProductSet::onUpdateFrameLost
```

`ImageStitch` 缓存 `MatInfo` 帧并每 N 张拼接一次；统计信息（计数、各相机丢帧数）保存在
`AsynchronousThreadModule::StatisticalInfo` 中，类型为 `std::atomic`，由 `RefreshUIThread`
轮询后刷新 UI。

### 配置与持久化（`oso`）

配置通过 RWUL 的 `oso` 框架序列化（XML 后端，使用 `rw::oso::StorageContext`）。纯数据配置类位于
`Modules/ConfigModule/osoInclude/`（如 `SetConfig`、`BagsRealTimeDisplayInfo`），它们提供与
`rw::oso::ObjectStoreAssembly` 互转的转换运算符，使用形如 `"$variable$baoguang1$"` 的字符串键。
`osoFile/` 中的 `.oso` 文件是 schema/序列化产物。

`ConfigModule` 在 `build()` 时加载这些 XML 文件，在 `destroy()` 时保存。所有文件路径集中在
`Global/Utility/include/Utility.hpp`（`globalPath` 单例），根目录为
`D:\zfkjData\BagsRealTimeDisplay\` —— 配置在 `config\`，模型在 `model\`。
`Modules::checkFileExistAndFormat` 负责校验/迁移配置文件，当磁盘上的格式过期时通过
`AssemblyMergeTool` 合并进新增字段。

## 约定

- **命名采用中文拼音**，尤其是配置字段和 UI 槽函数 —— 例如 `baoguang`（曝光）、`zengyi`（增益）、
  `chengfaqi`（乘法器）、`houfenpin`（后分频）、`xiangsudangliang`（像素当量）、
  `qiehuanzhangshu`（切换张数）、`zhengmian`/`beimian`（正面/背面）。请沿用既有命名，不要翻译成英文。
- **缩进用 Tab**，宽度 4（见 `.editorconfig`）；文件编码为 UTF-8。遵循 ReSharper/`.DotSettings`
  约定（局部变量 camelCase，部分类的私有成员以 `_` 前缀）。
- 本仓库的提交信息用中文书写。
- 新增代码：把源文件/头文件放到对应分层的 `src/`+`include/` 下，`file(GLOB)` 的 CMake 配置会自动
  收录；但**新增子目录需要 `add_subdirectory` 加上自己的 `CMakeLists.txt`**，并配置好
  `XXX::target` 别名和链接关系。
