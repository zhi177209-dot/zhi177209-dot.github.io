# Personal Project Blog

这是一个适合部署到 GitHub Pages 的静态个人项目博客。

## 当前结构

- `index.html`
  首页，只展示项目看板和个人介绍。
- `project-workspace-climate.html`
  “工作区温湿度检测系统”项目详情页。
- `style.css`
  全站样式文件。
- `assets/`
  页面图片资源。
- `SHT21/`
  本地温湿度检测嵌入式工程源码。

## 部署方式

推荐使用 GitHub Pages 的 `main` 分支根目录发布方式：

1. 将当前目录初始化为 Git 仓库。
2. 推送到 GitHub 仓库。
3. 在 GitHub 仓库设置中启用 Pages。
4. 选择 `Deploy from a branch`。
5. Branch 选择 `main`，Folder 选择 `/ (root)`。

部署完成后，首页入口是 `index.html`，项目详情页通过首页卡片进入。
