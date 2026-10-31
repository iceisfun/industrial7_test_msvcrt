/* TC
 * id:    gsf_neg_probe_fread_nolock_s_buffer_state
 * area:  stdio_file
 * funcs: _fread_nolock_s fopen fclose
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fread_s (silent on buffer state)
 * note:  PROBE: whether the destination buffer is zero-filled (memset) after a buffer-too-small failure, and that the stream position is unchanged. Believed from UCRT fread_s behaviour; older CRTs may differ.
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
static const char *fn = "tc_gsf_neg_probe_fread_nolock_s_buffer_state_" TC_S1(TC_CRT_VER) ".tmp";
static size_t volatile vsz, vcnt, vbuf;

TC_CASE(buffer_zero_filled)
{
    char b[16]; FILE *fp; mk(fn, "ABCDEFGHIJKL", 12);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    memset(b, 0x5a, sizeof b);
    vsz = 1; vcnt = 12; vbuf = 8;
    CHECK_EQ_U(_fread_nolock_s(b, vbuf, vsz, vcnt, fp), 0);
    { int i; for (i = 0; i < 8; i++) CHECK_EQ_I((unsigned char)b[i], 0); }
    { int i; for (i = 8; i < 16; i++) CHECK_EQ_I((unsigned char)b[i], 0x5a); }
    fclose(fp); remove(fn);
}
TC_CASE(file_position_after_failure)
{
    char b[16]; FILE *fp; mk(fn, "ABCDEFGHIJKL", 12);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    vsz = 1; vcnt = 12; vbuf = 8;
    _fread_nolock_s(b, vbuf, vsz, vcnt, fp);
    CHECK_EQ_I(ftell(fp), 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(buffer_zero_filled); TC_RUN(file_position_after_failure); return tc_end(); }
