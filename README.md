[TOC]

# 学习 OpenGL



## 1. 环境

- C++11+
- CMake3.12+
- VS2022



## 2. 下载及编译

使用 `gflw` `glm` `imgui` `stb` 作为子模块，所以需要下载 `submodule`。

```
git clone --recursive git@github.com:august295/learn_opengl.git
```

工程使用 `CMake` 构建。



## 3. 编译

### 3.1. Windows

其中 `imgui` 使用中文，需要加载字体库，所以目前只适配 `Windows`。

```bash
cmake -B"build" -G"Visual Studio 17 2022"
```






# 参考

