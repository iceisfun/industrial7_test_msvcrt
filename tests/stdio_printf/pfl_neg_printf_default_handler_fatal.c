/* TC
 * id:    pfl_neg_printf_default_handler_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_l _sprintf_s_l _create_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation": without an installed handler an invalid parameter terminates the process
 * note:  Each fatal call runs in a child process (tc_spawn_self). The parent asserts the child did not run to its
 *        "return 77" and crashed with an NTSTATUS error code (top two bits set). The exact status (0xC0000417 / 0xC0000409) is deliberately not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static int child_main(const char *name)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[4];
    const char *volatile nullfmt = NULL; const char *volatile big = "abcdefgh";
    if (!l) return 78;
    if (tc_strequ(name, "nullfmt")) { _sprintf_l(b, (const char *)nullfmt, l); return 77; }
    if (tc_strequ(name, "overflow")) { _sprintf_s_l(b, sizeof b, "%s", l, (const char *)big); return 77; }
    if (tc_strequ(name, "control")) { _sprintf_l(b, "ok", l); return 55; }
    return 79;
}

TC_CASE(child_control_runs_to_end)
{
    DWORD code = 0; int ran = tc_spawn_self("control", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 55);
}
TC_CASE(child_null_fmt_dies)
{
    DWORD code = 0; int ran = tc_spawn_self("nullfmt", &code, 20000);
    CHECK(ran); CHECK((code & 0xC0000000u) == 0xC0000000u);  /* NTSTATUS error severity, e.g. 0xC0000409 / 0xC0000417 */
}
TC_CASE(child_sprintf_s_l_overflow_dies)
{
    DWORD code = 0; int ran = tc_spawn_self("overflow", &code, 20000);
    CHECK(ran); CHECK((code & 0xC0000000u) == 0xC0000000u);
}
TC_CASE(parent_survives)
{
    DWORD code = 0; tc_spawn_self("nullfmt", &code, 20000);
    CHECK(1);  /* reaching here: the parent was unaffected by the child's termination */
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_control_runs_to_end); TC_RUN(child_null_fmt_dies); TC_RUN(child_sprintf_s_l_overflow_dies); TC_RUN(parent_survives);
    return tc_end();
}
