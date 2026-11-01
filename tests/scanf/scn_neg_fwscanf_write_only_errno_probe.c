/* TC
 * id:    scn_neg_fwscanf_write_only_errno_probe
 * area:  scanf
 * kind:  negative
 * funcs: fwscanf fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  observed
 * ref:   MS docs fwscanf, fopen
 * note:  PROBE: errno after fwscanf on a stream opened "w" (write-only). Expected EBADF (the read-side buffer fill rejects the
 *        stream or _read maps ERROR_ACCESS_DENIED to EBADF); could be left untouched (0).
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): fwscanf on a write-only stream returns EOF, errno untouched (0), no handler call. Stable (2 runs).
 *        Gated: the observed values apply to ucrtbase only (#ifdef TC_UCRT); other CRTs keep the original (unverified, probe) expectation until they are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN "tc_scn_neg_fwscanf_wo_errno_" TC_CRT_DLL ".tmp"
static const wchar_t *volatile vf;
TC_CASE(errno_ebadf)
{
    FILE *f = fopen(FN, "w"); int x = 77; CHECK(f != NULL); if (!f) return;
    CHECK(tc_iph_install()); vf = L"%d"; errno = 0; (void)fwscanf(f, vf, &x);
#ifdef TC_UCRT
    CHECK_EQ_I(errno, 0);
#else
    CHECK_EQ_I(errno, EBADF);
#endif
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(errno_ebadf); return tc_end(); }
