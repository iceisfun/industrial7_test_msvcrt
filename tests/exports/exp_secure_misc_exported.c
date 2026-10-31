/* TC
 * id:    exp_secure_misc_exported
 * area:  exports
 * funcs:
 * procs: rand_s fopen_s _wfopen_s freopen_s _wfreopen_s tmpfile_s tmpnam_s _wtmpnam_s fread_s gets_s _cgets_s _sopen_s _wsopen_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs rand_s, fopen_s, tmpnam_s, fread_s, gets_s, _sopen_s
 * note:  qsort_s / bsearch_s are in exp_probe_qsort_s_bsearch_s (first CRT generation unsure).
 *        Temp file: tc_exp_misc.tmp in cwd, removed.
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
    static const char *const n[] = { "rand_s", "fopen_s", "_wfopen_s", "freopen_s", "_wfreopen_s", "tmpfile_s", "tmpnam_s", "_wtmpnam_s",
        "fread_s", "gets_s", "_cgets_s", "_sopen_s", "_wsopen_s", 0 };
    need(n);
}
TC_CASE(rand_s_call)
{
    int (__cdecl *p)(unsigned int *) = (int (__cdecl *)(unsigned int *))tc_crt_proc("rand_s");
    unsigned int v = 0; int i, nz = 0;
    if (!p) TC_SKIP("not exported");
    for (i = 0; i < 8; i++) { CHECK_EQ_I(p(&v), 0); nz |= v != 0; }
    CHECK(nz);
}
TC_CASE(fopen_family)
{
    int (__cdecl *fo)(void **, const char *, const char *) = (int (__cdecl *)(void **, const char *, const char *))tc_crt_proc("fopen_s");
    int (__cdecl *fc)(void *) = (int (__cdecl *)(void *))tc_crt_proc("fclose");
    size_t (__cdecl *fw)(const void *, size_t, size_t, void *) = (size_t (__cdecl *)(const void *, size_t, size_t, void *))tc_crt_proc("fwrite");
    size_t (__cdecl *frs)(void *, size_t, size_t, size_t, void *) = (size_t (__cdecl *)(void *, size_t, size_t, size_t, void *))tc_crt_proc("fread_s");
    void *f = NULL; char b[8] = {0};
    if (!fo || !fc || !fw || !frs) TC_SKIP("not exported");
    CHECK_EQ_I(fo(&f, "tc_exp_misc.tmp", "wb"), 0); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fw("abcd", 1, 4, f), 4); fc(f); }
    f = NULL;
    CHECK_EQ_I(fo(&f, "tc_exp_misc.tmp", "rb"), 0); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(frs(b, sizeof b, 1, 4, f), 4); CHECK_MEM(b, "abcd", 4); fc(f); }
    DeleteFileA("tc_exp_misc.tmp");
}
TC_CASE(tmp_family)
{
    int (__cdecl *tn)(char *, size_t) = (int (__cdecl *)(char *, size_t))tc_crt_proc("tmpnam_s");
    char b[260];
    if (!tn) TC_SKIP("not exported");
    CHECK_EQ_I(tn(b, sizeof b), 0); CHECK(b[0] != 0);
}
int main(void) { tc_begin(); TC_RUN(exported_all); TC_RUN(rand_s_call); TC_RUN(fopen_family); TC_RUN(tmp_family); return tc_end(); }
