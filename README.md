# C++模型推理 v2

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
