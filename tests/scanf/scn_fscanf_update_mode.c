/* TC
 * id:    scn_fscanf_update_mode
 * area:  scanf
 * funcs: fscanf fprintf fseek fopen fclose
 * crt:   all
 * tier:  p2
 * conf:  standard
 * ref:   C11 7.21.5.3 (w+ update), 7.21.5.3p7 (fseek between write and read)
 * note:  fprintf is a helper only.
 */
#include "tc.h"
#include <stdio.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_scn_fscanf_update_mode_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(write_then_read)
{
    FILE *f = fopen(F, "w+"); int a = 0, b = 0, r; CHECK(f != NULL); if (!f) return;
    fprintf(f, "7 8");
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    r = fscanf(f, "%d %d", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 7); CHECK_EQ_I(b, 8);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(write_then_read); return tc_end(); }
