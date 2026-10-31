/* TC
 * id:    gst_strtof_l_neg_hash_inf_legacy_probe
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _strtof_l
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MSVC printf "1.#INF" output form
 * note:  Probes whether legacy _strtof_l round-trips the printf spellings "1.#INF" / "1.#QNAN". Asserted guess: it does NOT; it parses the leading '1' only (value 1.0f, endptr past the number prefix, i.e. > nptr). If real Windows returns inf, this probe flags it.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

TC_DECL_PROC(float, _strtof_l, (const char *, char **, _locale_t));
static _locale_t loc;
static const char *volatile v;

TC_CASE(hash_inf) { char *e = NULL; v = "1.#INF"; CHECK_BITS_F(tc_p__strtof_l(v, &e, loc), 1.0f); CHECK(e > v && e <= v + 2); }
TC_CASE(hash_qnan) { char *e = NULL; float f; v = "1.#QNAN"; f = tc_p__strtof_l(v, &e, loc); CHECK_BITS_F(f, 1.0f); CHECK(e > v && e <= v + 2); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_strtof_l);
    if (!tc_p__strtof_l) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " _strtof_l not exported by this CRT\n"); return tc_end(); }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(hash_inf); TC_RUN(hash_qnan);
    _free_locale(loc);
    return tc_end();
}
