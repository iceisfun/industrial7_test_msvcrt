/* TC
 * id:    str_strdup_alias_probe
 * area:  string
 * funcs: free
 * procs: strdup _strdup
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strdup (deprecated POSIX name for _strdup)
 * note:  PROBE: the DLL exports the plain name "strdup" (believed true for msvcrt/msvcr* and ucrtbase;
 *        ucrtbase may export it for old binaries) and it behaves like _strdup.
 */
#include "tc.h"
#include <stdlib.h>

typedef char *(__cdecl *strdup_t)(const char *);

TC_CASE(exported_alias)
{
    strdup_t p = (strdup_t)tc_crt_proc("strdup"); char *d;
    CHECK(p != NULL);
    if (!p) return;
    d = p("alias");
    CHECK(d != NULL);
    if (d) { CHECK_STR(d, "alias"); free(d); }
}

int main(void) { tc_begin(); TC_RUN(exported_alias); return tc_end(); }
