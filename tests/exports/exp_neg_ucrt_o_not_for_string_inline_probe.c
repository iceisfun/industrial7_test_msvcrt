/* TC
 * id:    exp_neg_ucrt_o_not_for_string_inline_probe
 * area:  exports
 * funcs:
 * procs: _o_strlen _o_wcslen _o_memcpy _o_strcpy _o_strcmp
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   ucrtbase _o_ aliases (undocumented)
 * note:  kind=negative conf=probe. Probing: ucrtbase does not export _o_strlen/_o_wcslen/_o_memcpy/_o_strcpy/_o_strcmp
 *        (no _o_ alias for these simple string/memory functions; note _o_memset and _o_wcscpy DO exist, so they are not asserted).
 */
#include "tc.h"

static HMODULE crt;
static int has(const char *n) { return crt && GetProcAddress(crt, n) != NULL; }

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(has("strlen")); CHECK(has("_o_malloc")); }
TC_CASE(strlen_absent) { CHECK(!has("_o_strlen")); CHECK(!has("_o_wcslen")); }
TC_CASE(memcpy_absent) { CHECK(!has("_o_memcpy")); }
TC_CASE(strcpy_strcmp_absent) { CHECK(!has("_o_strcpy")); CHECK(!has("_o_strcmp")); }
int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present); TC_RUN(strlen_absent); TC_RUN(memcpy_absent); TC_RUN(strcpy_strcmp_absent);
    return tc_end();
}
