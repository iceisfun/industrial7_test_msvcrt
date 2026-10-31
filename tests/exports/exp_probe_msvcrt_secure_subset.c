/* TC
 * id:    exp_probe_msvcrt_secure_subset
 * area:  exports
 * funcs:
 * procs: strcpy_s strcat_s wcscpy_s memcpy_s memmove_s sprintf_s _itoa_s _get_errno
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs Security-enhanced versions of CRT functions (msvcrt.dll listed as a requirement for some)
 * note:  Probing: which secure-CRT string/memory/conversion names the OS msvcrt.dll (Win7 and later) exports. Evidence is mixed (an import
 *        library suggests strcpy_s/_itoa_s are present; memcpy_s/sprintf_s/_get_errno are the less certain ones). One case per name;
 *        every failure is a finding, not a bug. See also exp_probe_msvcrt_secure_io_env.
 */
#include "tc.h"

TC_CASE(n_strcpy_s) { CHECK(tc_crt_proc("strcpy_s") != NULL); }
TC_CASE(n_strcat_s) { CHECK(tc_crt_proc("strcat_s") != NULL); }
TC_CASE(n_wcscpy_s) { CHECK(tc_crt_proc("wcscpy_s") != NULL); }
TC_CASE(n_memcpy_s) { CHECK(tc_crt_proc("memcpy_s") != NULL); }
TC_CASE(n_memmove_s) { CHECK(tc_crt_proc("memmove_s") != NULL); }
TC_CASE(n_sprintf_s) { CHECK(tc_crt_proc("sprintf_s") != NULL); }
TC_CASE(n_itoa_s) { CHECK(tc_crt_proc("_itoa_s") != NULL); }
TC_CASE(n_get_errno) { CHECK(tc_crt_proc("_get_errno") != NULL); }

int main(void) { tc_begin(); TC_RUN(n_strcpy_s); TC_RUN(n_strcat_s); TC_RUN(n_wcscpy_s); TC_RUN(n_memcpy_s); TC_RUN(n_memmove_s); TC_RUN(n_sprintf_s); TC_RUN(n_itoa_s); TC_RUN(n_get_errno); return tc_end(); }
