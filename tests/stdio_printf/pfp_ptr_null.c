/* TC
 * id:    pfp_ptr_null
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf p type
 * note:  MS CRTs print NULL as 16 zeros (not (nil)/(null)); x64 only.
 */
#include "tc.h"
#include <stdio.h>

#ifdef _WIN64
TC_CASE(null_ptr) { char b[64]; void *volatile p = NULL; int n = sprintf(b, "%p", p); CHECK_STR(b, "0000000000000000"); CHECK_EQ_I(n, 16); }
TC_CASE(null_ctx) { char b[64]; void *volatile p = NULL; int n = sprintf(b, "<%p>", p); CHECK_STR(b, "<0000000000000000>"); CHECK_EQ_I(n, 18); }
#else
TC_CASE(skip32) { TC_SKIP("x64 only"); }
#endif

int main(void)
{
    tc_begin();
#ifdef _WIN64
    TC_RUN(null_ptr); TC_RUN(null_ctx);
#else
    TC_RUN(skip32);
#endif
    return tc_end();
}
