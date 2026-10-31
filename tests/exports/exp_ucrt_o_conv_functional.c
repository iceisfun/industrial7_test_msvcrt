/* TC
 * id:    exp_ucrt_o_conv_functional
 * area:  exports
 * funcs: 
 * procs: _o_atoi _o_atof _o_strtol _o_atol
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   ucrtbase _o_ aliases (undocumented)
 * note:  PROBE: _o_ conversion aliases give the same results as atoi/atof/strtol/atol.
 */
#include "tc.h"


typedef int    (__cdecl *atoi_t)(const char *);
typedef long   (__cdecl *atol_t)(const char *);
typedef double (__cdecl *atof_t)(const char *);
typedef long   (__cdecl *strtol_t)(const char *, char **, int);
static atoi_t p_atoi; static atol_t p_atol; static atof_t p_atof; static strtol_t p_strtol;
static const char *volatile s42 = "42", *volatile sf = "1.5", *volatile shex = "0x1F", *volatile sneg = "-123";

TC_CASE(atoi_42) { if (!p_atoi) TC_SKIP("not exported"); CHECK_EQ_I(p_atoi(s42), 42); }
TC_CASE(atof_1_5) { if (!p_atof) TC_SKIP("not exported"); CHECK_BITS_D(p_atof(sf), 1.5); }
TC_CASE(strtol_hex)
{
    char *end = NULL; long v;
    if (!p_strtol) TC_SKIP("not exported");
    v = p_strtol(shex, &end, 16);
    CHECK_EQ_I(v, 31);
    CHECK(end == shex + 4);
}
TC_CASE(atol_negative) { if (!p_atol) TC_SKIP("not exported"); CHECK_EQ_I(p_atol(sneg), -123); }
int main(void)
{
    tc_begin();
    p_atoi = (atoi_t)tc_crt_proc("_o_atoi"); p_atof = (atof_t)tc_crt_proc("_o_atof");
    p_strtol = (strtol_t)tc_crt_proc("_o_strtol"); p_atol = (atol_t)tc_crt_proc("_o_atol");
    TC_RUN(atoi_42); TC_RUN(atof_1_5); TC_RUN(strtol_hex); TC_RUN(atol_negative);
    return tc_end();
}
