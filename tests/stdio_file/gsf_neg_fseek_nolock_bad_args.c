/* TC
 * id:    gsf_neg_fseek_nolock_bad_args
 * area:  stdio_file
 * funcs: _fseek_nolock fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fseek / _fseek_nolock: EINVAL, Parameter validation
 * note:  NULL stream and bad origin go through the invalid-parameter handler. Bad-origin cases are asserted on ucrtbase only (older CRTs may just set errno).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
int __cdecl _fseek_nolock(FILE *, long, int);
__int64 __cdecl _ftelli64_nolock(FILE *);

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static void mk(const char *n, const char *d, size_t len)
{
    FILE *f = fopen(n, "wb");
    if (f) { fwrite(d, 1, len, f); fclose(f); }
}
static const char *fn = "tc_gsf_neg_fseek_nolock_bad_args_" TC_S1(TC_CRT_VER) ".tmp";
static FILE *volatile nullfp;
static volatile int vorg;
static volatile long vpos;

TC_CASE(bad_origin_3)
{
#if !defined(TC_UCRT)
    TC_SKIP("bad whence validation only asserted for ucrtbase");
#else
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vpos = 0; vorg = 3;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
#endif
}
TC_CASE(bad_origin_minus1)
{
#if !defined(TC_UCRT)
    TC_SKIP("bad whence validation only asserted for ucrtbase");
#else
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vpos = 0; vorg = -1;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
#endif
}
TC_CASE(negative_set)
{
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    fgetc(fp); fgetc(fp);
    errno = 0; vpos = -1; vorg = SEEK_SET;
    CHECK(_fseek_nolock(fp, vpos, vorg) != 0);
    CHECK_EQ_I(errno, EINVAL);
    fclose(fp); remove(fn);
}
TC_CASE(null_stream)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vpos = 0; vorg = SEEK_SET;
    CHECK_EQ_I(_fseek_nolock(nullfp, vpos, vorg), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(position_unchanged_after_bad_origin)
{
#if !defined(TC_UCRT)
    TC_SKIP("bad whence validation only asserted for ucrtbase");
#else
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    fgetc(fp); fgetc(fp); fgetc(fp);
    vpos = 5; vorg = 3;
    CHECK(_fseek_nolock(fp, vpos, vorg) != 0);
    CHECK_EQ_I(ftell(fp), 3);
    CHECK_EQ_I(fgetc(fp), 0x33);
    fclose(fp); remove(fn);
#endif
}
int main(void) { tc_begin(); TC_RUN(bad_origin_3); TC_RUN(bad_origin_minus1); TC_RUN(negative_set); TC_RUN(null_stream); TC_RUN(position_unchanged_after_bad_origin); return tc_end(); }
