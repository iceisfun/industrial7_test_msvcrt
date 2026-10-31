/* TC
 * id:    gtm_neg_getmonths_free_twice_probe
 * area:  time
 * funcs: free
 * procs: _Getmonths _Getdays _W_Getdays
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   undocumented yvals/xtime helpers; callers free() the returned table
 * note:  PROBE of ownership semantics: each call returns a fresh heap block (no shared static buffer), so two live results must not alias,
 *        freeing the first must leave the second intact, and writing to the first must not alter the second.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

typedef char *(__cdecl *get_t)(void);
typedef wchar_t *(__cdecl *wget_t)(void);
static get_t pm, pd; static wget_t pw;

TC_CASE(getmonths_independent)
{
    char *a, *b; char copy[512]; if (!pm) TC_SKIP("missing");
    a = pm(); b = pm(); CHECK(a != NULL && b != NULL); if (!a || !b) return;
    CHECK(a != b);
    if (strlen(b) < sizeof copy) strcpy(copy, b); else copy[0] = 0;
    free(a);
    CHECK_STR(b, copy);
    free(b);
}
TC_CASE(getmonths_write_isolated)
{
    char *a, *b; char copy[512]; if (!pm) TC_SKIP("missing");
    a = pm(); b = pm(); CHECK(a != NULL && b != NULL); if (!a || !b) return;
    if (strlen(b) < sizeof copy) strcpy(copy, b); else copy[0] = 0;
    memset(a, 'Z', strlen(a));
    CHECK_STR(b, copy);
    free(a); free(b);
}
TC_CASE(getdays_independent)
{
    char *a, *b; if (!pd) TC_SKIP("missing");
    a = pd(); b = pd(); CHECK(a != NULL && b != NULL); if (!a || !b) return;
    CHECK(a != b);
    memset(a, 'Z', strlen(a));
    CHECK(b[0] != 'Z');
    free(a); free(b);
}
TC_CASE(w_getdays_independent)
{
    wchar_t *a, *b; if (!pw) TC_SKIP("missing");
    a = pw(); b = pw(); CHECK(a != NULL && b != NULL); if (!a || !b) return;
    CHECK(a != b);
    a[0] = L'Z';
    CHECK(b[0] != L'Z');
    free(a); free(b);
}
int main(void)
{
    tc_begin();
    pm = (get_t)tc_crt_proc("_Getmonths"); pd = (get_t)tc_crt_proc("_Getdays"); pw = (wget_t)tc_crt_proc("_W_Getdays");
    TC_RUN(getmonths_independent); TC_RUN(getmonths_write_isolated); TC_RUN(getdays_independent); TC_RUN(w_getdays_independent);
    return tc_end();
}
