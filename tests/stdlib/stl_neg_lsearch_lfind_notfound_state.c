/* TC
 * id:    stl_neg_lsearch_lfind_notfound_state
 * area:  stdlib
 * kind:  negative
 * funcs: _lfind _lsearch
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _lfind, _lsearch: _lfind returns NULL if key not found; _lsearch appends key and increments *num
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <errno.h>

static int __cdecl cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }
static const int orig[4] = { 7, 8, 9, 0 };

TC_CASE(lfind_miss)
{
    int a[4]; unsigned int n = 3; int key = 42; void *r;
    memcpy(a, orig, sizeof a); errno = 0; tc_iph_count = 0;
    r = _lfind(&key, a, &n, sizeof(int), cmp);
    CHECK(r == NULL); CHECK_EQ_I(n, 3); CHECK_MEM(a, orig, sizeof a); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(lsearch_miss_appends)
{
    int a[4]; unsigned int n = 3; int key = 42; void *r;
    memcpy(a, orig, sizeof a); errno = 0; tc_iph_count = 0;
    r = _lsearch(&key, a, &n, sizeof(int), cmp);
    CHECK(r == &a[3]); CHECK_EQ_I(n, 4); CHECK_EQ_I(a[3], 42); CHECK_MEM(a, orig, 3 * sizeof(int));
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(lsearch_hit)
{
    int a[4]; unsigned int n = 3; int key = 8; void *r;
    memcpy(a, orig, sizeof a); errno = 0; tc_iph_count = 0;
    r = _lsearch(&key, a, &n, sizeof(int), cmp);
    CHECK(r == &a[1]); CHECK_EQ_I(n, 3); CHECK_MEM(a, orig, sizeof a); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(empty_array)
{
    int a[4]; unsigned int n = 0; int key = 5; void *r;
    memcpy(a, orig, sizeof a); errno = 0; tc_iph_count = 0;
    r = _lfind(&key, a, &n, sizeof(int), cmp);
    CHECK(r == NULL); CHECK_EQ_I(n, 0);
    r = _lsearch(&key, a, &n, sizeof(int), cmp);
    CHECK(r == &a[0]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[0], 5); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(lfind_miss); TC_RUN(lsearch_miss_appends); TC_RUN(lsearch_hit); TC_RUN(empty_array); return tc_end(); }
