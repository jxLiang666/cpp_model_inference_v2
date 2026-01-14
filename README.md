# C++ 工程模板 精简版

## 第三方库
- [spdlog](https://github.com/gabime/spdlog)
- [googletest](https://github.com/google/googletest)

## 结构
``` text
├── CMakeLists.txt
├── cmake
│   └── .cmake
├── config
│   ├── CMakesLists.txt
│   └── config.h.in
├── core
│   ├── CMakeLists.txt
│   └── ...
├── deps
│   ├── CMakesLists.txt
│   ├── 
│   └── ...
├── examples
│   ├── CMakeLists.txt
│   └── ...
├── src
│   ├── CMakesLists.txt
│   ├── 
│   └── ...
└── tests
    ├── CMakeLists.txt
    └── ...
```

## 工程构建
### 拉取源码
```bash
```
### 编译编译选项
```bash
cmake -DEXAMPLES=ON \
      -DTESTS=ON \
      -DCMAKE_BUILD_TYPE=Debug \
      ..
```
### 编译
```bash
sh build.sh
```

## 测试
```bash
# 运行所有测试
ctest --test-dir build

# 指定测试集
ctest -R xx

# 指定测试集中的某一项
ctest -R xx.xx

# 测试输出详细信息
ctest -V
# 或者
ctest -VV
```
