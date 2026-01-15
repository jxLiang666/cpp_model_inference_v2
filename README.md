# C++模型推理 v2
### C++ 模型推理 v2 相较于 v1 版本 优化改进说明
#### 改进点
1. 推理优化，将支持更多模型格式（例如torchscript等）
2. 内存优化，将减少内存拷贝次数，多复用内存
3. 代码结构优化，减少运行时开销


## 工程构建
### 拉取源码
```bash
git clone https://github.com/jxLiang666/cpp_model_inference_v2.git
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
