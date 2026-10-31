/* TC
 * id:    pfx_neg_fprintf_default_handler_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation" / _set_invalid_parameter_handler: with the default handler the process terminates
 * note:  Fatal paths run in child processes (tc_spawn_self). Exact status: 0xC0000417 (STATUS_INVALID_CRUNTIME_PARAMETER) or
 *        0xC0000409 (fast fail) depending on CRT; the test accepts either, and rejects 0 and the sentinel 77.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wformat"

static FILE *volatile g_null_stream;
static const char *volatile g_null_fmt;
static const char *volatile g_n_fmt = "abc%n";
#define TMP "tc_pfx_neg_fprintf_default_handler_fatal_" TCV ".tmp"

static void check_code(DWORD code)
{
    CHECK(code != 0); CHECK(code != 77);
    CHECK(code == TC_STATUS_INVALID_CRT_PARAM || code == TC_STATUS_FASTFAIL);
}
TC_CASE(null_stream)  { DWORD c = 0; CHECK(tc_spawn_self("nullstream", &c, 20000)); check_code(c); }
TC_CASE(null_format)  { DWORD c = 0; CHECK(tc_spawn_self("nullfmt", &c, 20000)); check_code(c); DeleteFileA(TMP); }
TC_CASE(percent_n)    { DWORD c = 0; CHECK(tc_spawn_self("percentn", &c, 20000)); check_code(c); DeleteFileA(TMP); }

static int child(const char *name)
{
    int n = 0;
    if (name[0] == 'n' && name[1] == 'u' && name[4] == 's') { fprintf(g_null_stream, "x"); return 77; }
    else {
        FILE *f = fopen(TMP, "w");
        if (!f) return 78;
        if (name[0] == 'p') fprintf(f, g_n_fmt, &n);
        else fprintf(f, g_null_fmt);
        return 77;
    }
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(null_stream); TC_RUN(null_format); TC_RUN(percent_n);
    return tc_end();
}
