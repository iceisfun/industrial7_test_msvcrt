/* TC
 * id:    gct_strcmpi_basic
 * area:  ctype
 * funcs: 
 * procs: _stricmp _strcmpi strcmpi _stricoll stricoll
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _stricmp, _stricoll
 * note:  _stricmp/_strcmpi/strcmpi fold to lower case, so '_' (0x5F) < 'a' and [ (0x5B) < a too (both below the lowercase letters). Aliases (_strcmpi, strcmpi, stricoll) are compared only when exported; their export is checked in gct_undecorated_aliases_exported_probe.
 */
#include "tc.h"
typedef int (__cdecl *cmp_t)(const char *, const char *);
static cmp_t fn[5];
static const char *const nm[5] = { "_stricmp", "_strcmpi", "strcmpi", "_stricoll", "stricoll" };
static volatile const char *pa, *pb;
static int sgn(int x) { return x < 0 ? -1 : x > 0; }
#define A (const char *)pa
#define B (const char *)pb
static void all4(const char *a, const char *b, int want, int line)
{
    int i;
    pa = a; pb = b;
    for (i = 0; i < 5; i++) {
        int r;
        if (!fn[i]) continue;
        r = sgn(fn[i]((const char *)pa, (const char *)pb));
        if (r != want) { tc_fail_ints(__FILE__, line, nm[i], a, r, want); }
    }
}
#define T(a, b, w) all4(a, b, w, __LINE__)
TC_CASE(decorated_present) { CHECK(fn[0] != NULL); CHECK(fn[3] != NULL); }
TC_CASE(equal) { T("Hello", "hELLO", 0); T("abc", "abc", 0); T("ABC", "abc", 0); }
TC_CASE(order) { T("abc", "ABD", -1); T("b", "A", 1); T("ABD", "abc", 1); }
TC_CASE(prefix_empty) { T("abc", "ABCD", -1); T("ABCD", "abc", 1); T("", "a", -1); T("a", "", 1); }
TC_CASE(both_empty) { T("", "", 0); }
TC_CASE(underscore) { T("_", "A", -1); T("[", "a", -1); T("a", "_", 1); }
int main(void)
{
    tc_begin(); { int i; for (i = 0; i < 5; i++) fn[i] = (cmp_t)tc_crt_proc(nm[i]); }
    TC_RUN(decorated_present); TC_RUN(equal); TC_RUN(order); TC_RUN(prefix_empty); TC_RUN(both_empty); TC_RUN(underscore);
    return tc_end();
}
