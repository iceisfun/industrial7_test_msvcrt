/* TC
 * id:    pfq_neg_sprintf_p_default_handler_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation": with the default invalid-parameter handler the process is terminated; _sprintf_p NULL buffer/format (invalid-format-character case is in the code probe)
 * note:  Each fatal call runs in a child process. Children must not return 77 (reached only if the CRT did not terminate).
 *        Exact termination status is in pfq_neg_sprintf_p_default_handler_code_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int cp(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap);
    return r;
}
static int child_main(const char *c)
{
    char buf[16]; char * volatile nb = NULL; const char * volatile nf = NULL;
    if (tc_strequ(c, "null_buf")) cp(nb, 10, "%1$d", 1);
    else if (tc_strequ(c, "null_fmt")) cp(buf, sizeof buf, nf);
    else if (tc_strequ(c, "control")) { int r = cp(buf, sizeof buf, "%1$d", 5); return (r == 1 && buf[0] == '5') ? 77 : 3; }
    return 77;
}
static void fatal(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK(code != 3);
}
TC_CASE(null_buf) { fatal("null_buf"); }
TC_CASE(null_fmt) { fatal("null_fmt"); }
TC_CASE(control_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("control", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(control_returns);
    return tc_end();
}
