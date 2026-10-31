/* TC
 * id:    exp_stdlib_misc_exported
 * area:  exports
 * funcs: 
 * procs: qsort bsearch rand srand _rotl _rotr _lrotl _lrotr _makepath _splitpath _fullpath _wfullpath _wmakepath _wsplitpath
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.5 (qsort, bsearch), 7.22.2 (rand); MS docs _rotl, _lrotl, _makepath, _splitpath, _fullpath
 * note:  Calls go through GetProcAddress so the DLL under test is the one executing.
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *cmp_t)(const void *, const void *);
typedef void (__cdecl *qsort_t)(void *, size_t, size_t, cmp_t);
typedef void *(__cdecl *bsearch_t)(const void *, const void *, size_t, size_t, cmp_t);
typedef int (__cdecl *rand_t)(void);
typedef void (__cdecl *srand_t)(unsigned int);
typedef unsigned int (__cdecl *rot_t)(unsigned int, int);
typedef unsigned long (__cdecl *lrot_t)(unsigned long, int);
typedef void (__cdecl *split_t)(const char *, char *, char *, char *, char *);
typedef void (__cdecl *make_t)(char *, const char *, const char *, const char *, const char *);
typedef char *(__cdecl *full_t)(char *, const char *, size_t);

static int __cdecl cmp_int(const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; return x < y ? -1 : x > y; }

TC_CASE(sort_search)
{
    qsort_t q = (qsort_t)tc_crt_proc("qsort"); bsearch_t b = (bsearch_t)tc_crt_proc("bsearch");
    int a[6] = { 5, 3, 9, 1, 7, 3 }; int key = 7, miss = 4; int *p;
    CHECK(q && b);
    if (!(q && b)) return;
    q(a, 6, sizeof a[0], cmp_int);
    CHECK_EQ_I(a[0], 1); CHECK_EQ_I(a[1], 3); CHECK_EQ_I(a[2], 3); CHECK_EQ_I(a[3], 5); CHECK_EQ_I(a[4], 7); CHECK_EQ_I(a[5], 9);
    p = (int *)b(&key, a, 6, sizeof a[0], cmp_int);
    CHECK(p == &a[4]);
    CHECK(b(&miss, a, 6, sizeof a[0], cmp_int) == NULL);
}
TC_CASE(rand)
{
    rand_t r = (rand_t)tc_crt_proc("rand"); srand_t s = (srand_t)tc_crt_proc("srand");
    int first[8], i;
    CHECK(r && s);
    if (!(r && s)) return;
    s(12345);
    for (i = 0; i < 8; i++) { first[i] = r(); CHECK(first[i] >= 0 && first[i] <= 32767); }
    s(12345);
    for (i = 0; i < 8; i++) CHECK_EQ_I(r(), first[i]);   /* same seed, same sequence */
}
TC_CASE(rotates)
{
    rot_t rl = (rot_t)tc_crt_proc("_rotl"), rr = (rot_t)tc_crt_proc("_rotr");
    lrot_t lrl = (lrot_t)tc_crt_proc("_lrotl"), lrr = (lrot_t)tc_crt_proc("_lrotr");
    CHECK(rl && rr && lrl && lrr);
    if (!(rl && rr && lrl && lrr)) return;
    CHECK_EQ_U(rl(0x80000001u, 1), 3u);
    CHECK_EQ_U(rr(1u, 1), 0x80000000u);
    CHECK_EQ_U(rl(0x12345678u, 8), 0x34567812u);
    CHECK_EQ_U(rr(0x12345678u, 8), 0x78123456u);
    CHECK_EQ_U(lrl(0x80000001ul, 1), 3ul);
    CHECK_EQ_U(lrr(1ul, 1), 0x80000000ul);
}
TC_CASE(path_functions)
{
    split_t sp = (split_t)tc_crt_proc("_splitpath"); make_t mk = (make_t)tc_crt_proc("_makepath"); full_t fp = (full_t)tc_crt_proc("_fullpath");
    char drv[_MAX_DRIVE], dir[_MAX_DIR], fn[_MAX_FNAME], ext[_MAX_EXT], out[_MAX_PATH];
    CHECK(sp && mk && fp);
    CHECK(tc_crt_proc("_wfullpath") && tc_crt_proc("_wmakepath") && tc_crt_proc("_wsplitpath"));
    if (!(sp && mk && fp)) return;
    sp("C:\\dir\\sub\\file.ext", drv, dir, fn, ext);
    CHECK_STR(drv, "C:"); CHECK_STR(dir, "\\dir\\sub\\"); CHECK_STR(fn, "file"); CHECK_STR(ext, ".ext");
    mk(out, "C:", "\\dir\\", "name", ".txt");
    CHECK_STR(out, "C:\\dir\\name.txt");
    CHECK(fp(out, "C:\\a\\..\\b\\c.txt", _MAX_PATH) == out);
    CHECK_STR(out, "C:\\b\\c.txt");
}
int main(void)
{
    tc_begin();
    TC_RUN(sort_search); TC_RUN(rand); TC_RUN(rotates); TC_RUN(path_functions);
    return tc_end();
}
