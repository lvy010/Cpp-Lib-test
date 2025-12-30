// 简化版 RSA 教学示例（小素数，非生产安全）
// 编译：gcc -O2 -std=c11 rsa_demo.c -o rsa_demo
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef uint64_t u64;
typedef __uint128_t u128;

// 工具函数：乘模与幂模
static u64 mul_mod(u64 a, u64 b, u64 m) { return (u128)a * b % m; }
static u64 pow_mod(u64 a, u64 e, u64 m) {
    u64 r = 1 % m;
    while (e) {
        if (e & 1) r = mul_mod(r, a, m);
        a = mul_mod(a, a, m);
        e >>= 1;
    }
    return r;
}
static u64 gcd_u64(u64 a, u64 b) { return b ? gcd_u64(b, a % b) : a; }

// Miller-Rabin 基集合：覆盖 32/48bit 素性检测
static int is_probable_prime(u64 n) {
    if (n < 4) return n == 2 || n == 3;
    if ((n & 1) == 0) return 0;
    u64 d = n - 1, s = 0;
    while ((d & 1) == 0) d >>= 1, ++s;
    u64 bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        u64 a = bases[i];
        if (a % n == 0) continue;
        u64 x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int ok = 0;
        for (u64 r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { ok = 1; break; }
        }
        if (!ok) return 0;
    }
    return 1;
}

static u64 rand_u64(u64 lo, u64 hi) { return lo + (u64)((double)rand() / RAND_MAX * (double)(hi - lo)); }
static u64 random_prime(u64 lo, u64 hi) {
    u64 x;
    do { x = rand_u64(lo, hi) | 1ULL; } while (!is_probable_prime(x));
    return x;
}

// 扩展欧几里得求逆
static u64 mod_inv(u64 a, u64 m) {
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt; newt = t - q * newt; t = tmp;
        tmp = newr; newr = r - q * newr; r = tmp;
    }
    if (r > 1) return 0;       // 不可逆
    if (t < 0) t += m;
    return (u64)t;
}

typedef struct { u64 n, e, d; } RSAKey;

// 生成简化密钥对（使用 32/40bit 小素数，仅演示）
static RSAKey gen_keypair() {
    RSAKey k = {0};
    // 选取 ~26-28 bit 素数，保证 n 与 phi 不溢出 u64
    u64 p = random_prime(1ULL << 26, 1ULL << 28);
    u64 q = random_prime(1ULL << 26, 1ULL << 28);
    while (p == q) q = random_prime(1ULL << 26, 1ULL << 28);

    u64 n = p * q;
    u64 phi = (p - 1) * (q - 1);

    u64 e = 65537;
    while (gcd_u64(e, phi) != 1) e += 2;
    u64 d = mod_inv(e, phi);

    k.n = n; k.e = e; k.d = d;
    return k;
}

// 按字节加解密（无填充，仅演示）
static void rsa_encrypt(const RSAKey *k, const char *plain, u64 *out, size_t *out_len) {
    size_t len = strlen(plain);
    for (size_t i = 0; i < len; ++i)
        out[i] = pow_mod((unsigned char)plain[i], k->e, k->n);
    *out_len = len;
}
static void rsa_decrypt(const RSAKey *k, const u64 *in, size_t in_len, char *out) {
    for (size_t i = 0; i < in_len; ++i)
        out[i] = (char)pow_mod(in[i], k->d, k->n);
    out[in_len] = '\0';
}

int main() {
    srand((unsigned)time(NULL));

    RSAKey key = gen_keypair();
    printf("n = p*q = %llu\n", (unsigned long long)key.n);
    printf("e = %llu, d = %llu\n", (unsigned long long)key.e, (unsigned long long)key.d);

    const char *msg = "Hello RSA!";
    u64 cipher[1024];
    size_t clen = 0;
    rsa_encrypt(&key, msg, cipher, &clen);

    printf("Cipher (per byte):");
    for (size_t i = 0; i < clen; ++i) printf(" %llu", (unsigned long long)cipher[i]);
    printf("\n");

    char plain[1024];
    rsa_decrypt(&key, cipher, clen, plain);
    printf("Decrypted: %s\n", plain);
    return 0;
}

