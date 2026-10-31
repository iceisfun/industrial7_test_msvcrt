/* TC
 * id:    mb_neg_mbstok_s_dbcs_trail_probe
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbstok_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbstok_s (multibyte-aware)
 * note:  PROBE: in CP932 a single-byte delimiter 'A' (0x41) must NOT split a DBCS character whose trail byte is 0x41 (0x82 0x41).
 */
#include "tc.h"
#include <mbstring.h>
typedef unsigned char *(__cdecl *tok_t)(unsigned char *, const unsigned char *, unsigned char **);
static tok_t p_tok;
static int old_cp;
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(dbcs_delim_trail_not_split_932)
{
    unsigned char s[] = {'x',0x82,0x41,'y',0}, e[] = {'x',0x82,0x41,'y',0}, *ctx = NULL, *t;
    static const unsigned char d[] = "A";
    CHECK(p_tok); if (!p_tok) return;
    t = p_tok(s, d, &ctx);
    CHECK(t == s); if (t) CHECK_MEM(t, e, 5);
    CHECK(p_tok(NULL, d, &ctx) == NULL);
}
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    p_tok = (tok_t)tc_crt_proc("_mbstok_s");
    TC_RUN(cp); TC_RUN(dbcs_delim_trail_not_split_932);
    _setmbcp(old_cp); return tc_end();
}
