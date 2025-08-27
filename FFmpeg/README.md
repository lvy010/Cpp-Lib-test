# FFmpeg 测试和示例

这个目录包含了FFmpeg多媒体处理库的测试程序和相关资源，演示了如何使用FFmpeg API进行音视频文件处理。

## 📁 文件概览

| 文件名 | 类型 | 大小 | 功能描述 |
|--------|------|------|----------|
| `ffmpeg_buffer_test.c` | C源码 | 2.0KB | 内存缓冲区I/O测试程序 |
| `ffmpeg.png` | 图像文件 | 178KB | FFmpeg相关图像资源 |

## 🚀 功能特性

### `ffmpeg_buffer_test.c` - 内存缓冲区I/O测试

这个程序演示了如何使用FFmpeg的自定义I/O功能，将整个媒体文件加载到内存中进行处理。

#### 核心功能
- **内存文件读取**: 将完整的媒体文件加载到内存缓冲区
- **自定义I/O上下文**: 创建基于内存的AVIO上下文
- **媒体信息解析**: 解析并显示媒体文件的格式信息
- **资源管理**: 正确的内存分配和释放

#### 技术要点
```c
// 关键结构体 - 内存缓冲区数据
struct buffer_data {
    uint8_t *ptr;    // 数据指针
    size_t size;     // 数据大小
};

// 自定义读取回调函数
static int read_packet(void *opaque, uint8_t *buf, int buf_size);
```

#### 使用的FFmpeg API
- `av_malloc()` - FFmpeg内存分配
- `avio_alloc_context()` - 创建自定义I/O上下文
- `avformat_open_input()` - 打开媒体输入流
- `av_dump_format()` - 打印媒体格式信息
- `avformat_close_input()` - 关闭输入流
- `avio_context_free()` - 释放I/O上下文

## 📋 编译要求

### 依赖库
- **FFmpeg开发库** (libavformat, libavutil, libavcodec)
- **C编译器** (GCC 或 Clang)

### 安装FFmpeg开发库

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install libavformat-dev libavutil-dev libavcodec-dev
```

#### CentOS/RHEL
```bash
sudo yum install ffmpeg-devel
# 或者 (CentOS 8+)
sudo dnf install ffmpeg-devel
```

#### macOS
```bash
brew install ffmpeg
```

## 🔧 编译和运行

### 编译命令
```bash
# 基本编译
gcc -o ffmpeg_buffer_test ffmpeg_buffer_test.c -lavformat -lavutil

# 带调试信息
gcc -g -Wall -o ffmpeg_buffer_test ffmpeg_buffer_test.c -lavformat -lavutil

# 优化编译
gcc -O2 -o ffmpeg_buffer_test ffmpeg_buffer_test.c -lavformat -lavutil
```

### 运行示例
```bash
# 运行程序，分析媒体文件
./ffmpeg_buffer_test input_video.mp4

# 测试音频文件
./ffmpeg_buffer_test audio_sample.mp3

# 测试图像文件
./ffmpeg_buffer_test image.jpg
```

### 预期输出
```
Input #0, mov,mp4,m4a,3gp,3g2,mj2, from 'input_video.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2avc1mp41
    encoder         : Lavf58.29.100
  Duration: 00:00:10.50, start: 0.000000, bitrate: 1000 kb/s
    Stream #0:0(und): Video: h264 (High) (avc1 / 0x31637661), yuv420p, 1920x1080, 900 kb/s, 30 fps, 30 tbr, 15360 tbn, 60 tbc (default)
    Metadata:
      handler_name    : VideoHandler
    Stream #0:1(und): Audio: aac (LC) (mp4a / 0x6134706D), 44100 Hz, stereo, fltp, 96 kb/s (default)
    Metadata:
      handler_name    : SoundHandler
```

## 💡 核心概念解析

### 1. 内存I/O vs 文件I/O
- **传统方式**: FFmpeg直接从文件系统读取
- **内存方式**: 先将文件加载到内存，然后从内存读取
- **优势**: 更快的访问速度，适合小文件和特殊场景

### 2. 自定义I/O上下文
```c
AVIOContext *avio_ctx = avio_alloc_context(
    avio_ctx_buffer,  // I/O缓冲区
    4096,            // 缓冲区大小
    0,               // 写入标志 (0=只读)
    &bd,             // 用户数据
    read_packet,     // 读取回调
    NULL,            // 写入回调
    NULL             // 定位回调
);
```

### 3. 内存管理策略
- 使用`av_malloc()`分配FFmpeg兼容的内存
- 正确释放所有分配的资源
- 避免内存泄漏和双重释放

## 🎯 应用场景

1. **嵌入式媒体**: 将媒体文件嵌入到程序中
2. **网络流处理**: 处理从网络接收的媒体数据
3. **内存优化**: 减少磁盘I/O操作
4. **实时处理**: 对内存中的媒体数据进行实时分析
5. **测试和调试**: 验证FFmpeg功能和性能

## 🔍 扩展示例

### 支持的媒体格式
- **视频**: MP4, AVI, MKV, MOV, FLV, WebM
- **音频**: MP3, AAC, WAV, OGG, FLAC
- **图像**: JPEG, PNG, BMP, GIF, TIFF
- **流媒体**: RTMP, HLS, DASH

### 可能的扩展功能
```c
// 添加写入回调实现双向I/O
static int write_packet(void *opaque, uint8_t *buf, int buf_size);

// 添加定位回调支持随机访问
static int64_t seek_callback(void *opaque, int64_t offset, int whence);
```

## 🛠️ 调试技巧

### 常见编译错误
```bash
# 错误: 找不到头文件
# 解决: 安装开发库或指定头文件路径
gcc -I/usr/local/include/ffmpeg ...

# 错误: 链接库失败
# 解决: 确保库文件存在并指定库路径
gcc -L/usr/local/lib ...
```

### 运行时调试
```bash
# 启用FFmpeg详细日志
export FFREPORT=file=ffmpeg_log.txt:level=debug

# 使用valgrind检查内存泄漏
valgrind --leak-check=full ./ffmpeg_buffer_test input.mp4

# 使用gdb调试
gdb ./ffmpeg_buffer_test
```

## 📚 相关资源

### 官方文档
- [FFmpeg官方文档](https://ffmpeg.org/documentation.html)
- [FFmpeg API文档](https://ffmpeg.org/doxygen/trunk/)
- [AVIO文档](https://ffmpeg.org/doxygen/trunk/group__lavf__io.html)

### 学习资源
- [FFmpeg编程指南](https://github.com/leandromoreira/ffmpeg-libav-tutorial)
- [音视频处理基础](https://github.com/FFmpeg/FFmpeg/tree/master/doc/examples)

## ⚠️ 注意事项

1. **版本兼容性**: 不同FFmpeg版本的API可能有差异
2. **内存限制**: 大文件可能导致内存不足
3. **错误处理**: 务必检查所有FFmpeg函数的返回值
4. **资源释放**: 确保正确释放所有FFmpeg资源
5. **线程安全**: FFmpeg某些函数不是线程安全的

## 🐛 故障排除

### 常见问题

**问题**: 程序崩溃或段错误
```bash
# 解决方案
1. 检查输入文件是否存在和可读
2. 验证内存分配是否成功
3. 确保正确的资源释放顺序
```

**问题**: 无法识别媒体格式
```bash
# 解决方案
1. 确认文件不损坏
2. 检查FFmpeg是否支持该格式
3. 更新FFmpeg到最新版本
```

**问题**: 内存使用过高
```bash
# 解决方案
1. 对大文件使用流式处理
2. 分块读取而非一次性加载
3. 及时释放不需要的内存
```

---

*这个示例程序展示了FFmpeg自定义I/O的功能，为更复杂的音视频处理应用提供了基础。*
