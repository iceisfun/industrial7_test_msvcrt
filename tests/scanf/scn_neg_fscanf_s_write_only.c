/* TC
 * id:    scn_neg_fscanf_s_write_only
 * area:  scanf
 * kind:  negative
 * funcs: fscanf_s fopen fclose
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.2 (input failure); MS docs fopen (mode w is write-only)
 * note:  Probes: fscanf_s on a stream opened with mode w does not succeed: returns EOF (and not 1). errno/handler not asserted.
 */
#include "tc.h"
#include <stdio.h>

#define FN "tc_scn_neg_fscanf_wo_" TC_CRT_DLL ".tmp"
TC_CASE(write_only) { FILE *f = fopen(FN, "w"); char a[4]; int n; CHECK(f != NULL); if (!f) return; tc_iph_install(); a[0] = 'Z'; n = fscanf_s(f, "%s", a, (unsigned)4); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, EOF); CHECK_EQ_I(a[0], 'Z'); }
int main(void) { tc_begin(); TC_RUN(write_only); return tc_end(); }
