/* TC
 * id:    exp_secure_env_path_exported
 * area:  exports
 * funcs:
 * procs: getenv_s _dupenv_s _putenv_s _wgetenv_s _wdupenv_s _wputenv_s _searchenv_s _makepath_s _splitpath_s _wmakepath_s _wsplitpath_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs getenv_s, _putenv_s, _splitpath_s, _makepath_s
 */
#include "tc.h"
static void need(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(*n); tc_write("\n"); }
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("unexpected export: "); tc_write(*n); tc_write("\n"); }
}

TC_CASE(exported_all)
{
    static const char *const n[] = { "getenv_s", "_dupenv_s", "_putenv_s", "_wgetenv_s", "_wdupenv_s", "_wputenv_s", "_searchenv_s",
        "_makepath_s", "_splitpath_s", "_wmakepath_s", "_wsplitpath_s", 0 };
    need(n);
}
TC_CASE(putenv_getenv_group)
{
    int (__cdecl *put)(const char *, const char *) = (int (__cdecl *)(const char *, const char *))tc_crt_proc("_putenv_s");
    int (__cdecl *get)(size_t *, char *, size_t, const char *) = (int (__cdecl *)(size_t *, char *, size_t, const char *))tc_crt_proc("getenv_s");
    char b[32]; size_t n = 99;
    if (!put || !get) TC_SKIP("not exported");
    CHECK_EQ_I(put("TC_EXP_ENV_VAR", "val1"), 0);
    CHECK_EQ_I(get(&n, b, sizeof b, "TC_EXP_ENV_VAR"), 0); CHECK_EQ_U(n, 5); CHECK_STR(b, "val1");
    CHECK_EQ_I(put("TC_EXP_ENV_VAR", ""), 0);   /* removes */
    n = 99; CHECK_EQ_I(get(&n, b, sizeof b, "TC_EXP_ENV_VAR"), 0); CHECK_EQ_U(n, 0);
}
TC_CASE(dupenv)
{
    int (__cdecl *put)(const char *, const char *) = (int (__cdecl *)(const char *, const char *))tc_crt_proc("_putenv_s");
    int (__cdecl *dup)(char **, size_t *, const char *) = (int (__cdecl *)(char **, size_t *, const char *))tc_crt_proc("_dupenv_s");
    void (__cdecl *fr)(void *) = (void (__cdecl *)(void *))tc_crt_proc("free");
    char *p = NULL; size_t n = 0;
    if (!put || !dup || !fr) TC_SKIP("not exported");
    CHECK_EQ_I(put("TC_EXP_ENV_DUP", "zz"), 0);
    CHECK_EQ_I(dup(&p, &n, "TC_EXP_ENV_DUP"), 0); CHECK(p != NULL); if (p) { CHECK_STR(p, "zz"); CHECK_EQ_U(n, 3); fr(p); }
    put("TC_EXP_ENV_DUP", "");
}
TC_CASE(path_group)
{
    int (__cdecl *sp)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t) =
        (int (__cdecl *)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t))tc_crt_proc("_splitpath_s");
    int (__cdecl *mk)(char *, size_t, const char *, const char *, const char *, const char *) =
        (int (__cdecl *)(char *, size_t, const char *, const char *, const char *, const char *))tc_crt_proc("_makepath_s");
    char dr[4], di[64], fn[64], ex[16], out[128];
    if (!sp || !mk) TC_SKIP("not exported");
    CHECK_EQ_I(sp("C:\\dir\\sub\\name.ext", dr, 4, di, 64, fn, 64, ex, 16), 0);
    CHECK_STR(dr, "C:"); CHECK_STR(di, "\\dir\\sub\\"); CHECK_STR(fn, "name"); CHECK_STR(ex, ".ext");
    CHECK_EQ_I(mk(out, sizeof out, "C:", "\\a\\b\\", "file", "txt"), 0); CHECK_STR(out, "C:\\a\\b\\file.txt");
}
int main(void) { tc_begin(); TC_RUN(exported_all); TC_RUN(putenv_getenv_group); TC_RUN(dupenv); TC_RUN(path_group); return tc_end(); }
