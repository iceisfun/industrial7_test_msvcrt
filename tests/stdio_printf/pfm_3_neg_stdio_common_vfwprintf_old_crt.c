/* TC
 * id:    pfm_3_neg_stdio_common_vfwprintf_old_crt
 * area:  stdio_printf
 * funcs:
 * procs: sprintf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs __stdio_common_vfwprintf, __stdio_common_vsprintf (Universal CRT only, ucrtbase.dll / Windows 10 SDK)
 * note:  Pre-UCRT DLLs must not export any __stdio_common_* entry point; the printf functions are real exports there.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(vfwprintf_absent)   { CHECK(tc_crt_proc("__stdio_common_vfwprintf") == NULL); }
TC_CASE(vsprintf_absent)    { CHECK(tc_crt_proc("__stdio_common_vsprintf") == NULL); }
TC_CASE(vfprintf_absent)    { CHECK(tc_crt_proc("__stdio_common_vfprintf") == NULL); }
TC_CASE(vswprintf_absent)   { CHECK(tc_crt_proc("__stdio_common_vswprintf") == NULL); }
TC_CASE(vsnprintf_s_absent) { CHECK(tc_crt_proc("__stdio_common_vsnprintf_s") == NULL); }
TC_CASE(vfwprintf_p_absent) { CHECK(tc_crt_proc("__stdio_common_vfwprintf_p") == NULL); }
TC_CASE(control_exports)    { CHECK(tc_crt_proc("sprintf") != NULL); CHECK(tc_crt_proc("_vsnwprintf") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(vfwprintf_absent); TC_RUN(vsprintf_absent); TC_RUN(vfprintf_absent); TC_RUN(vswprintf_absent);
    TC_RUN(vsnprintf_s_absent); TC_RUN(vfwprintf_p_absent); TC_RUN(control_exports);
    return tc_end();
}
