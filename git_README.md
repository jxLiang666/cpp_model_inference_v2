## git commit comment编写

| 类型  | 描述 |
|-------|-------|
| feat | 新功能 |
| fix | 修复bug |
| docs | 文档修改 |
| style | 格式修改（不影响逻辑） |
| refactor | 代码重构，不改变功能 |
| perf | 性能优化 |
| test | 添加或修改测试 |
| build | 构建相关的修改（脚本、依赖） |
| ci | CI/CD 配置修改 |
| chore | 杂项修改，不影响源代码，如工具、配置文件 |

## git 文件追踪
暂时忽略对该文件的修改
git update-index --assume-unchanged <文件路径>

重新跟踪该文件的修改
git update-index --no-assume-unchanged <文件路径>

## git 常用命令
删除远端分支
git push <remote> --delete <branch>

创建分支并且切换
git checkout -b <new_branch> <base_branch> 基于本地分支
git checkout -b <new_branch> origin/<base_branch> 基于远端分支
