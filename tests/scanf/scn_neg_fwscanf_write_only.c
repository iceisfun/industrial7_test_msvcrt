/* TC
 * id:    scn_neg_fwscanf_write_only
 * area:  scanf
 * kind:  negative
 * funcs: fwscanf fopen fclose ferror
 * procs: fwscanf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.29.2.2 (input failure returns EOF); reading an output-only stream is undefined by ISO C, MS behaviour is probed
 * note:  PROBE: fwscanf/fwscanf_s on a "w" stream return EOF, leave outputs untouched and set ferror. errno (EBADF?) is in
 *        scn_neg_fwscanf_write_only_errno_probe.
 *        Wide counterpart of the fscanf write-only tests. Reading from a stream opened "w" must fail with EOF.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN "tc_scn_neg_fwscanf_write_only_" TC_CRT_DLL ".tmp"
typedef int (__cdecl *fw_t)(FILE *, const wchar_t *, ...);
static fw_t p_fwscanf_s;
#if TC_CRT_VER >= 140
#define CALL_FWS fwscanf_s
#else
#define CALL_FWS p_fwscanf_s
#endif
static const wchar_t *volatile vf;

TC_CASE(fwscanf_returns_eof)
{
    FILE *f = fopen(FN, "w"); int x = 77, n; CHECK(f != NULL); if (!f) return;
    vf = L"%d"; n = fwscanf(f, vf, &x);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(x, 77);
    fclose(f); remove(FN);
}
TC_CASE(fwscanf_s_returns_eof_buf_untouched)
{
    FILE *f = fopen(FN, "w"); wchar_t b[4]; int n, i; CHECK(f != NULL); if (!f) return;
    for (i = 0; i < 4; i++) b[i] = L'Z';
    if (!CALL_FWS) { fclose(f); remove(FN); return; }
    vf = L"%s"; CHECK(tc_iph_install()); tc_iph_count = 0;
    n = CALL_FWS(f, vf, b, (unsigned)4);
    CHECK_EQ_I(n, EOF); CHECK_EQ_U(b[0], L'Z'); CHECK_EQ_U(b[3], L'Z');
    fclose(f); remove(FN);
}
TC_CASE(ferror_set)
{
    FILE *f = fopen(FN, "w"); int x = 0; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ferror(f), 0);
    vf = L"%d"; (void)fwscanf(f, vf, &x);
    CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); p_fwscanf_s = (fw_t)tc_crt_proc("fwscanf_s"); TC_RUN(fwscanf_returns_eof); 
    TC_RUN(fwscanf_s_returns_eof_buf_untouched); TC_RUN(ferror_set);
    return tc_end();
}
