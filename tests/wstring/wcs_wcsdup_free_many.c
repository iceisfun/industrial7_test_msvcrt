/* TC
 * id:    wcs_wcsdup_free_many
 * area:  wstring
 * funcs: _wcsdup
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _strdup, _wcsdup (allocated with malloc, release with free)
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>

#define N 1000
static wchar_t src[N + 1];
static wchar_t *live[N];

static void mk(int len, int seed)
{
    int i;
    for (i = 0; i < len; i++) src[i] = (wchar_t)(1 + ((i * 7 + seed) % 0xFFF0));
    src[len] = 0;
}
static int same(const wchar_t *a, const wchar_t *b, int len)
{
    int i; for (i = 0; i <= len; i++) if (a[i] != b[i]) return 0;
    return 1;
}
TC_CASE(each_iteration_verified)
{
    int i, bad = 0;
    for (i = 0; i < N; i++) {
        wchar_t *d; mk(i, i);
        d = _wcsdup(src);
        if (!d || d == src || !same(d, src, i)) bad++;
        free(d);
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(live_allocations_no_overlap)
{
    int i, j, bad = 0;
    for (i = 0; i < N; i++) { mk(i, i * 3); live[i] = _wcsdup(src); if (!live[i]) bad++; }
    for (i = 0; i < N; i++) {
        mk(i, i * 3);
        if (live[i] && !same(live[i], src, i)) bad++;
    }
    for (i = 0; i < N; i++) for (j = i + 1; j < N; j++) {
        if (!live[i] || !live[j]) continue;
        {
            ULONG_PTR a0 = (ULONG_PTR)live[i], a1 = a0 + (size_t)(i + 1) * sizeof(wchar_t);
            ULONG_PTR b0 = (ULONG_PTR)live[j], b1 = b0 + (size_t)(j + 1) * sizeof(wchar_t);
            if (a0 < b1 && b0 < a1) bad++;
        }
    }
    for (i = 0; i < N; i++) free(live[i]);
    CHECK_EQ_I(bad, 0);
}
TC_CASE(empty_string_dup)
{
    wchar_t *d; src[0] = 0; d = _wcsdup(src);
    CHECK(d != NULL); if (d) CHECK_EQ_I(d[0], 0);
    free(d);
}
int main(void) { tc_begin(); TC_RUN(each_iteration_verified); TC_RUN(live_allocations_no_overlap); TC_RUN(empty_string_dup); return tc_end(); }
