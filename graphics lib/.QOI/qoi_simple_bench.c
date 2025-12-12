#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <png.h>

#define QOI_IMPLEMENTATION
#include "qoi.h"

// 高精度计时器
static double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// 读取PNG文件
unsigned char* read_png(const char* filename, int* width, int* height, int* channels) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(file);
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(file);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(file);
        return NULL;
    }

    png_init_io(png, file);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    int color_type = png_get_color_type(png, info);
    int bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    *channels = png_get_channels(png, info);
    int rowbytes = png_get_rowbytes(png, info);
    unsigned char* data = malloc(*height * rowbytes);

    png_bytep* row_pointers = malloc(*height * sizeof(png_bytep));
    for (int i = 0; i < *height; i++) {
        row_pointers[i] = data + i * rowbytes;
    }

    png_read_image(png, row_pointers);
    png_read_end(png, NULL);

    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(file);

    return data;
}

// QOI编码测试
void test_qoi_encode(const unsigned char* data, int width, int height, int channels, int runs) {
    printf("QOI编码测试 (%d次运行):\n", runs);
    
    double total_time = 0;
    int total_size = 0;
    
    for (int i = 0; i < runs; i++) {
        double start_time = get_time();
        
        int encoded_size;
        void* encoded = qoi_encode(data, &(qoi_desc){
            .width = width,
            .height = height,
            .channels = channels,
            .colorspace = QOI_SRGB
        }, &encoded_size);
        
        double end_time = get_time();
        double elapsed = (end_time - start_time) * 1000.0; // 转换为毫秒
        
        total_time += elapsed;
        total_size = encoded_size;
        
        printf("  运行 %d: %.2f ms, 大小: %d 字节\n", i+1, elapsed, encoded_size);
        
        free(encoded);
    }
    
    printf("  平均编码时间: %.2f ms\n", total_time / runs);
    printf("  编码后大小: %d 字节\n", total_size);
    printf("  压缩率: %.1f%%\n", (1.0 - (double)total_size / (width * height * channels)) * 100);
}

// QOI解码测试
void test_qoi_decode(const unsigned char* data, int width, int height, int channels, int runs) {
    printf("QOI解码测试 (%d次运行):\n", runs);
    
    // 先编码一次得到QOI数据
    int encoded_size;
    void* encoded = qoi_encode(data, &(qoi_desc){
        .width = width,
        .height = height,
        .channels = channels,
        .colorspace = QOI_SRGB
    }, &encoded_size);
    
    if (!encoded) {
        printf("  编码失败，无法进行解码测试\n");
        return;
    }
    
    double total_time = 0;
    
    for (int i = 0; i < runs; i++) {
        double start_time = get_time();
        
        qoi_desc desc;
        void* decoded = qoi_decode(encoded, encoded_size, &desc, channels);
        
        double end_time = get_time();
        double elapsed = (end_time - start_time) * 1000.0; // 转换为毫秒
        
        total_time += elapsed;
        
        printf("  运行 %d: %.2f ms\n", i+1, elapsed);
        
        free(decoded);
    }
    
    printf("  平均解码时间: %.2f ms\n", total_time / runs);
    printf("  解码速度: %.1f MP/s\n", (width * height / 1000000.0) / (total_time / runs / 1000.0));
    
    free(encoded);
}

// 测试单个文件
void benchmark_file(const char* filename, int runs) {
    printf("\n## 基准测试 %s -- %d 次运行\n\n", filename, runs);
    
    int width, height, channels;
    unsigned char* data = read_png(filename, &width, &height, &channels);
    
    if (!data) {
        printf("无法读取文件: %s\n", filename);
        return;
    }
    
    printf("图像信息: %dx%d, %d 通道, 原始大小: %d 字节\n\n", 
           width, height, channels, width * height * channels);
    
    test_qoi_encode(data, width, height, channels, runs);
    printf("\n");
    test_qoi_decode(data, width, height, channels, runs);
    
    free(data);
}

// 遍历目录中的PNG文件
void benchmark_directory(const char* dirname, int runs) {
    DIR* dir = opendir(dirname);
    if (!dir) {
        printf("无法打开目录: %s\n", dirname);
        return;
    }
    
    struct dirent* entry;
    int file_count = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // 普通文件
            const char* ext = strrchr(entry->d_name, '.');
            if (ext && strcasecmp(ext, ".png") == 0) {
                char fullpath[1024];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", dirname, entry->d_name);
                benchmark_file(fullpath, runs);
                file_count++;
            }
        }
    }
    
    closedir(dir);
    
    if (file_count == 0) {
        printf("在目录 %s 中没有找到PNG文件\n", dirname);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("用法: %s <运行次数> <目录路径>\n", argv[0]);
        printf("示例: %s 5 my_images/\n", argv[0]);
        return 1;
    }
    
    int runs = atoi(argv[1]);
    const char* dirname = argv[2];
    
    if (runs <= 0) {
        printf("运行次数必须是正整数\n");
        return 1;
    }
    
    printf("QOI基准测试工具\n");
    printf("测试目录: %s\n", dirname);
    printf("运行次数: %d\n\n", runs);
    
    benchmark_directory(dirname, runs);
    
    return 0;
} 