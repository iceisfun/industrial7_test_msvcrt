/* TC
 * id:    str_stricmp_alias
 * area:  string
 * funcs: _stricmp _strnicmp
 * procs: stricmp strnicmp
 * crt:   win7
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _stricmp (stricmp is a deprecated POSIX-name alias)
 * note:  PROBE: whether the msvcr*.dll/msvcrt.dll export the old names stricmp/strnicmp (and alias the underscore versions). Believed (unverified) that msvcr80-120 export the old names stricmp/strnicmp; msvcrt.dll is the least certain. ucrtbase is believed not to (see str_neg_stricmp_alias_ucrt_probe). If an export is absent the `exported` case fails and the behaviour cases SKIP.
 */
#include "tc.h"
#include <string.h>

typedef int (__cdecl *f2_t)(const char *, const char *);
typedef int (__cdecl *f3_t)(const char *, const char *, size_t);
static f2_t p_stricmp; static f3_t p_strnicmp;

static const char *vs[][2] = { {"Hello","hELLO"}, {"abc","abd"}, {"b","A"}, {"","x"}, {"same","same"} };
#define NV (int)(sizeof vs / sizeof vs[0])
static int sgn(int x) { return (x > 0) - (x < 0); }

TC_CASE(exported) { CHECK(p_stricmp != NULL); CHECK(p_strnicmp != NULL); }
TC_CASE(stricmp_same)
{
    int i;
    if (!p_stricmp) TC_SKIP("stricmp not exported");
    for (i = 0; i < NV; i++) {
        volatile const char *a = vs[i][0], *b = vs[i][1];
        CHECK_EQ_I(sgn(p_stricmp((const char *)a, (const char *)b)), sgn(_stricmp((const char *)a, (const char *)b)));
    }
    CHECK_EQ_I(p_stricmp("Hello", "hELLO"), 0);
}
TC_CASE(strnicmp_same)
{
    int i;
    volatile const char *a = "ABCx", *b = "abcy";
    if (!p_strnicmp) TC_SKIP("strnicmp not exported");
    for (i = 0; i < NV; i++) {
        volatile const char *c = vs[i][0], *d = vs[i][1];
        CHECK_EQ_I(sgn(p_strnicmp((const char *)c, (const char *)d, 2)), sgn(_strnicmp((const char *)c, (const char *)d, 2)));
    }
    CHECK_EQ_I(p_strnicmp((const char *)a, (const char *)b, 3), 0);
    CHECK(p_strnicmp((const char *)a, (const char *)b, 4) < 0);
}
int main(void)
{
    tc_begin();
    p_stricmp = (f2_t)tc_crt_proc("stricmp"); p_strnicmp = (f3_t)tc_crt_proc("strnicmp");
    TC_RUN(exported); TC_RUN(stricmp_same); TC_RUN(strnicmp_same);
    return tc_end();
}
