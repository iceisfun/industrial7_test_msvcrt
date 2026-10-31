/* TC
 * id:    scn_neg_fwscanf_write_only_errno_probe
 * area:  scanf
 * kind:  negative
 * funcs: fwscanf fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fwscanf, fopen
 * note:  PROBE: errno after fwscanf on a stream opened "w" (write-only). Expected EBADF (the read-side buffer fill rejects the
 *        stream or _read maps ERROR_ACCESS_DENIED to EBADF); could be left untouched (0).
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
    CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(errno_ebadf); return tc_end(); }
