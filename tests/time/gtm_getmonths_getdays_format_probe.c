/* TC
 * id:    gtm_getmonths_getdays_format_probe
 * area:  time
 * funcs: free setlocale
 * procs: _Getdays _Getmonths
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   undocumented yvals/xtime helpers (_Getdays/_Getmonths); callers free() the result
 * note:  PROBE: exact colon-delimited format of the C-locale tables, ":Sun:Sunday:Mon:...:Sat:Saturday" and ":Jan:January:...:Dec:December",
 *        that the result is malloc'd (free() via the same CRT is safe), repeatable, and unchanged by setlocale(LC_TIME,"C").
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef char *(__cdecl *get_t)(void);
static get_t pd, pm;
static const char DAYS[] = ":Sun:Sunday:Mon:Monday:Tue:Tuesday:Wed:Wednesday:Thu:Thursday:Fri:Friday:Sat:Saturday";
static const char MONS[] = ":Jan:January:Feb:February:Mar:March:Apr:April:May:May:Jun:June:Jul:July:Aug:August:Sep:September:Oct:October:Nov:November:Dec:December";

TC_CASE(exports) { CHECK(pd != NULL); CHECK(pm != NULL); }
TC_CASE(days_c_locale)
{
    char *s; if (!pd) TC_SKIP("no _Getdays");
    s = pd(); CHECK(s != NULL); if (!s) return;
    CHECK_STR(s, DAYS); free(s);
}
TC_CASE(months_c_locale)
{
    char *s; if (!pm) TC_SKIP("no _Getmonths");
    s = pm(); CHECK(s != NULL); if (!s) return;
    CHECK_STR(s, MONS); free(s);
}
TC_CASE(repeatable_after_free)
{
    char *a, *b, *c, *d; if (!pd || !pm) TC_SKIP("missing");
    a = pd(); if (a) { char *copy = _strdup(a); free(a); b = pd(); CHECK(b != NULL && copy != NULL); if (b && copy) CHECK_STR(b, copy); free(b); free(copy); }
    c = pm(); if (c) { char *copy = _strdup(c); free(c); d = pm(); CHECK(d != NULL && copy != NULL); if (d && copy) CHECK_STR(d, copy); free(d); free(copy); }
    CHECK(a != NULL && c != NULL);
}
TC_CASE(unchanged_after_setlocale_c)
{
    char *a, *b; if (!pd) TC_SKIP("no _Getdays");
    CHECK(setlocale(LC_TIME, "C") != NULL);
    a = pd(); CHECK(a != NULL); if (a) { CHECK_STR(a, DAYS); free(a); }
    b = pm ? pm() : NULL; CHECK(b != NULL); if (b) { CHECK_STR(b, MONS); free(b); }
}
int main(void)
{
    tc_begin();
    pd = (get_t)tc_crt_proc("_Getdays"); pm = (get_t)tc_crt_proc("_Getmonths");
    TC_RUN(exports); TC_RUN(days_c_locale); TC_RUN(months_c_locale); TC_RUN(repeatable_after_free); TC_RUN(unchanged_after_setlocale_c);
    return tc_end();
}
