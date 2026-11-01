/* TC
 * id:    chr_neg_wide_default_handler
 * area:  stdio_char
 * funcs: fgetwc fputws
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation
 * note:  NULL FILE* to wide functions terminates under the default handler (children).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
TC_CASE(fgetwc_null) { DWORD code = 0; int ran = tc_spawn_self("fgetwc", &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); }
TC_CASE(fputws_null) { DWORD code = 0; int ran = tc_spawn_self("fputws", &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); }
int main(void)
{
    const char *c; volatile FILE *np = NULL; tc_begin(); if (tc_child_name()) tc_iph_default();
    if ((c = tc_child_name()) != NULL) { if (c[1] == 'g') (void)fgetwc((FILE *)np); else (void)fputws(L"a", (FILE *)np); return 77; }
    TC_RUN(fgetwc_null); TC_RUN(fputws_null); return tc_end();
}
