/* TC
 * id:    str_neg_strdup_null_errno_probe
 * area:  string
 * funcs: _strdup
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strdup (NULL source not documented)
 * note:  PROBE: _strdup(NULL) returns NULL silently (recollection of the UCRT source: early `if (string == nullptr) return nullptr;`): errno untouched and the invalid-parameter handler not called. Alternative on real Windows: errno EINVAL and one handler call (see str_neg_strdup_null, which asserts only the NULL return).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
static const char *volatile pn = NULL;
TC_CASE(null_input)
{
    char *r; errno = 0x4242; tc_iph_count = 0;
    r = _strdup(pn);
    CHECK(r == NULL); CHECK_EQ_I(errno, 0x4242); CHECK_EQ_I(tc_iph_count, 0);
    free(r);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_input); return tc_end(); }
