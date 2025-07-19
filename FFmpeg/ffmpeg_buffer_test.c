#include <stdio.h>
#include <libavformat/avformat.h>
#include <libavutil/mem.h>

struct buffer_data {
    uint8_t *ptr;
    size_t size;
};

// 内存读取回调
static int read_packet(void *opaque, uint8_t *buf, int buf_size) {
    struct buffer_data *bd = (struct buffer_data *)opaque;
    buf_size = FFMIN(buf_size, bd->size);
    if (!buf_size)
        return AVERROR_EOF;
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;
    return buf_size;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: %s <输入媒体文件>\n", argv[0]);
        return 1;
    }
    // av_register_all(); // 新版FFmpeg无需调用，已废弃

    // 读取整个文件到内存
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("无法打开文件");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    size_t filesize = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *file_buffer = av_malloc(filesize);
    fread(file_buffer, 1, filesize, f);
    fclose(f);

    struct buffer_data bd = { file_buffer, filesize };

    // 创建自定义IO上下文
    uint8_t *avio_ctx_buffer = av_malloc(4096);
    AVIOContext *avio_ctx = avio_alloc_context(
        avio_ctx_buffer, 4096, 0, &bd, read_packet, NULL, NULL
    );

    AVFormatContext *fmt_ctx = avformat_alloc_context();
    fmt_ctx->pb = avio_ctx;
    fmt_ctx->flags |= AVFMT_FLAG_CUSTOM_IO;

    // 打开输入流
    int ret = avformat_open_input(&fmt_ctx, NULL, NULL, NULL);
    if (ret < 0) {
        char errbuf[128];
        av_strerror(ret, errbuf, sizeof(errbuf));
        printf("avformat_open_input 失败: %s\n", errbuf);
        av_free(file_buffer);
        av_free(avio_ctx_buffer);
        avio_context_free(&avio_ctx);
        avformat_free_context(fmt_ctx);
        return 1;
    }

    // 打印媒体信息
    av_dump_format(fmt_ctx, 0, argv[1], 0);

    // 释放资源
    avformat_close_input(&fmt_ctx);
    avio_context_free(&avio_ctx);
    av_free(file_buffer);

    return 0;
} 