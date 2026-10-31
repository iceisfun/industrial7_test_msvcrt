/* TC
 * id:    gsf_fread_nolock_s_basic
 * area:  stdio_file
 * funcs: _fread_nolock_s fopen fwrite fclose feof
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs fread_s, _fread_nolock_s
 * note:  none
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
size_t __cdecl _fread_nolock_s(void *, size_t, size_t, size_t, FILE *);

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static void mk(const char *n, const char *d, size_t len)
{
    FILE *f = fopen(n, "wb");
    if (f) { fwrite(d, 1, len, f); fclose(f); }
}
static const char *fn = "tc_gsf_fread_nolock_s_basic_" TC_S1(TC_CRT_VER) ".tmp";
static size_t volatile vsz, vcnt;

TC_CASE(exact_fit)
{
    char b[8]; FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    vsz = 1; vcnt = 8;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 8);
    CHECK_MEM(b, "ABCDEFGH", 8);
    fclose(fp); remove(fn);
}
TC_CASE(bigger_buffer)
{
    char b[16]; FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    memset(b, 0x5a, sizeof b);
    vsz = 1; vcnt = 3;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 3);
    CHECK_MEM(b, "ABC", 3);
    { int i; for (i = 3; i < 16; i++) CHECK_EQ_I((unsigned char)b[i], 0x5a); }
    CHECK_EQ_I(ftell(fp), 3);
    fclose(fp); remove(fn);
}
TC_CASE(short_read_eof)
{
    char b[16]; FILE *fp; mk(fn, "ABCDEFG", 7);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    vsz = 2; vcnt = 8;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 3);  /* 7 bytes = 3 whole 2-byte elements */
    CHECK_MEM(b, "ABCDEF", 6);
    CHECK(feof(fp) != 0);
    fclose(fp); remove(fn);
}
TC_CASE(count_zero_and_size_zero)
{
    char b[8]; FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    memset(b, 0x33, sizeof b);
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    vsz = 1; vcnt = 0;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 0);
    vsz = 0; vcnt = 4;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    { int i; for (i = 0; i < 8; i++) CHECK_EQ_I((unsigned char)b[i], 0x33); }
    CHECK_EQ_I(ftell(fp), 0);
    fclose(fp); remove(fn);
}
TC_CASE(element_size_gt_1)
{
    unsigned char b[12]; FILE *fp; mk(fn, "abcdefghijkl", 12);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    vsz = 4; vcnt = 3;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 3);
    CHECK_MEM(b, "abcdefghijkl", 12);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(exact_fit); TC_RUN(bigger_buffer); TC_RUN(short_read_eof); TC_RUN(count_zero_and_size_zero); TC_RUN(element_size_gt_1); return tc_end(); }
