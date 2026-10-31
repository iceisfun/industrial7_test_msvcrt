/* TC
 * id:    gsf_neg_fread_nolock_s_null_args
 * area:  stdio_file
 * funcs: _fread_nolock_s fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fread_s: NULL buffer / stream -> EINVAL via invalid parameter handler
 * note:  Counts are non-zero so the zero-count early exit is not involved.
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
static const char *fn = "tc_gsf_neg_fread_nolock_s_null_args_" TC_S1(TC_CRT_VER) ".tmp";
static void *volatile nullbuf;
static FILE *volatile nullfp;
static size_t volatile vsz, vcnt, vbuf;

TC_CASE(null_buffer)
{
    FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 1; vcnt = 4; vbuf = 8;
    CHECK_EQ_U(_fread_nolock_s(nullbuf, vbuf, vsz, vcnt, fp), 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
TC_CASE(null_stream)
{
    char b[8];
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 1; vcnt = 4; vbuf = 8;
    CHECK_EQ_U(_fread_nolock_s(b, vbuf, vsz, vcnt, nullfp), 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_bufsize_zero)
{
    FILE *fp; mk(fn, "ABCDEFGH", 8);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 1; vcnt = 4; vbuf = 0;
    CHECK_EQ_U(_fread_nolock_s(nullbuf, vbuf, vsz, vcnt, fp), 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(null_buffer); TC_RUN(null_stream); TC_RUN(null_buffer_bufsize_zero); return tc_end(); }
