/* TC
 * id:    pfm_1_neg_stdio_common_vsprintf_absent
 * area:  stdio_printf
 * funcs:
 * procs: sprintf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs __stdio_common_vsprintf (UCRT / Windows 10 SDK only; inline printf wrappers over it live in the UCRT headers)
 * note:  The __stdio_common_* entry points exist only in ucrtbase.dll; no msvcr* / msvcrt.dll exports them.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(vsprintf_absent)  { CHECK(tc_crt_proc("__stdio_common_vsprintf") == NULL); }
TC_CASE(vsnprintf_s_absent) { CHECK(tc_crt_proc("__stdio_common_vsnprintf_s") == NULL); }
TC_CASE(vswprintf_absent) { CHECK(tc_crt_proc("__stdio_common_vswprintf") == NULL); }
TC_CASE(vfprintf_absent)  { CHECK(tc_crt_proc("__stdio_common_vfprintf") == NULL); }
TC_CASE(sprintf_present)  { CHECK(tc_crt_proc("sprintf") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(vsprintf_absent); TC_RUN(vsnprintf_s_absent); TC_RUN(vswprintf_absent); TC_RUN(vfprintf_absent); TC_RUN(sprintf_present);
    return tc_end();
}
