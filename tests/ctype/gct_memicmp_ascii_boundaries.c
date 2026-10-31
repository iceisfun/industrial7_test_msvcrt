/* TC
 * id:    gct_memicmp_ascii_boundaries
 * area:  ctype
 * funcs: 
 * procs: _memicmp memicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _memicmp: compares first count bytes, case-insensitively (lowercase fold)
 * note:  Stresses count/NUL semantics and the undecorated alias.
 */
#include "tc.h"
typedef int (__cdecl *mem_t)(const void *, const void *, size_t);
static mem_t f1, f2;
static volatile size_t vn;
static volatile const char *pa, *pb;
static int sgn(int x) { return x < 0 ? -1 : x > 0; }
static void both(const void *a, const void *b, size_t n, int want, int line)
{
    int r1, r2; pa = a; pb = b; vn = n;
    r1 = sgn(_memicmp((const void *)pa, (const void *)pb, vn));
    r2 = f2 ? sgn(f2((const void *)pa, (const void *)pb, vn)) : want;
    if (r1 != want) { tc_fail_ints(__FILE__, line, "sign _memicmp", "want", r1, want); }
    if (r2 != want) { tc_fail_ints(__FILE__, line, "sign memicmp", "want", r2, want); }
}
#define T(a, b, n, w) both(a, b, n, w, __LINE__)
TC_CASE(present) { CHECK(f1 != NULL); }
TC_CASE(equal_ignoring_case) { T("Hello", "hELLO", 5, 0); T("ABC", "abc", 3, 0); }
TC_CASE(beyond_count_ignored) { T("abcX", "ABCY", 3, 0); T("abcX", "ABCY", 4, -1); T("abcY", "ABCX", 4, 1); }
TC_CASE(zero_count) { T("abc", "xyz", 0, 0); }
TC_CASE(embedded_nul)
{
    static const char a[] = "a\0b", b[] = "A\0c", c[] = "A\0B";
    T(a, b, 3, -1); T(b, a, 3, 1); T(a, c, 3, 0);
}
TC_CASE(lowercase_fold) { T("_", "a", 1, -1); T("_", "A", 1, -1); T("[", "A", 1, -1); T("{", "Z", 1, 1); }
int main(void) { tc_begin(); f1 = (mem_t)tc_crt_proc("_memicmp"); f2 = (mem_t)tc_crt_proc("memicmp");
    TC_RUN(present); if (!f1) return tc_end(); TC_RUN(equal_ignoring_case); TC_RUN(beyond_count_ignored); TC_RUN(zero_count); TC_RUN(embedded_nul); TC_RUN(lowercase_fold); return tc_end(); }
