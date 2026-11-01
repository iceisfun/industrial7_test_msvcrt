/* TC
 * id:    pfw_neg_fatal_default_iph_ucrt
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"; see tests/errno_param/err_neg_default_handler_fatal.c
 * note:  Each child triggers a wide printf invalid-parameter path with the default handler; UCRT terminates with the
 *        fast-fail status 0xC0000409. Docs only promise termination; the exact status is PROBED.
 *        swprintf_s is header-inline in ucrt (not exported), so it is called via the header, not tc_crt_proc.
 *        HARNESS FIX (class F, Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe5/6: every printf/scanf/strcpy_s "default handler" child exited 0x4d = survived): the mingw-w64 startup code installs
 *        a no-op invalid-parameter handler (call to _set_invalid_parameter_handler(__mingw_invalidParameterHandler) in main's crt startup), so the CRT's own
 *        default (terminate) never ran. The child now calls tc_iph_default() (= _set_invalid_parameter_handler(NULL)) first. Test expectations unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

static int sws(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf_s(b, n, f, ap); va_end(ap); return r; }
static const wchar_t *volatile nfmt = NULL;
static wchar_t *volatile nbuf = NULL;
static FILE *volatile nfile = NULL;

static void run(const char *name)
{ DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000); CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL); }

TC_CASE(swprintf_s_null_buf) { run("s_nullbuf"); }
TC_CASE(snwprintf_null_fmt)  { run("n_nullfmt"); }
TC_CASE(percent_n)           { run("pct_n"); }
TC_CASE(fwprintf_null_stream){ run("fw_null"); }

int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL)
    {
        wchar_t b[16]; volatile int x = 0;
        tc_iph_default();
        if (c[0] == 's') sws(nbuf, 10, L"x");
        else if (c[0] == 'n') _snwprintf(b, 8, nfmt);
        else if (c[0] == 'p') _snwprintf(b, 8, L"%n", &x);
        else if (c[0] == 'f') fwprintf(nfile, L"x");
        return 77;
    }
    TC_RUN(swprintf_s_null_buf); TC_RUN(snwprintf_null_fmt); TC_RUN(percent_n); TC_RUN(fwprintf_null_stream);
    return tc_end();
}
