/* TC
 * id:    scn_neg_fscanf_writeonly_probe
 * area:  scanf
 * kind:  negative
 * funcs: fscanf fopen fclose
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fscanf
 * note:  Probes that fscanf on a stream opened "w" (write-only) reads nothing and returns EOF (input failure) with the
 *        variable untouched. Whether errno is EBADF and whether the invalid-parameter handler runs is deliberately
 *        not asserted.
 */
#include "tc.h"
#include <stdio.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_scn_neg_fscanf_writeonly_probe_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(writeonly)
{
    FILE *f = fopen(F, "w"); int a = 5, r; CHECK(f != NULL); if (!f) return;
    tc_iph_install();
    r = fscanf(f, "%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 5);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(writeonly); return tc_end(); }
