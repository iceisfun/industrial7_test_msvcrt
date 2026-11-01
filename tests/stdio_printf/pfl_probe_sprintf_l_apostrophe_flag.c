/* TC
 * id:    pfl_probe_sprintf_l_apostrophe_flag
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p2
 * conf:  observed
 * ref:   MS docs printf format specification (flags: - + 0 space #; no grouping flag)
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "apostrophe_d_c_locale n == -1 got=2"; zzp_probe2 "%'d C: r=2 errno=0 iph=0 b=['d]" and same for German_Germany.1252):
 *        the POSIX apostrophe is not a Microsoft flag; "%'d" is not a recognised specification, % is dropped and "'d" printed literally (2 chars)
 *        in both the C and the German locale; no handler, errno unchanged, the argument is not consumed. Old belief (-1/EINVAL/handler) wrong.
 *        C11 7.21.6.1: the ' flag is not defined by ISO C (UB). Stable; locale-independent for both locales tried.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static volatile int v_i = 1234567;
static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";

static void run(const char *name)
{
    char b[64]; int n; _locale_t l = _create_locale(LC_ALL, name);
    if (!l) { TC_SKIP("locale missing"); }
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    b[0] = 0;
    n = _sprintf_l(b, "%'d", l, v_i);
    CHECK_EQ_I(n, 2); CHECK_STR(b, "'d"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
    _free_locale(l);
}
TC_CASE(apostrophe_d_c_locale_literal) { run(v_c); }
TC_CASE(apostrophe_d_de_locale_literal) { run(v_de); }

int main(void) { tc_begin(); TC_RUN(apostrophe_d_c_locale_literal); TC_RUN(apostrophe_d_de_locale_literal); return tc_end(); }
