---
name: project-codegraph-auto-update
description: 每次新会话开始时自动检查 git 变更并重新初始化 Codegraph
metadata:
  type: project
---

每次开启本项目的新会话时，先执行 `git status --porcelain` 检查是否有未追踪的代码变更。
如果有变更（包括新增/修改/删除的文件），则自动运行 `codegraph init -i` 重新索引，确保 Codegraph 的知识图谱与最新源码保持同步。

如果 `git status` 显示干净（working tree clean），则跳过 codegraph init。

**Why:** Codegraph 索引是 SQLite 数据库文件（`.codegraph/codegraph.db`），不会自动跟随源码更新。如果不重新 init，模型读取到的符号和调用关系将是过期的，影响代码理解准确度。

**How to apply:** 本会话开始时根据 git status 决定是否执行 `codegraph init -i`。用户已在 2026-06-12 打开一次且 git status 干净，所以无需执行；后续新会话时照此规则判断。
