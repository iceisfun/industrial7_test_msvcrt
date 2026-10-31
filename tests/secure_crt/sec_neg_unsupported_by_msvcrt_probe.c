/* TC
 * id:    sec_neg_unsupported_by_msvcrt_probe
 * area:  secure_crt
 * funcs:
 * procs: set_constraint_handler_s memset_s strnlen_s abort_handler_s ignore_handler_s strerrorlen_s
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   ISO C11 Annex K (optional); MS docs: the _s functions are not Annex K conformant
 * note:  Probes that ucrtbase.dll does not EXPORT the Annex K names (GetProcAddress == NULL). strnlen_s is the least
 *        certain: the UCRT headers provide it inline / as an alias, and the DLL may or may not export it.
 */
#include "tc.h"

TC_CASE(set_constraint_handler_s_absent) { CHECK(tc_crt_proc("set_constraint_handler_s") == NULL); }
TC_CASE(memset_s_absent)                 { CHECK(tc_crt_proc("memset_s") == NULL); }
TC_CASE(abort_handler_s_absent)          { CHECK(tc_crt_proc("abort_handler_s") == NULL); }
TC_CASE(ignore_handler_s_absent)         { CHECK(tc_crt_proc("ignore_handler_s") == NULL); }
TC_CASE(strerrorlen_s_absent)            { CHECK(tc_crt_proc("strerrorlen_s") == NULL); }
TC_CASE(strnlen_s_absent_probe)          { CHECK(tc_crt_proc("strnlen_s") == NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(set_constraint_handler_s_absent); TC_RUN(memset_s_absent); TC_RUN(abort_handler_s_absent);
    TC_RUN(ignore_handler_s_absent); TC_RUN(strerrorlen_s_absent); TC_RUN(strnlen_s_absent_probe);
    return tc_end();
}
