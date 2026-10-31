/* TC
 * id:    pfm_1_len_jzt_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1p7 (j, z); MS docs printf Size prefixes (j, z supported by UCRT)
 * note:  t is in pfm_1_len_t_integers. msvcr120 is covered by pfm_1_len_jzt_msvcr120_probe; earlier runtimes by pfv_neg_probe_zjt_pre120.
 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

static char b[64];
TC_CASE(ji) { volatile intmax_t v = -5000000000LL; sprintf(b, "%ji", v); CHECK_STR(b, "-5000000000"); }
TC_CASE(jX) { volatile uintmax_t v = 0xDEADBEEFCAFEULL; sprintf(b, "%jX", v); CHECK_STR(b, "DEADBEEFCAFE"); }
TC_CASE(ju) { volatile uintmax_t v = 18446744073709551615ULL; sprintf(b, "%ju", v); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(zi) { volatile ptrdiff_t v = -3; sprintf(b, "%zi", v); CHECK_STR(b, "-3"); }
TC_CASE(zo) { volatile size_t v = 8; sprintf(b, "%zo", v); CHECK_STR(b, "10"); }
TC_CASE(zx) { volatile size_t v = 255; sprintf(b, "%zx", v); CHECK_STR(b, "ff"); }
TC_CASE(zX) { volatile size_t v = 255; sprintf(b, "%zX", v); CHECK_STR(b, "FF"); }

int main(void)
{
    tc_begin();
    TC_RUN(ji); TC_RUN(jX); TC_RUN(ju); TC_RUN(zi); TC_RUN(zo); TC_RUN(zx); TC_RUN(zX);
    return tc_end();
}
