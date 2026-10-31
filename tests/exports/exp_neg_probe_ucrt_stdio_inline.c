/* TC
 * id:    exp_neg_probe_ucrt_stdio_inline
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   UCRT stdio.h (printf family is inline over __stdio_common_v*)
 * note:  Probing: ucrtbase.dll exports only the __stdio_common_v* workers; sprintf/printf/sscanf and the _s variants are inline in
 *        the headers and therefore NOT exported by name. (gets is deliberately not probed: the import library suggests ucrtbase still exports it.)
 *        Each group is its own case so one wrong guess does not hide the others.
 */
#include "tc.h"

TC_CASE(workers_exported) { CHECK(tc_crt_proc("__stdio_common_vsprintf") != NULL); CHECK(tc_crt_proc("__stdio_common_vfprintf") != NULL); CHECK(tc_crt_proc("__stdio_common_vsscanf") != NULL); }
TC_CASE(secure_printf_absent) { CHECK(tc_crt_proc("sprintf_s") == NULL); CHECK(tc_crt_proc("fprintf_s") == NULL); CHECK(tc_crt_proc("printf_s") == NULL); CHECK(tc_crt_proc("swprintf_s") == NULL); }
TC_CASE(secure_scanf_absent) { CHECK(tc_crt_proc("sscanf_s") == NULL); CHECK(tc_crt_proc("scanf_s") == NULL); CHECK(tc_crt_proc("fscanf_s") == NULL); }
TC_CASE(legacy_printf_absent) { CHECK(tc_crt_proc("printf") == NULL); CHECK(tc_crt_proc("sprintf") == NULL); CHECK(tc_crt_proc("fprintf") == NULL); }
TC_CASE(legacy_scanf_absent) { CHECK(tc_crt_proc("scanf") == NULL); CHECK(tc_crt_proc("sscanf") == NULL); }
int main(void)
{
    tc_begin();
    TC_RUN(workers_exported); TC_RUN(secure_printf_absent); TC_RUN(secure_scanf_absent); TC_RUN(legacy_printf_absent); TC_RUN(legacy_scanf_absent);
    return tc_end();
}
