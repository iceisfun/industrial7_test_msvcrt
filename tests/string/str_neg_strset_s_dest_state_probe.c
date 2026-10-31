/* TC
 * id:    str_neg_strset_s_dest_state_probe
 * area:  string
 * funcs: _strset_s _strnset_s
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strset_s (silent on destination state after error); secure-CRT convention of zeroing dest[0]
 * note:  PROBE: after _strset_s / _strnset_s fails because the buffer is not NUL-terminated within size, is dest[0] set to 0
 *        (ucrt _VALIDATE_STRING-style behaviour) ? Return code is asserted in str_neg_strset_s_invalid.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static char *volatile pd;
static volatile size_t psz;

TC_CASE(strset_dest0)
{
    char b[8]; int i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    pd = b; psz = 4; tc_iph_count = 0;
    (void)_strset_s(pd, psz, 'y');
    CHECK_EQ_I((unsigned char)b[0], 0);
}
TC_CASE(strnset_dest0)
{
    char b[8]; int i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    pd = b; psz = 4; tc_iph_count = 0;
    (void)_strnset_s(pd, psz, 'y', 2);
    CHECK_EQ_I((unsigned char)b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(strset_dest0); TC_RUN(strnset_dest0);
    return tc_end();
}
