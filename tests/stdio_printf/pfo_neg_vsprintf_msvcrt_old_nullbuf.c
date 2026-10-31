/* TC
 * id:    pfo_neg_vsprintf_msvcrt_old_nullbuf
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vscprintf _vsnprintf
 * crt:   msvcr80 msvcr90 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _vscprintf, _vsnprintf (NULL format / NULL buffer are invalid parameters on VS2005+)
 * note:  PROBE: bad pointers handed to the legacy CRTs. Each bad call runs in a child process (msvcrt.dll has no
 *        invalid-parameter handler API and might fault). Child installs tc_iph where possible, makes the call, and exits 10 if
 *        the result is negative, 11 otherwise. Asserted: the child survives with 10 for NULL format (_vscprintf, _vsnprintf)
 *        and for _vsnprintf(NULL, 0, fmt); msvcrt.dll validating NULL format at all is the thing being probed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int c_vscprintf(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static int c_vsnprintf(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

static int child_main(const char *name)
{
    volatile char *nullp = NULL; char buf[16]; int r = 0;
    tc_iph_install();
    if (tc_strequ(name, "scprintf_nullfmt")) r = c_vscprintf((const char *)nullp);
    else if (tc_strequ(name, "snprintf_nullfmt")) r = c_vsnprintf(buf, sizeof buf, (const char *)nullp);
    else if (tc_strequ(name, "snprintf_nullbuf0")) r = c_vsnprintf((char *)nullp, 0, "%d", 1);
    else return 99;
    return r < 0 ? 10 : 11;
}
static void run(const char *name)
{
    DWORD code = 0; int ok = tc_spawn_self(name, &code, 20000);
    CHECK(ok);
    CHECK_EQ_U(code, 10);
}
TC_CASE(scprintf_nullfmt)   { run("scprintf_nullfmt"); }
TC_CASE(snprintf_nullfmt)   { run("snprintf_nullfmt"); }
TC_CASE(snprintf_nullbuf0)  { run("snprintf_nullbuf0"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(scprintf_nullfmt); TC_RUN(snprintf_nullfmt); TC_RUN(snprintf_nullbuf0);
    return tc_end();
}
