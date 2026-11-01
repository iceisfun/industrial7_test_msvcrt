/* TC
 * id:    chr_neg_default_handler_fastfail_exact
 * area:  stdio_char
 * funcs: fgetc
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation
 * note:  PROBE: exact exit status of the UCRT default invalid-parameter handler for fgetc(NULL); expected fast-fail 0xC0000409.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
TC_CASE(status)
{
    DWORD code = 0; int ran = tc_spawn_self("fgetc", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    volatile FILE *np = NULL; tc_begin(); if (tc_child_name()) tc_iph_default();
    if (tc_child_name()) { (void)fgetc((FILE *)np); return 77; }
    TC_RUN(status); return tc_end();
}
