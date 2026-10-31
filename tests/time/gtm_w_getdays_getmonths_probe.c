/* TC
 * id:    gtm_w_getdays_getmonths_probe
 * area:  time
 * funcs: free
 * procs: _W_Getdays _W_Getmonths _Getdays _Getmonths
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   undocumented yvals/xtime helpers
 * note:  PROBE: wide locale tables in the C locale equal L":Sun:Sunday:...:Saturday" / L":Jan:January:...:December" and equal the
 *        widened narrow tables. msvcr100/90 left out: mingw import library does not list the wide forms.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef wchar_t *(__cdecl *wget_t)(void);
typedef char *(__cdecl *get_t)(void);
static wget_t wd, wm; static get_t pd, pm;

static int widen_eq(const wchar_t *w, const char *n)
{
    while (*w && *n) { if (*w != (wchar_t)(unsigned char)*n) return 0; w++; n++; }
    return *w == 0 && *n == 0;
}
TC_CASE(wide_days)
{
    wchar_t *s; if (!wd) TC_SKIP("no _W_Getdays");
    s = wd(); CHECK(s != NULL); if (!s) return;
    CHECK_WSTR(s, L":Sun:Sunday:Mon:Monday:Tue:Tuesday:Wed:Wednesday:Thu:Thursday:Fri:Friday:Sat:Saturday"); free(s);
}
TC_CASE(wide_months)
{
    wchar_t *s; if (!wm) TC_SKIP("no _W_Getmonths");
    s = wm(); CHECK(s != NULL); if (!s) return;
    CHECK_WSTR(s, L":Jan:January:Feb:February:Mar:March:Apr:April:May:May:Jun:June:Jul:July:Aug:August:Sep:September:Oct:October:Nov:November:Dec:December"); free(s);
}
TC_CASE(matches_narrow)
{
    wchar_t *w; char *n;
    if (!wd || !wm || !pd || !pm) TC_SKIP("missing");
    w = wd(); n = pd(); CHECK(w && n); if (w && n) CHECK(widen_eq(w, n)); free(w); free(n);
    w = wm(); n = pm(); CHECK(w && n); if (w && n) CHECK(widen_eq(w, n)); free(w); free(n);
}
int main(void)
{
    tc_begin();
    wd = (wget_t)tc_crt_proc("_W_Getdays"); wm = (wget_t)tc_crt_proc("_W_Getmonths");
    pd = (get_t)tc_crt_proc("_Getdays"); pm = (get_t)tc_crt_proc("_Getmonths");
    TC_RUN(wide_days); TC_RUN(wide_months); TC_RUN(matches_narrow);
    return tc_end();
}
