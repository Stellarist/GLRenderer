# GLRenderer TODO List

## 🔥 高优先级
### 相机控制问题
- [ ] Camera movement rotation problem
  - 问题：鼠标移动后相机位置和旋转不同步
  - 文件：src/main.cpp - mouseCallback() 和 updateCameraTransform()
  - 原因：全局变量 camera_pos 与 Camera Transform 状态不一致
  - 解决：统一相机状态管理，或实现 FirstPersonCamera 类

### 资源管理问题  
- [ ] Duplicate meshes loading problem
  - 问题：同一模型被重复加载到内存
  - 文件：src/resource/AssetManager.cpp - loadMesh()
  - 影响：内存浪费，加载性能差
  - 解决：完善缓存机制，检查 loaded_meshes 映射逻辑

## 📋 中优先级
### 场景序列化
- [ ] Scene Serialization System
  - [ ] 选择序列化库（Cereal vs nlohmann/json）
  - [ ] 实现基础 Component 序列化
    - [ ] Transform 序列化
    - [ ] Camera 序列化  
    - [ ] Light 序列化
    - [ ] Material 序列化
  - [ ] 实现 Scene 整体序列化
    - [ ] Node 关系保存/恢复
    - [ ] Component 关联管理
  - [ ] 文件格式设计 (.scene 扩展名)
  - [ ] 版本兼容性考虑

## 🐛 已知 Bug 修复
- [ ] GLMesh 纹理绑定问题
  - 文件：src/graphics/GLMesh.cpp - draw()
  - 问题：纹理 uniform 名称映射错误
  - 相关：uploadMesh 函数中的纹理处理逻辑
- [ ] GraphicsManager 纹理重复上传
  - 文件：src/resource/GraphicsManager.cpp - uploadMesh()
  - 问题：gl_meshes[&mesh].emplace_back() 位置错误

## 💡 优化和改进（有时间再做）
- [ ] 性能优化
  - [ ] 批量渲染支持
  - [ ] 视锥剔除
  - [ ] Mipmap 生成优化
- [ ] 代码质量
  - [ ] 添加单元测试
  - [ ] 完善错误处理
  - [ ] 代码文档补充
- [ ] 功能扩展
  - [ ] 支持更多光照类型
  - [ ] 材质编辑器基础功能
  - [ ] glTF 导入支持

## ✅ 近期已完成
- [x] 基础 Scene/Node/Component 架构
- [x] AssetManager 和 GraphicsManager 单例
- [x] OpenGL 渲染管线基础实现
- [x] Assimp 模型加载集成

---
## 📝 下次开发重点
1. 优先修复相机问题 - 影响基础使用体验
2. 解决重复加载 - 影响内存和性能  
3. 开始场景序列化设计 - 为后续功能打基础
