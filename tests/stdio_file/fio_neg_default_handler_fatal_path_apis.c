/* TC
 * id:    fio_neg_default_handler_fatal_path_apis
 * area:  stdio_file
 * kind:  negative
 * funcs: rename fsetpos freopen fopen
 * procs: tmpnam_s _fseeki64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation"; docs for rename, fsetpos, freopen, _fseek/_fseeki64, tmpnam_s
 * note:  Each bad call runs in a child process; without a handler the process must terminate abnormally.
 *        TRIAGE win10: (class F) the child now runs with the CRT default invalid-parameter handler (tc.h tc_begin restores it; mingw startup had installed a silent one, so
 *        children returned 77). (class A) c_rename: ucrtbase 10.0.19045 rename(NULL, x) does not validate and does not terminate (child exits 77, observed), consistent with
 *        fio_neg_rename_null_args (errno ENOENT, no handler call). The other four cases terminate as documented.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define NAME "tc_fio_hpath_" TC_S1(TC_CRT_VER) ".tmp"

TC_DECL_PROC(int, tmpnam_s, (char *, size_t));
TC_DECL_PROC(int, _fseeki64, (FILE *, long long, int));

static int child_main(const char *c)
{
    const char *volatile nul = NULL; FILE *f; volatile int r = 0; fpos_t *volatile np = NULL; volatile int org = 99;
    if (!strcmp(c, "rename")) r = rename(nul, "b");
    else if (!strcmp(c, "fsetpos")) { f = fopen(NAME, "wb"); r = fsetpos(f, np); }
    else if (!strcmp(c, "freopen")) { f = fopen(NAME, "wb"); f = freopen("x", nul, f); }
    else if (!strcmp(c, "fseeki64")) { f = fopen(NAME, "wb"); TC_LOAD_PROC(_fseeki64); if (!tc_p__fseeki64) return 78; r = tc_p__fseeki64(f, 0, org); }
    else if (!strcmp(c, "tmpnam_s")) { TC_LOAD_PROC(tmpnam_s); if (!tc_p_tmpnam_s) return 78; r = tc_p_tmpnam_s((char *)nul, 10); }
    (void)r;
    return 77;
}
static void chk(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 78); CHECK(code != 79);
}
TC_CASE(c_rename)
{
#if TC_AT_LEAST(140)
    /* observed ucrtbase 10.0.19045: rename(NULL, "b") does not invoke the invalid-parameter handler; it fails with ENOENT and returns,
     * so the child runs to its 'returned' exit code 77 instead of terminating */
    DWORD code = 0; int ran = tc_spawn_self("rename", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
#else
    chk("rename");
#endif
}
TC_CASE(c_fsetpos)  { chk("fsetpos"); }
TC_CASE(c_freopen)  { chk("freopen"); }
TC_CASE(c_fseeki64) { chk("fseeki64"); }
TC_CASE(c_tmpnam_s) { chk("tmpnam_s"); }

int main(void)
{
    const char *c;
    tc_begin();
    if (tc_child_name()) tc_iph_default();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(c_rename); TC_RUN(c_fsetpos); TC_RUN(c_freopen); TC_RUN(c_fseeki64); TC_RUN(c_tmpnam_s);
    DeleteFileA(NAME);
    return tc_end();
}
