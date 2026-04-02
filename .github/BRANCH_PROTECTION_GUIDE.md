# GitHub Branch Protection 配置指南

本指南说明如何配置 GitHub 分支保护规则，确保所有 PR 必须通过测试才能合并。

## 🔒 配置步骤

### 1. 进入仓库设置
1. 打开你的 GitHub 仓库
2. 点击 **Settings** (设置)
3. 在左侧菜单中选择 **Branches** (分支)

### 2. 添加分支保护规则
1. 点击 **Add rule** (添加规则) 或 **Add branch protection rule**
2. 在 **Branch name pattern** 中输入要保护的分支名称：
   - 输入 `main` 或 `master`（根据你的主分支名称）
   - 或使用通配符 `main*` 保护所有以 main 开头的分支

### 3. 配置必需的保护选项

勾选以下选项：

#### ✅ **Require a pull request before merging**
- 勾选此选项确保所有更改必须通过 PR
- 子选项：
  - ✅ **Require approvals**: 设置需要的审批数量（建议至少 1 个）
  - ✅ **Dismiss stale pull request approvals when new commits are pushed**: 新提交时撤销旧的审批
  - ✅ **Require review from Code Owners**: 如果有 CODEOWNERS 文件

#### ✅ **Require status checks to pass before merging**
这是最重要的选项！确保 CI 测试通过才能合并。

配置步骤：
1. 勾选 **Require status checks to pass before merging**
2. 勾选 **Require branches to be up to date before merging**
3. 在搜索框中搜索并添加以下状态检查：
   - `build-and-run-pattern-tests` - 这是运行所有测试的主要任务
   - `all-tests-passed` - 这是确认所有测试都通过的最终检查

> 💡 **提示**: 状态检查名称必须至少运行一次才会出现在搜索列表中。你可以先创建一个 PR 触发 CI，然后再配置分支保护。

#### ✅ **Require conversation resolution before merging** (可选)
- 确保所有评论都已解决才能合并

#### ✅ **Require signed commits** (可选，推荐)
- 要求提交必须经过签名验证

#### ✅ **Require linear history** (可选)
- 强制使用线性历史记录（rebase 或 squash merge）

#### ✅ **Include administrators**
- 勾选此项使规则也适用于仓库管理员

### 4. 保存规则
点击页面底部的 **Create** 或 **Save changes** 按钮

## 📋 推荐的完整配置

```yaml
Branch protection rules for: main (or master)

✅ Require a pull request before merging
   ✅ Require approvals: 1
   ✅ Dismiss stale pull request approvals when new commits are pushed

✅ Require status checks to pass before merging
   ✅ Require branches to be up to date before merging
   Required checks:
   - all-tests-passed

✅ Require conversation resolution before merging

✅ Require linear history

✅ Include administrators
```

## 🚀 测试配置

配置完成后，创建一个测试 PR 来验证：

1. 创建新分支：
   ```bash
   git checkout -b test-branch-protection
   ```

2. 做一个小改动并提交：
   ```bash
   echo "// test" >> test_singleton_pattern.cpp
   git add test_singleton_pattern.cpp
   git commit -m "Test branch protection"
   git push origin test-branch-protection
   ```

3. 在 GitHub 上创建 PR

4. 验证以下内容：
   - ✅ CI 自动运行
   - ✅ 所有测试必须通过（显示绿色勾号）
   - ✅ "Merge" 按钮在测试通过前是禁用的
   - ✅ 测试通过后才能点击合并

## 🔍 当前 CI 配置说明

我们的 GitHub Actions 工作流配置包括：

### 测试矩阵
- **23 个设计模式测试** - 涵盖所有 GoF 设计模式
- **6 个实用模式测试** - 内存池、线程池等
- **总计 29 个独立测试**

### 测试过程
1. 每个测试文件独立编译和运行
2. 使用 C++14 标准
3. 启用编译器警告（`-Wall -Wextra -pedantic`）
4. 支持多线程（`-pthread`）
5. 并行运行以提高速度（`fail-fast: false`）

### 成功标准
- 所有 29 个测试都必须编译成功
- 所有 29 个测试都必须运行成功（返回码 0）
- 任何一个测试失败，整个 CI 将失败

## 🐛 故障排查

### 问题：找不到状态检查
**解决方案**:
1. 确保 CI 至少运行过一次
2. 创建一个测试 PR 触发 CI
3. 等待 CI 完成后再配置分支保护

### 问题：管理员可以跳过检查
**解决方案**:
- 确保勾选了 "Include administrators" 选项

### 问题：CI 运行但不阻止合并
**解决方案**:
- 检查状态检查名称是否正确：`all-tests-passed`
- 确保勾选了 "Require status checks to pass before merging"

## 📚 更多资源

- [GitHub Branch Protection 官方文档](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches)
- [GitHub Actions 工作流语法](https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions)
- [关于必需状态检查](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches#require-status-checks-before-merging)

## ✅ 验证清单

配置完成后，确认以下内容：

- [ ] 分支保护规则已创建
- [ ] 要求 PR 才能合并
- [ ] 要求状态检查通过
- [ ] 添加了 `all-tests-passed` 状态检查
- [ ] 测试 PR 成功验证了配置
- [ ] 测试失败时无法合并
- [ ] 测试通过后可以合并
