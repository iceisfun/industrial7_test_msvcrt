/* TC
 * id:    gcv_neg_atodbl_l_null_probe
 * area:  conv
 * funcs: _atodbl_l _atoflt_l _create_locale _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _atodbl (no documented NULL behaviour)
 * note:  PROBING: NULL string / NULL out-pointer for _atodbl_l/_atoflt_l. Each runs in a child with the handler installed
 *        so a crash is survivable. Parent asserts only that the child terminates (no hang) and prints the exit code.
 *        Child exit 10 = returned and handler called, 11 = returned without handler, otherwise child crashed.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

static int child_main(const char *n)
{
    _CRT_DOUBLE d; _CRT_FLOAT f; char one[2] = "1";
    _locale_t loc = _create_locale(LC_ALL, "C");
    if (!tc_iph_install()) return 12;
    tc_iph_count = 0;
    if (tc_strequ(n, "dbl_nullstr")) _atodbl_l(&d, NULL, loc);
    else if (tc_strequ(n, "flt_nullstr")) _atoflt_l(&f, NULL, loc);
    else if (tc_strequ(n, "dbl_nullout")) _atodbl_l(NULL, one, loc);
    else return 13;
    return tc_iph_count ? 10 : 11;
}
static void run(const char *n)
{
    DWORD code = 0; char b[24]; int ok = tc_spawn_self(n, &code, 20000);
    tc_write("TC NOTE " TC_TEST_ID "::"); tc_write(n); tc_write(" exit="); tc_hex(code, b); tc_write(b); tc_write("\n");
    CHECK(ok);
}
TC_CASE(dbl_nullstr) { run("dbl_nullstr"); }
TC_CASE(flt_nullstr) { run("flt_nullstr"); }
TC_CASE(dbl_nullout) { run("dbl_nullout"); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(dbl_nullstr); TC_RUN(flt_nullstr); TC_RUN(dbl_nullout);
    return tc_end();
}
