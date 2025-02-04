[TOC]

# 学习 OpenGL



## 1. 环境

- C++11+
- CMake3.12+
- VS2022



## 2. 下载及编译

使用 `gflw` `glm` `imgui` `stb` `assimp` 作为子模块，所以需要下载 `submodule`。

```bash
git clone --recursive git@github.com:august295/learn_opengl.git
```



## 3. 编译

工程使用 `CMake` 构建。

### 3.1. Windows

```bash
cmake -B"build" -G"Visual Studio 17 2022"
```

#### 3.1.1. imgui

`imgui` 使用中文，需要加载字体库，所以目前只适配 `Windows`。

```c++
# 微软雅黑
io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
```

#### 3.1.2. assimp

`assimp` 编译时使用 `/source-charset:utf-8`， 但是和本项目使用 `/utf-8`（包括 /source-charset、/execution-charset、/validate-charset，/input-charset等） 冲突，修改 `assimp/CMakeLists.txt` 中 `/source-charset:utf-8` 为 `/utf-8`




# 参考

