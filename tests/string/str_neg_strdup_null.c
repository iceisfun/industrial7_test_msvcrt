/* TC
 * id:    str_neg_strdup_null
 * area:  string
 * funcs: _strdup
 * procs:
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strdup (NULL source not documented)
 * note:  PROBE: _strdup(NULL) returns NULL without crashing. Docs do not mention NULL input; the release
 *        UCRT is believed to return NULL early. errno/invalid-parameter-handler behaviour is deliberately
 *        NOT asserted (release CRT likely does neither; debug CRT validates). msvcrt.dll excluded.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
static char *(*volatile p_dup)(const char *) = _strdup;
TC_CASE(null_returns_null)
{
    char *r; tc_iph_count = 0; errno = 0;
    r = p_dup(NULL);
    CHECK(r == NULL);
    free(r);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_returns_null); return tc_end(); }
