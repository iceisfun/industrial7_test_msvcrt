/* TC
 * id:    gct_wcscmpi_wcsicoll
 * area:  ctype
 * funcs: 
 * procs: _wcsicoll wcsicoll wcscmpi _wcsicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _wcsicmp, _wcsicoll
 * note:  wcscmpi is the undecorated alias of _wcsicmp (mingw lists it for all CRTs); verify exported on Windows.
 */
#include "tc.h"
typedef int (__cdecl *cmp_t)(const wchar_t *, const wchar_t *);
static cmp_t fn[4];
static const char *const nm[4] = { "_wcsicmp", "wcscmpi", "wcsicoll", "_wcsicoll" };
static volatile const wchar_t *pa, *pb;
static int sgn(int x) { return x < 0 ? -1 : x > 0; }
static void all3(const wchar_t *a, const wchar_t *b, int want, int line)
{
    int i;
    pa = a; pb = b;
    for (i = 0; i < 4; i++) {
        int r;
        if (!fn[i]) continue;
        r = sgn(fn[i]((const wchar_t *)pa, (const wchar_t *)pb));
        if (r != want) { tc_fail_ints(__FILE__, line, nm[i], "want", r, want); }
    }
}
#define T(a, b, w) all3(a, b, w, __LINE__)
TC_CASE(decorated_present) { CHECK(fn[0] != NULL); CHECK(fn[3] != NULL); }
TC_CASE(equal) { T(L"Hello", L"hELLO", 0); T(L"", L"", 0); }
TC_CASE(order) { T(L"abc", L"ABD", -1); T(L"b", L"A", 1); }
TC_CASE(prefix_empty) { T(L"abc", L"ABCD", -1); T(L"ABCD", L"abc", 1); T(L"", L"a", -1); T(L"a", L"", 1); }
TC_CASE(six_pairs_agree) { T(L"x", L"X", 0); T(L"a", L"b", -1); T(L"B", L"a", 1); T(L"abc", L"abd", -1); T(L"zz", L"ZZ", 0); T(L"Zeta", L"alpha", 1); }
TC_CASE(wide_identical) { T(L"\x0100", L"\x0100", 0); T(L"a\x0100", L"A\x0100", 0); }
int main(void) { tc_begin(); { int i; for (i = 0; i < 4; i++) fn[i] = (cmp_t)tc_crt_proc(nm[i]); }
    TC_RUN(decorated_present); TC_RUN(equal); TC_RUN(order); TC_RUN(prefix_empty); TC_RUN(six_pairs_agree); TC_RUN(wide_identical); return tc_end(); }
