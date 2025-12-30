// 实验六：ECC 加密与解密算法（教学演示版）
// 特点：自包含纯 C，使用 61-bit 素数域上的简化曲线 y^2 = x^3 + 7。
// 安全性：仅为学习演示，未做标准安全防护（未使用标准曲线/填充/认证）。
// 编译运行：
//   gcc -O2 -std=c11 ecc_demo.c -o ecc_demo
//   ./ecc_demo
// 运行后会生成示例加解密输出。

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef unsigned __int128 u128;
typedef uint64_t u64;

// 选用 61-bit Mersenne 素数 p = 2^61 - 1
static const u64 P = 2305843009213693951ULL;
// 曲线参数 y^2 = x^3 + a*x + b (a=0,b=7)
static const u64 A = 0;
static const u64 B = 7;

static u64 add_mod(u64 a, u64 b) {
    u64 r = a + b;
    if (r >= P || r < a) r -= P;
    return r;
}
static u64 sub_mod(u64 a, u64 b) {
    return a >= b ? a - b : P - (b - a);
}
static u64 mul_mod(u64 a, u64 b) {
    return (u64)((u128)a * b % P);
}
static u64 pow_mod(u64 a, u64 e) {
    u64 r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a);
        a = mul_mod(a, a);
        e >>= 1;
    }
    return r;
}
static u64 inv_mod(u64 a) { return pow_mod(a, P - 2); } // Fermat 逆元

// Tonelli-Shanks 求平方根：返回 y 使 y^2=n (mod P)，不存在返回 0
static u64 mod_sqrt(u64 n) {
    if (n == 0) return 0;
    if (P % 4 == 3) return pow_mod(n, (P + 1) / 4);
    // 通用 Tonelli-Shanks
    u64 q = P - 1, s = 0;
    while ((q & 1) == 0) { q >>= 1; ++s; }
    u64 z = 2;
    while (pow_mod(z, (P - 1) / 2) != P - 1) ++z;
    u64 m = s;
    u64 c = pow_mod(z, q);
    u64 t = pow_mod(n, q);
    u64 r = pow_mod(n, (q + 1) / 2);
    while (t != 1) {
        u64 i = 1;
        u64 t2i = mul_mod(t, t);
        while (t2i != 1) { t2i = mul_mod(t2i, t2i); ++i; }
        u64 b = pow_mod(c, 1ULL << (m - i - 1));
        c = mul_mod(b, b);
        t = mul_mod(t, c);
        r = mul_mod(r, b);
        m = i;
    }
    return r;
}

typedef struct {
    u64 x, y;
    int inf; // 1 表示无穷远点
} Point;

static int is_on_curve(const Point *p) {
    if (p->inf) return 1;
    u64 lhs = mul_mod(p->y, p->y);
    u64 rhs = add_mod(mul_mod(mul_mod(p->x, p->x), p->x), B);
    return lhs == rhs;
}

static Point point_add(const Point *p, const Point *q) {
    if (p->inf) return *q;
    if (q->inf) return *p;
    if (p->x == q->x) {
        if ((p->y + q->y) % P == 0) return (Point){0, 0, 1};
        // 点加退化为倍点
    }
    u64 lambda;
    if (p->x == q->x && p->y == q->y) { // 倍点
        u64 num = mul_mod(3 * mul_mod(p->x, p->x) + A % P, 1);
        u64 den = inv_mod((2 * p->y) % P);
        lambda = mul_mod(num % P, den);
    } else { // 普通加
        u64 num = sub_mod(q->y, p->y);
        u64 den = inv_mod(sub_mod(q->x, p->x));
        lambda = mul_mod(num, den);
    }
    u64 xr = sub_mod(sub_mod(mul_mod(lambda, lambda), p->x), q->x);
    u64 yr = sub_mod(mul_mod(lambda, sub_mod(p->x, xr)), p->y);
    Point r = {xr, yr, 0};
    return r;
}

static Point scalar_mul(u64 k, const Point *p) {
    Point r = {0, 0, 1};
    Point base = *p;
    while (k) {
        if (k & 1) r = point_add(&r, &base);
        base = point_add(&base, &base);
        k >>= 1;
    }
    return r;
}

// 寻找曲线上的一个基点 G
static Point find_basepoint() {
    for (u64 x = 2; x < 100000; ++x) {
        u64 rhs = add_mod(mul_mod(mul_mod(x, x), x), B);
        if (pow_mod(rhs, (P - 1) / 2) == 1) { // 是二次剩余
            u64 y = mod_sqrt(rhs);
            if (y) {
                Point g = {x, y, 0};
                if (is_on_curve(&g)) return g;
            }
        }
    }
    // 理论上不会到这里
    return (Point){0, 0, 1};
}

// 简单 KDF：把 shared.x 展开为字节流
static void kdf_u64(u64 x, uint8_t *out, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        out[i] = (uint8_t)(x & 0xFF);
        x = (x >> 8) | (x << 56); // 旋转，避免全零
    }
}

// ECC 加密：ElGamal 变体，使用共享秘钥 XOR 明文
static void ecc_encrypt(const Point *pub, const Point *G, const uint8_t *msg, size_t mlen) {
    u64 r = ((u64)rand() << 32) ^ rand();
    r %= P;
    if (r == 0) r = 1;
    Point C1 = scalar_mul(r, G);
    Point S = scalar_mul(r, pub);
    uint8_t keystream[4096];
    if (mlen > sizeof(keystream)) {
        fprintf(stderr, "消息过长，示例仅支持 <= %zu 字节\n", sizeof(keystream));
        exit(1);
    }
    kdf_u64(S.x, keystream, mlen);
    printf("C1 = (%llu, %llu)\n", (unsigned long long)C1.x, (unsigned long long)C1.y);
    printf("Ciphertext bytes: ");
    for (size_t i = 0; i < mlen; ++i) {
        uint8_t c = msg[i] ^ keystream[i];
        printf("%02X", c);
        keystream[i] = c; // reuse buffer to store cipher for demo
    }
    printf("\n");
    // 演示解密
    Point S_dec = scalar_mul(123456789ULL, &C1); // placeholder
    (void)S_dec;
}

// ECC 解密：输入 C1 与密文字节，恢复明文
static void ecc_decrypt(const Point *priv_pub_dummy, u64 priv, const Point *C1, uint8_t *cipher, size_t clen) {
    (void)priv_pub_dummy; // 未用
    Point S = scalar_mul(priv, C1);
    uint8_t keystream[4096];
    kdf_u64(S.x, keystream, clen);
    printf("Decrypted: ");
    for (size_t i = 0; i < clen; ++i) {
        uint8_t p = cipher[i] ^ keystream[i];
        printf("%c", (char)p);
    }
    printf("\n");
}

int main() {
    srand((unsigned)time(NULL));
    Point G = find_basepoint();
    if (G.inf) {
        fprintf(stderr, "未找到基点，退出\n");
        return 1;
    }
    printf("Curve: y^2 = x^3 + %llu over p = %llu\n", (unsigned long long)B, (unsigned long long)P);
    printf("Base point G = (%llu, %llu)\n", (unsigned long long)G.x, (unsigned long long)G.y);

    // 生成密钥对
    u64 d = ((u64)rand() << 32) ^ rand();
    d %= P;
    if (d == 0) d = 1;
    Point Q = scalar_mul(d, &G);
    printf("Private d = %llu\n", (unsigned long long)d);
    printf("Public  Q = (%llu, %llu)\n", (unsigned long long)Q.x, (unsigned long long)Q.y);

    // 演示加解密
    const char *msg = "ECC demo message";
    size_t mlen = strlen(msg);
    uint8_t cipher[256];
    // 加密
    u64 r = ((u64)rand() << 32) ^ rand();
    r %= P; if (r == 0) r = 1;
    Point C1 = scalar_mul(r, &G);
    Point S = scalar_mul(r, &Q);
    kdf_u64(S.x, cipher, mlen);
    for (size_t i = 0; i < mlen; ++i) cipher[i] ^= (uint8_t)msg[i];

    printf("Ciphertext bytes: ");
    for (size_t i = 0; i < mlen; ++i) printf("%02X", cipher[i]);
    printf("\n");

    // 解密
    Point S2 = scalar_mul(d, &C1);
    uint8_t plain[256];
    kdf_u64(S2.x, plain, mlen);
    for (size_t i = 0; i < mlen; ++i) plain[i] ^= cipher[i];
    plain[mlen] = '\0';
    printf("Decrypted: %s\n", plain);
    return 0;
}

