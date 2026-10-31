/* TC

 * id:    cpp_neg_get_terminate_absent_old_probe
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: _get_terminate _get_unexpected _get_purecall_handler
 * crt:   msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_terminate / _get_unexpected / _get_purecall_handler (Visual Studio 2008+)
 * note:  PROBE: documents that the getter APIs are NOT exported by the oldest CRTs (they were added with VS2008 / msvcr90).
 */
#include "tc.h"
#include <errno.h>
TC_CASE(no_get_terminate) { CHECK(tc_crt_proc("_get_terminate") == NULL); }
TC_CASE(no_get_unexpected) { CHECK(tc_crt_proc("_get_unexpected") == NULL); }
TC_CASE(no_get_purecall) { CHECK(tc_crt_proc("_get_purecall_handler") == NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(no_get_terminate); TC_RUN(no_get_unexpected); TC_RUN(no_get_purecall);
    return tc_end();
}
