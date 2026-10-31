/* TC
 * id:    dat_mb_cur_max_data_probe
 * area:  data_exports
 * funcs: ___mb_cur_max_func
 * procs: __mb_cur_max
 * crt:   msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   (undocumented) exported int data variable __mb_cur_max
 * note:  Probe: the DLL exports the data variable __mb_cur_max and it equals ___mb_cur_max_func() (1 in the C locale). Skipped when not exported.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(data_equals_func)
{
    int *p = (int *)tc_crt_proc("__mb_cur_max");
    if (!p) TC_SKIP("__mb_cur_max not exported");
    CHECK_EQ_I(*p, ___mb_cur_max_func()); CHECK_EQ_I(*p, 1);
}
int main(void) { tc_begin(); TC_RUN(data_equals_func); return tc_end(); }
