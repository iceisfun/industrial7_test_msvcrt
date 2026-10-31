/* TC
 * id:    gsf_neg_probe_fread_nolock_s_writeonly
 * area:  stdio_file
 * funcs: _fread_nolock_s fopen fclose ferror
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fread_s
 * note:  PROBE: errno value (EBADF) and whether the invalid-parameter handler runs when reading from a write-only stream. Reported values are asserted: ret 0, ferror set, errno EBADF; iph count expected 0.
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
static const char *fn = "tc_gsf_neg_probe_fread_nolock_s_writeonly_" TC_S1(TC_CRT_VER) ".tmp";
static size_t volatile vsz, vcnt;

TC_CASE(wronly)
{
    char b[8]; FILE *fp = fopen(fn, "wb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 1; vcnt = 4;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 0);
    CHECK(ferror(fp) != 0);
    fclose(fp); remove(fn);
}
TC_CASE(wronly_errno_iph)
{
    char b[8]; FILE *fp = fopen(fn, "wb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 1; vcnt = 4;
    CHECK_EQ_U(_fread_nolock_s(b, sizeof b, vsz, vcnt, fp), 0);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(wronly); TC_RUN(wronly_errno_iph); return tc_end(); }
