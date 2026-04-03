# mtkLogger

**mtkLogger** 是一个高性能、低延迟、零动态内存分配的 C++ 日志库，专为高并发、对延迟敏感的系统设计。

## ✨ 核心特性

- **极速异步日志**：基于无锁 MPSC 队列，单消费者后台线程，业务线程几乎零阻塞
- **零动态内存分配**：日志事件对象池 + 栈上固定缓冲区，关键路径杜绝 `malloc`/`new`
- **极低延迟**：单条日志入队 P99 < 100 纳秒
- **高吞吐量**：多生产者场景下 ≥ 100 万条/秒（每条 ~50 字节）
- **类型安全格式化**：集成 `fmt` 库，支持现代 C++ 格式化语法
- **可扩展 Sink**：支持控制台、文件滚动、自定义输出
- **RAII 风格**：资源自动管理，异常安全

## 📊 性能对标

| 指标               | mtkLogger (目标) |
| -----------------  | ---------------- |
| 吞吐量 (64 线程)   | ≥ 100 万条/秒    |
| 单日志入队延迟 P99 | < 100 ns         |
| 内存分配 (热路径)  | 零次             |


## 🚀 快速开始

### 环境要求
- C++20 编译器（GCC 11+ / Clang 14+）
- CMake 3.20+

### 构建与测试

```bash
git clone https://github.com/yourname/mtkLogger.git
cd mtkLogger
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
ctest --output-on-failure
