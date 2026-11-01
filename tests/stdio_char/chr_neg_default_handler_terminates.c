/* TC
 * id:    chr_neg_default_handler_terminates
 * area:  stdio_char
 * funcs: fgetc fputc fgets fputs ungetc
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation
 * note:  Children via tc_spawn_self; the parent never calls the failing functions. Streams on NUL need no temp files.
 *        HARNESS FIX (class F, Win10 22H2): mingw crt2.o installs a no-op invalid-parameter handler at startup, so the child survived (exit 77 instead of
 *        0xC0000409); the child now calls tc_iph_default() (tc.h) to restore the CRT default before the call under test.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
static int spawn(const char *n) { DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); return (int)code; }
TC_CASE(fgetc_null) { spawn("fgetc"); }
TC_CASE(fputc_null) { spawn("fputc"); }
TC_CASE(fgets_null) { spawn("fgets"); }
TC_CASE(fputs_null) { spawn("fputs"); }
TC_CASE(ungetc_null) { spawn("ungetc"); }
static int child(const char *c)
{
    volatile FILE *np = NULL; volatile const char *ns = NULL; volatile char *nb = NULL; FILE *f;
    tc_iph_default();  /* undo mingw startup no-op handler: this test is about the CRT default */
    if (c[1]=='g' && c[4]=='c') (void)fgetc((FILE *)np);
    else if (c[1]=='p' && c[4]=='c') (void)fputc('a', (FILE *)np);
    else if (c[1]=='g') { f = fopen("NUL", "r"); (void)fgets((char *)nb, 10, f); }
    else if (c[1]=='p') { f = fopen("NUL", "w"); (void)fputs((const char *)ns, f); }
    else (void)ungetc('a', (FILE *)np);
    return 77;
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(fgetc_null); TC_RUN(fputc_null); TC_RUN(fgets_null); TC_RUN(fputs_null); TC_RUN(ungetc_null);
    return tc_end();
}
