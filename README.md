# CG_Final_SYSU

### 2025/1/1 17：20
实现了用于加载并绑定模型数据的 Model 类。

绘制模型时先在 MyGLWidget 类下添加 Model 成员变量，并在 initializeGL 中显式初始化，最后在 paintGL 中使用 draw 函数绘制。

### 2025/1/2 13：00

- 新增了文件夹resources，内有若干模型和贴图可参考
- 新增文件夹sample，[b站室内装潢模拟](https://www.bilibili.com/video/BV13v4y1f7X4/?spm_id_from=333.337.search-card.all.click&vd_source=11fd0556160bb5cc3994fd10610c3d4d)的源码，需要预先配置freeglut环境
