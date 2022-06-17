# software renderer

## 目标 
- 一个真正的软件渲染器
- 学习初具规模的 C++ 11 项目的编写
- 尽可能的面向对象
- 面向windows平台，因此需要学习windows api的相关知识
- 尽可能的减少依赖，能造轮子就造轮子
- 生成有价值的文档内容
  - 图形学笔记
  - 数学知识
  - 开发文档
  - 统一的注释

## feature

- windows
- 无依赖
  - 标准库
  - windows api
- 可编程的渲染管线
  - 支持shader
- 齐次裁剪
- 透视插值
- 背面剔除
- 深度测试
- 透明混合*
- 天空盒*
- 骨骼动画*
- 切线空间法向贴图*
  - Tangent space normal mapping
- 阴影贴图
  - shadow mapping
- ACES tone mapping算法*
  - ???
- Blinn-Phong 反射模型
- PBR*
- Image-based rendering
- 环绕式相机
- 材质查看器

- 参考
  - https://zauonlok.github.io/renderer/
  - https://www.zhihu.com/question/24786878
  - https://zhuanlan.zhihu.com/p/372125256