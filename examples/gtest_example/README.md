# parser_testp 使用说明

## 查看所有的测试内容
```bash
./build/bin/gtest_ex --gtest_list_tests
```

## 启动测试
```bash
ctest --test-dir build -R CalcTest 只看结果
ctest --test-dir build -V -R CalcTest 会打印出 每个测试套件名 + 测试用例名 的运行状态
ctest --test-dir build -VV -R CalcTest 打印测试用例的「内部日志 / 自定义打印信息」
```