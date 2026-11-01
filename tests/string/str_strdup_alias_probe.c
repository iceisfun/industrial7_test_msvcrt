/* TC
 * id:    str_strdup_alias_probe
 * area:  string
 * funcs: free
 * procs: strdup _strdup
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs strdup (deprecated POSIX name for _strdup)
 * note:  PROBE: the DLL exports the plain name "strdup" (believed true for msvcrt/msvcr* and ucrtbase;
 *        ucrtbase may export it for old binaries) and it behaves like _strdup.
 *        Win10 22H2 19045 (observed, GetProcAddress on ucrtbase): the plain name strdup is NOT exported; the export is _strdup (class D).
 *        On ucrtbase (TC_AT_LEAST(140)) the test asserts that "strdup" is NOT exported and that "_strdup" duplicates a string; [unverified] on older CRTs, where the
 *        original probe of the plain name "strdup" is kept.
 */
#include "tc.h"
#include <stdlib.h>

typedef char *(__cdecl *strdup_t)(const char *);

TC_CASE(exported_alias)
{
#if TC_AT_LEAST(140)
    strdup_t p = (strdup_t)tc_crt_proc("_strdup"); char *d;
    CHECK(tc_crt_proc("strdup") == NULL);   /* observed: the plain POSIX name is not exported by ucrtbase 19045 */
#else
    strdup_t p = (strdup_t)tc_crt_proc("strdup"); char *d;
#endif
    CHECK(p != NULL);
    if (!p) return;
    d = p("alias");
    CHECK(d != NULL);
    if (d) { CHECK_STR(d, "alias"); free(d); }
}

int main(void) { tc_begin(); TC_RUN(exported_alias); return tc_end(); }
