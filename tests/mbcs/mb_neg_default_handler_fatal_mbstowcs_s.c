/* TC
 * id:    mb_neg_default_handler_fatal_mbstowcs_s
 * area:  mbcs
 * kind:  negative
 * funcs:
 * procs: mbstowcs_s wcstombs_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs mbstowcs_s, wcstombs_s, "Parameter validation"
 * note:  Invalid arguments (NULL source; too-small buffer without _TRUNCATE) reach the default handler and kill the child.
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *mbstowcs_s_t)(size_t *, wchar_t *, size_t, const char *, size_t);
typedef int (__cdecl *wcstombs_s_t)(size_t *, char *, size_t, const wchar_t *, size_t);

static void run_child(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 78);  /* 78 = proc missing in child */
}
TC_CASE(mbstowcs_null_src) { run_child("mbnull"); }
TC_CASE(wcstombs_small_buf) { run_child("wcsmall"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();   /* mingw startup installs a no-op handler; restore the CRT default (fatal) */
        size_t r = 0; char b[2]; static const char *volatile nul = NULL;
        static const wchar_t *volatile wide = L"toolong";
        mbstowcs_s_t pm = (mbstowcs_s_t)tc_crt_proc("mbstowcs_s"); wcstombs_s_t pw = (wcstombs_s_t)tc_crt_proc("wcstombs_s");
        if (!pm || !pw) return 78;
        if (c[0] == 'm') pm(&r, NULL, 0, nul, 0);
        else pw(&r, b, sizeof b, wide, 7);
        return 77;
    }
    TC_RUN(mbstowcs_null_src); TC_RUN(wcstombs_small_buf);
    return tc_end();
}
