/* TC
 * id:    gsf_neg_fread_nolock_s_buffer_small
 * area:  stdio_file
 * funcs: _fread_nolock_s fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fread_s: buffer too small -> ERANGE + invalid parameter handler
 * note:  Buffer contents and stream position after failure are not asserted here (see the probe test).
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
static const char *fn = "tc_gsf_neg_fread_nolock_s_buffer_small_" TC_S1(TC_CRT_VER) ".tmp";
static size_t volatile vsz, vcnt, vbuf;

TC_CASE(small_buffer)
{
    char b[16]; FILE *fp; mk(fn, "ABCDEFGHIJKL", 12);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 1; vcnt = 8; vbuf = 4;
    CHECK_EQ_U(_fread_nolock_s(b, vbuf, vsz, vcnt, fp), 0);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
TC_CASE(small_buffer_elem)
{
    char b[16]; FILE *fp; mk(fn, "ABCDEFGHIJKL", 12);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vsz = 4; vcnt = 2; vbuf = 7;
    CHECK_EQ_U(_fread_nolock_s(b, vbuf, vsz, vcnt, fp), 0);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
TC_CASE(boundary_exact_minus_one)
{
    char b[16]; FILE *fp; mk(fn, "ABCDEFGHIJKL", 12);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; vsz = 4; vcnt = 2; vbuf = 7;
    CHECK_EQ_U(_fread_nolock_s(b, vbuf, vsz, vcnt, fp), 0);
    CHECK_EQ_I(tc_iph_count, 1);
    vbuf = 8; rewind(fp);
    CHECK_EQ_U(_fread_nolock_s(b, vbuf, vsz, vcnt, fp), 2);
    CHECK_MEM(b, "ABCDEFGH", 8);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(small_buffer); TC_RUN(small_buffer_elem); TC_RUN(boundary_exact_minus_one); return tc_end(); }
