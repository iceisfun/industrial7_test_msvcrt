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
TC_CASE(c_rename)   { chk("rename"); }
TC_CASE(c_fsetpos)  { chk("fsetpos"); }
TC_CASE(c_freopen)  { chk("freopen"); }
TC_CASE(c_fseeki64) { chk("fseeki64"); }
TC_CASE(c_tmpnam_s) { chk("tmpnam_s"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(c_rename); TC_RUN(c_fsetpos); TC_RUN(c_freopen); TC_RUN(c_fseeki64); TC_RUN(c_tmpnam_s);
    DeleteFileA(NAME);
    return tc_end();
}
