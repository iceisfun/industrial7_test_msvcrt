/* TC
 * id:    gsf_neg_fseek_nolock_bad_args
 * area:  stdio_file
 * funcs: _fseek_nolock fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs fseek / _fseek_nolock: EINVAL, Parameter validation
 * note:  NULL stream faults; bad origin is not handler-validated in the _nolock variant. Bad-origin cases are asserted on ucrtbase only (older CRTs may just set errno).
 *        OBSERVED (Win10 22H2, class A, deviates from MS docs fseek origin validation): _fseek_nolock does NOT validate origin: tc_iph_count stays 0. On an unread
 *        stream origin 3 / -1 makes the OS seek fail (-1, errno EINVAL); with data buffered, (pos 3, offset 5, origin 3) returns 0 and ftell==8 (relative seek). Deterministic (10-byte file, one buffer fill).
 *        OBSERVED (Win10 22H2, class C): _fseek_nolock(NULL,..) is unvalidated and faults; null_stream now asserts 0xC0000005 from a child (baseline rc -1073741819).
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
    CHECK_EQ_I(tc_iph_count, 0); /* observed: no handler call; EINVAL comes from the OS seek failing */
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
    CHECK_EQ_I(tc_iph_count, 0); /* observed: no handler call; EINVAL comes from the OS seek failing */
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
/* Win10 22H2 ucrtbase: _fseek_nolock(NULL, ...) does no stream validation and faults (observed rc 0xc0000005,
 * baseline raw EXITCODE=-1073741819 right after negative_set). Windows behaviour, so a child asserts the AV. */
TC_CASE(null_stream)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("nullseek", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(position_after_bad_origin_buffered)
{
#if !defined(TC_UCRT)
    TC_SKIP("bad whence validation only asserted for ucrtbase");
#else
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    fgetc(fp); fgetc(fp); fgetc(fp);
    vpos = 5; vorg = 3;
    tc_iph_count = 0;
    /* observed: with data buffered, unvalidated origin 3 behaves like a relative seek: returns 0 and moves 3+5 = 8 */
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_I(ftell(fp), 8);
    CHECK_EQ_I(fgetc(fp), 0x38);
    CHECK_EQ_I(tc_iph_count, 0);
    fclose(fp); remove(fn);
#endif
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_install(); vpos = 0; vorg = SEEK_SET;
        (void)_fseek_nolock(nullfp, vpos, vorg);
        return 77;
    }
    TC_RUN(bad_origin_3); TC_RUN(bad_origin_minus1); TC_RUN(negative_set); TC_RUN(null_stream); TC_RUN(position_after_bad_origin_buffered);
    return tc_end();
}
