/* TC
 * id:    str_msvcrt_secure_string_exports_probe
 * area:  string
 * funcs:
 * procs: strcpy_s strnlen _strlwr_s strtok_s
 * crt:   msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs strcpy_s, strnlen, _strlwr_s (secure CRT, VS2005+)
 * note:  PROBE: which secure/bounded string functions the system msvcrt.dll (Windows 7) exports. Asserted present (most likely):
 *        strcpy_s, strnlen. Asserted present (less sure): _strlwr_s. strtok_s is only REPORTED via TC SKIP markers (presence unknown).
 */
#include "tc.h"

TC_CASE(strcpy_s_present) { CHECK(tc_crt_proc("strcpy_s") != NULL); }
TC_CASE(strnlen_present) { CHECK(tc_crt_proc("strnlen") != NULL); }
TC_CASE(strlwr_s_present) { CHECK(tc_crt_proc("_strlwr_s") != NULL); }
TC_CASE(report_strtok_s)
{
    if (tc_crt_proc("strtok_s")) TC_SKIP("INFO strtok_s present in msvcrt.dll");
    TC_SKIP("INFO strtok_s absent from msvcrt.dll");
}

int main(void) { tc_begin(); TC_RUN(strcpy_s_present); TC_RUN(strnlen_present); TC_RUN(strlwr_s_present); TC_RUN(report_strtok_s); return tc_end(); }
