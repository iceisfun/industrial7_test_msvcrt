/* TC
 * id:    pfl_neg_default_handler_fatal_more
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_l _vsnprintf_l _vswprintf_l _vfprintf_l _vprintf_l _create_locale _free_locale
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", _set_invalid_parameter_handler (no handler installed: the process is terminated)
 * note:  Each child triggers one invalid-parameter path with the DEFAULT handler and must terminate abnormally (exit code neither 0 nor the
 *        survival marker 77). The child restores the CRT default with tc_iph_default() first (the mingw-w64 startup installs a no-op handler,
 *        which otherwise makes every child survive). The exact status is probed in pfl_neg_default_handler_fatal_status_probe.
 *        The control child proves the harness path (no failing call -> exit 77).
 */
#include "tc.h"
#include "pfl_shim.h"

static const char * volatile v_nof;
static FILE * volatile v_nofile;
static const wchar_t * volatile v_nowf;

static int child_main(const char *c)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[16]; wchar_t w[16]; int n = 1;
    if (!l) return 78;
    tc_iph_default();
    if (tc_strequ(c, "printf_null")) pl_printf_l(v_nof, l);
    else if (tc_strequ(c, "snprintf_pctn")) pl_snprintf_l(b, 16, "%n", l, &n);
    else if (tc_strequ(c, "swprintf_nullfmt")) pl_swprintf_l(w, v_nowf, l);
    else if (tc_strequ(c, "fprintf_nullstream")) pl_fprintf_l(v_nofile, "x", l);
    _free_locale(l);
    return 77;
}
static void run(const char *name, int expect_death)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    if (expect_death) { CHECK(code != 0); CHECK(code != 77); CHECK(code != 78); }
    else CHECK_EQ_U(code, 77);
}
TC_CASE(control) { run("control", 0); }
TC_CASE(printf_l_null_fmt) { run("printf_null", 1); }
TC_CASE(snprintf_l_pctn) { run("snprintf_pctn", 1); }
TC_CASE(swprintf_l_null_fmt) { run("swprintf_nullfmt", 1); }
TC_CASE(fprintf_l_null_stream) { run("fprintf_nullstream", 1); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(control); TC_RUN(printf_l_null_fmt); TC_RUN(snprintf_l_pctn); TC_RUN(swprintf_l_null_fmt); TC_RUN(fprintf_l_null_stream);
    return tc_end();
}
