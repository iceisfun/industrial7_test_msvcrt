/* TC
 * id:    pfq_neg_probe_sprintf_p_msvcrt_absent
 * area:  stdio_printf
 * funcs:
 * procs: _sprintf_p _swprintf_p
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p, _sprintf_p (Requirements table lists no msvcrt.dll)
 * note:  PROBE: msvcrt.dll (Windows 7 era) does not export _sprintf_p / _swprintf_p, so GetProcAddress returns NULL. Only these two are asserted: the mingw import library for msvcrt.dll lists _vsprintf_p, _fprintf_p, _printf_p (and _cprintf_p etc.), so those ARE probably exported by Vista+ msvcrt.dll and are deliberately not asserted absent. Whether _sprintf_p is really absent is the guess being validated.
 */
#include "tc.h"

static void absent(const char *name)
{
    CHECK(tc_crt_proc(name) == NULL);
}
TC_CASE(sprintf_p)  { absent("_sprintf_p"); }
TC_CASE(swprintf_p) { absent("_swprintf_p"); }
TC_CASE(control_sprintf_present) { CHECK(tc_crt_proc("sprintf") != NULL); }
int main(void)
{
    tc_begin();
    TC_RUN(sprintf_p); TC_RUN(swprintf_p); TC_RUN(control_sprintf_present);
    return tc_end();
}
