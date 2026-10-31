/* TC
 * id:    ucr_strtol_l_null_locale
 * area:  ucrt_only
 * funcs:
 * procs: _strtol_l _create_locale _free_locale
 * crt:   >=80
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _strtol_l (locale param); NULL meaning 'current locale' is long-standing CRT behaviour (_LocaleUpdate) but not stated on the strtol page
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

TC_DECL_PROC(long, _strtol_l, (const char *, char **, int, _locale_t));
TC_DECL_PROC(_locale_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (_locale_t));
static const char *volatile v;
TC_CASE(null_locale) { char *e; v = "123xyz"; CHECK_EQ_I(tc_p__strtol_l((const char *)v, &e, 10, NULL), 123); CHECK_EQ_P(e, v + 3);
  v = "-0x10"; CHECK_EQ_I(tc_p__strtol_l((const char *)v, &e, 16, NULL), -16); }
TC_CASE(c_locale) { char *e; _locale_t l = tc_p__create_locale(LC_ALL, "C"); v = "  456";
  CHECK(l != NULL); if (!l) return;
  CHECK_EQ_I(tc_p__strtol_l((const char *)v, &e, 10, l), 456); CHECK_EQ_P(e, v + 5); tc_p__free_locale(l); }

int main(void) { tc_begin(); TC_LOAD_PROC(_strtol_l); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale);
  if (!tc_p__strtol_l || !tc_p__create_locale || !tc_p__free_locale) return 3; TC_RUN(null_locale); TC_RUN(c_locale); return tc_end(); }
