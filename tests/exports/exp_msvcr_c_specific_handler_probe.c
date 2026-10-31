/* TC
 * id:    exp_msvcr_c_specific_handler_probe
 * area:  exports
 * funcs: 
 * procs: __C_specific_handler
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   x64 SEH runtime (__C_specific_handler)
 * note:  PROBE: x64 builds of msvcr80..120 export __C_specific_handler by name. x64 only; on x86 builds this export does not exist.
 */
#include "tc.h"

TC_CASE(specific_handler_exported) { if (!tc_crt_proc("__C_specific_handler")) tc_fail_expr(__FILE__, __LINE__, "__C_specific_handler"); }

int main(void)
{
    tc_begin();
    TC_RUN(specific_handler_exported);
    return tc_end();
}
