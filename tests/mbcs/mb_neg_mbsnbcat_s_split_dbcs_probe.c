/* TC
 * id:    mb_neg_mbsnbcat_s_split_dbcs_probe
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbsnbcat_s _mbsncat_s
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsnbcat_s
 * note:  PROBE: in CP932, a byte count that would split a DBCS character does not append the orphan lead byte (result terminated);
 *        with _TRUNCATE and a dst too small for the 2nd DBCS char, only whole chars are kept and STRUNCATE is returned.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
typedef int (__cdecl *ncat_t)(unsigned char *, size_t, const unsigned char *, size_t);
static ncat_t p_nbcat;
static int old_cp;
static unsigned char sb[] = {0x82,0xA0,0x82,0xA2,0};
static const unsigned char * volatile src = sb;
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(split_count_result_probe)
{
    unsigned char d[16] = {'a',0}, e[4] = {'a',0x82,0xA0,0};
    CHECK(p_nbcat); if (!p_nbcat) return;
    CHECK_EQ_I(p_nbcat(d, 16, src, 3), 0);
    CHECK_MEM(d, e, 4);
}
TC_CASE(truncate_split_probe)
{
    unsigned char d[5] = {'a',0}, e[4] = {'a',0x82,0xA0,0};
    CHECK(p_nbcat); if (!p_nbcat) return;
    /* room for 'a' + one DBCS char + NUL only */
    CHECK_EQ_I(p_nbcat(d, 4, src, (size_t)-1), STRUNCATE);
    CHECK_MEM(d, e, 4);
}
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    p_nbcat = (ncat_t)tc_crt_proc("_mbsnbcat_s");
    TC_RUN(cp); TC_RUN(split_count_result_probe); TC_RUN(truncate_split_probe);
    _setmbcp(old_cp); return tc_end();
}
