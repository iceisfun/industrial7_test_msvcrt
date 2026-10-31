/* TC
 * id:    fio_probe_tmpfile_legacy
 * area:  stdio_file
 * funcs:
 * procs: tmpfile, fwrite, rewind, fread, fclose
 * crt:   win7
 * tier:  p2
 * conf:  probe
 * ref:   MS docs tmpfile
 * note:  PROBE: does tmpfile() succeed for the running user on Win7-era CRTs? Legacy CRTs create the file in the
 *        drive root, which a standard user may not be allowed to do (NULL with errno EACCES is the expected non-admin result).
 *        A failure here on a non-elevated account is informative, not necessarily a bug.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(FILE *, tmpfile, (void));
TC_DECL_PROC(size_t, fwrite, (const void *, size_t, size_t, FILE *));
TC_DECL_PROC(size_t, fread, (void *, size_t, size_t, FILE *));
TC_DECL_PROC(void, rewind, (FILE *));
TC_DECL_PROC(int, fclose, (FILE *));
static FILE * volatile g_f;

TC_CASE(non_null)
{
    if (!tc_p_tmpfile) TC_SKIP("tmpfile missing");
    errno = 0;
    g_f = tc_p_tmpfile();
    if (!g_f) { char b[16]; tc_i64(errno, b); tc_write("TC NOTE tmpfile returned NULL errno="); tc_write(b); tc_write("\n"); }
    CHECK(g_f != NULL);
}
TC_CASE(round_trip)
{
    char buf[16] = {0};
    if (!g_f || !tc_p_fwrite || !tc_p_fread || !tc_p_rewind || !tc_p_fclose) TC_SKIP("tmpfile failed");
    CHECK_EQ_U(tc_p_fwrite("hello", 1, 5, g_f), 5);
    tc_p_rewind(g_f);
    CHECK_EQ_U(tc_p_fread(buf, 1, 5, g_f), 5);
    CHECK_MEM(buf, "hello", 5);
    CHECK_EQ_I(tc_p_fclose(g_f), 0);
    g_f = NULL;
}
int main(void) { tc_begin(); TC_LOAD_PROC(tmpfile); TC_LOAD_PROC(fwrite); TC_LOAD_PROC(fread); TC_LOAD_PROC(rewind); TC_LOAD_PROC(fclose); TC_RUN(non_null); TC_RUN(round_trip); return tc_end(); }
