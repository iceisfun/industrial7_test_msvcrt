/* TC
 * id:    gsf_neg_fread_nolock_s_zero_args_ok
 * area:  stdio_file
 * funcs: _fread_nolock_s fopen fclose
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fread_s: size or count of 0 returns 0, buffer unchanged
 * note:  Negative in spirit: asserts the failure paths are NOT taken for zero size/count.
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
static const char *fn = "tc_gsf_neg_fread_nolock_s_zero_args_ok_" TC_S1(TC_CRT_VER) ".tmp";
static size_t volatile vsz, vcnt;

TC_CASE(size_zero)
{
    char b[8]; FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 0; vcnt = 4;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(errno, 0);
    fclose(fp); remove(fn);
}
TC_CASE(count_zero)
{
    char b[8]; FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 4; vcnt = 0;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(errno, 0);
    fclose(fp); remove(fn);
}
TC_CASE(buffer_untouched)
{
    char b[8]; FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    memset(b, 0x77, sizeof b);
    vsz = 0; vcnt = 4;
    _fread_nolock_s(b, sizeof b, vsz, vcnt, fp);
    vsz = 4; vcnt = 0;
    _fread_nolock_s(b, sizeof b, vsz, vcnt, fp);
    { int i; for (i = 0; i < 8; i++) CHECK_EQ_I((unsigned char)b[i], 0x77); }
    CHECK_EQ_I(ftell(fp), 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(size_zero); TC_RUN(count_zero); TC_RUN(buffer_untouched); return tc_end(); }
