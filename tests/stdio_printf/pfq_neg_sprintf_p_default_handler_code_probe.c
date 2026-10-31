/* TC
 * id:    pfq_neg_sprintf_p_default_handler_code_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"; STATUS_INVALID_CRUNTIME_PARAMETER 0xC0000417, fast-fail 0xC0000409
 * note:  PROBES (a) the exact exit status of the default invalid-parameter path: expected 0xC0000417 (Watson / old CRTs) or 0xC0000409
 *        (__fastfail, UCRT on Win8+); (b) a positional gap ("%1$d %3$d") with the default handler is treated as invalid -> same fatal status;
 *        (c) an unknown conversion character ("%1$y") is likewise invalid -> fatal status.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cp(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap);
    return r;
}
static int child_main(const char *c)
{
    char buf[32]; char * volatile nb = NULL; const char * volatile gap = "%1$d %3$d"; const char * volatile bad = "%1$y";
    if (tc_strequ(c, "exact")) cp(nb, 10, "%1$d", 1);
    else if (tc_strequ(c, "gap")) { int r = cp(buf, sizeof buf, gap, 1, 2, 3); return r < 0 ? 78 : 77; }
    else if (tc_strequ(c, "bad_spec")) cp(buf, sizeof buf, bad, 1);
    return 77;
}
TC_CASE(exact_status)
{
    DWORD code = 0; int ran = tc_spawn_self("exact", &code, 20000);
    CHECK(ran);
    CHECK(code == 0xC0000417u || code == 0xC0000409u);
}
TC_CASE(gap_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("gap", &code, 20000);
    CHECK(ran);
    /* most likely: gap is invalid -> fatal (0xC0000417/0xC0000409) */
    CHECK(code == 0xC0000417u || code == 0xC0000409u);
}
TC_CASE(bad_spec_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("bad_spec", &code, 20000);
    CHECK(ran);
    CHECK(code == 0xC0000417u || code == 0xC0000409u);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(exact_status); TC_RUN(gap_terminates); TC_RUN(bad_spec_terminates);
    return tc_end();
}
