/* TC
 * id:    dat_wargv_matches_cmdline
 * area:  data_exports
 * kind:  positive
 * funcs: __p___wargv __p___argv
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs __wargv, __argv
 * note:  PROBE: if *__p___wargv() is populated in a narrow main() program, wargv[0] widens argv[0] (ASCII only; non-ASCII argv0 skips). Skipped when wargv is NULL.
 */
#include "tc.h"

extern wchar_t ***__cdecl __p___wargv(void);
extern char ***__cdecl __p___argv(void);

TC_CASE(wargv0_widen_equals_argv0)
{
    wchar_t ***w = __p___wargv(); char ***a = __p___argv(); const wchar_t *w0; const char *a0; int i;
    if (!w || !*w) TC_SKIP("wargv not populated");
    if (!a || !*a) { CHECK(a && *a); return; }
    w0 = (*w)[0]; a0 = (*a)[0];
    if (!w0 || !a0) { CHECK(w0 && a0); return; }
    for (i = 0; a0[i] || w0[i]; i++) {
        if ((unsigned char)a0[i] > 0x7f) TC_SKIP("non-ASCII argv0");
        if ((wchar_t)(unsigned char)a0[i] != w0[i]) { CHECK_EQ_I(w0[i], a0[i]); return; }
    }
}
int main(void) { tc_begin(); TC_RUN(wargv0_widen_equals_argv0); return tc_end(); }
