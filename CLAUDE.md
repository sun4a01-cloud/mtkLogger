# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

mtkLogger 是一个基于 Qt 的 C++ 日志库，目标是实现高性能、低延迟、零动态内存分配的异步日志系统。

## 构建与测试

### 环境要求
- C++20 编译器（GCC 11+ / Clang 14+）
- CMake 3.16+
- Qt5 或 Qt6 Core 模块

### 构建命令
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
```

### 运行示例程序
```bash
./mtkLogger_example
```

### CMake 选项
- `MTKLOGGER_BUILD_EXAMPLE=ON/OFF`: 是否构建示例程序（默认 ON）
- `-DCMAKE_BUILD_TYPE`: Debug/Release/RelWithDebInfo/MinSizeRel

## 核心架构

### 三层设计模式

1. **mtkLoggerManager（单例管理器）**
   - 负责创建和管理所有 Logger 实例
   - 实现基于点号分隔的层级继承（如 "net.tcp" 继承 "net"）
   - 线程安全的 Logger 注册表
   - 关键方法：`getLogger(category)`, `log(msg, category)`

2. **mtkAbstractLogger（抽象日志器）**
   - 每个 Logger 有独立的 category 名称和 Level 阈值
   - 管理多个 Appender（输出目标）
   - 层级继承：子 Logger 自动继承父 Logger 的 Appenders
   - 示例：Logger("net.tcp") 的消息会同时发送到 "net.tcp" 和 "net" 的所有 Appenders
   - 关键方法：`setLevel()`, `addAppender()`, `log()`

3. **mtkAbstractAppender（抽象输出器）**
   - 负责格式化和输出日志消息到具体目标
   - 每个 Appender 有独立的 Level 阈值（与 Logger 的阈值独立）
   - 内置实现：
     - `mtkConsoleAppender`: 控制台输出
     - `mtkFileAppender`: 文件输出
     - `mtkRollingFileAppender`: 滚动文件输出（支持大小限制和文件数量限制）
   - 关键方法：`processMessage()`, `format()`, `setDetailsLevel()`

### 消息流转

```
用户代码 → MessageLogger (RAII)
         ↓
    mtkLoggerManager::log()
         ↓
    解析层级关系 ("net.tcp" → ["net.tcp", "net"])
         ↓
    对每个 Logger 调用 log()
         ↓
    Logger 应用 Level 过滤
         ↓
    对每个 Appender 调用 doProcessMessage()
         ↓
    Appender 应用 detailsLevel 过滤
         ↓
    Appender::processMessage() 格式化并输出
```

### MessageLogger（消息对象）

- 使用 RAII 模式：析构时自动推送到 mtkLoggerManager
- 支持三种写入方式：
  - `write(QString)`: 直接赋值
  - `write(const char*, ...)`: printf 风格格式化
  - `write()`: 返回 QDebug 流，支持 `<<` 操作符
- 携带完整元数据：level, file, line, function, category, threadId, modulename, timestamp

## 关键设计决策

### 访问控制
- Appender 的 `doProcessMessage()` 和 `processMessage()` 是 protected
- 只有 AbstractLogger（通过 friend 声明）可以调用
- 这确保了消息处理流程的封装性

### 层级继承规则
- 使用点号分隔的 category 名称表示层级关系
- 子 Logger 自动继承父 Logger 的所有 Appenders
- 特殊情况：Logger("net") 不会自动继承 Logger("")（全局 Logger）
- 每个 Logger 和 Appender 都有独立的 Level 阈值，双重过滤

### 线程安全
- mtkLoggerManager 使用 QMutex 保护 Logger 注册表
- 每个 Appender 内部有 QReadWriteLock
- AbstractLogger 在调用 Appender 前可以获取锁

## 代码约定

### 命名空间
- 所有类型都在 `mtk::logger` 命名空间中
- 使用宏 `MTK_LOGGER_BEGIN_NAMESPACE` 和 `MTK_LOGGER_END_NAMESPACE`

### 类型别名
- 在示例代码中使用了简化别名（如 `mtkLogger`, `mtkAppender`, `Msg`）
- 实际类名为 `mtkAbstractLogger`, `mtkAbstractAppender`, `MessageLogger`

### 头文件包含
- 根目录和 Appender/ 子目录都在 include path 中
- 可以直接 `#include "mtkXxx.h"` 或 `#include "mtkConsoleAppender.h"`

## 扩展指南

### 添加新的 Appender
1. 继承 `mtkAbstractAppender`
2. 实现 `processMessage(const MessageLogger&)` 方法
3. 可选：重写 `format()` 方法自定义格式
4. 在 CMakeLists.txt 中添加源文件

### 添加新的 Logger 类型
1. 继承 `mtkAbstractLogger`
2. 实现 `processMessage(const MessageLogger&)` 方法
3. 定义 Appender 调度策略（如同步/异步）
