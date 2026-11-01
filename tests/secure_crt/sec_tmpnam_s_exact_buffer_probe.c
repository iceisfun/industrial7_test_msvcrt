/* TC
 * id:    sec_tmpnam_s_exact_buffer_probe
 * area:  secure_crt
 * funcs:
 * procs: tmpnam_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs tmpnam_s (L_tmpnam_s)
 * note:  Observed on Win10 22H2 19045 ucrtbase: tmpnam_s(buf, 18) fails with ERANGE because the generated name is a full path
 *        in the temp directory (probe: first accepted size 45 for C:\\Users\\ADMINI~1\\AppData\\Local\\Temp\\u5v8.19). The old
 *        fixed size 18 (my recollection of L_tmpnam_s) was wrong. The test now checks the exact-buffer property without hard-
 *        coding the temp path length (smallest accepted size == strlen(name)+1, one less is ERANGE); only the unique-suffix
 *        length (base-36 counter) could in theory change between two calls. Environment: the name is built from the temp directory
 *        (class F); the lower bound is derived from GetTempPathA instead of a hard-coded 18, so a short TMP does not break it.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *tmpnam_s_t)(char *, size_t);
static tmpnam_s_t p_tmpnam_s;

/* observed: ucrtbase tmpnam_s returns a FULL temp-directory path (e.g. C:\\Users\\...\\AppData\\Local\\Temp\\u5v8.19), so the 18-char L_tmpnam_s
 * style size from the headers is far too small (ERANGE). The exact-buffer property is checked instead: the smallest accepted size is
 * strlen(name) + 1, and one less is ERANGE. */
TC_CASE(smallest_accepted_size_is_strlen_plus_1)
{
    char b[400]; size_t i, first = 0; int r; size_t len;
    for (i = 1; i < sizeof b; i++) { r = p_tmpnam_s(b, i); if (r == 0) { first = i; break; } CHECK_EQ_I(r, 34); }
    { char t[MAX_PATH + 2]; DWORD tl = GetTempPathA(sizeof t, t); CHECK(tl > 0 && tl < sizeof t); CHECK(first > tl); }   /* name = temp dir + file name */
    len = 0; while (b[len]) len++;
    CHECK_EQ_I(len + 1, first);
    CHECK_EQ_I(p_tmpnam_s(b, first - 1), 34);
}
int main(void)
{
    tc_begin();
    p_tmpnam_s = (tmpnam_s_t)tc_crt_proc("tmpnam_s");
    CHECK(p_tmpnam_s); if (!p_tmpnam_s) return tc_end();
    TC_RUN(smallest_accepted_size_is_strlen_plus_1);
    return tc_end();
}
