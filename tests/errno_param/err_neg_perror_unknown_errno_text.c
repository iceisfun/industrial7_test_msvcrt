/* TC
 * id:    err_neg_perror_unknown_errno_text
 * area:  errno_param
 * kind:  negative
 * funcs: perror freopen
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs perror (out-of-range errno text not documented; prints the string, a colon, a blank, the system error message for errno, and a newline, to stderr)
 * note:  A child process redirects its stderr with freopen to a temp file (tc_err_neg_perror_text_<ver>.tmp), calls perror with an out-of-range
 *        errno and exits 77; the parent reads the file with kernel32. Only the shape "pfx: <non-empty text>\n" is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#define TMPF "tc_err_neg_perror_text_" TC_STR(TC_CRT_VER) ".tmp"

static volatile int v_e;

static int run_child(const char *name, char *buf, int cap)
{
    DWORD code = 0, n = 0; HANDLE h; int ran;
    DeleteFileA(TMPF);
    ran = tc_spawn_self(name, &code, 20000);
    buf[0] = 0;
    h = CreateFileA(TMPF, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, (DWORD)(cap - 1), &n, NULL); buf[n] = 0; CloseHandle(h); }
    DeleteFileA(TMPF);
    return ran && code == 77 ? (int)n : -1;
}
static void shape(const char *name)
{
    char b[512]; int n = run_child(name, b, sizeof b), i, nl = 0;
    CHECK(n > 6);
    if (n > 6) {
        CHECK(b[0] == 'p' && b[1] == 'f' && b[2] == 'x' && b[3] == ':' && b[4] == ' ');
        CHECK(b[5] != '\n');
        CHECK_EQ_I(b[n - 1], '\n');
        for (i = 0; i < n; i++) if (b[i] == '\n') nl++;
        CHECK_EQ_I(nl, 1);
    }
}
TC_CASE(errno_9999)    { shape("e9999"); }
TC_CASE(errno_minus_1) { shape("em1"); }
TC_CASE(errno_intmax)  { shape("emax"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        v_e = c[1] == '9' ? 9999 : c[1] == 'm' && c[2] == '1' ? -1 : INT_MAX;
        if (!freopen(TMPF, "w", stderr)) return 3;
        errno = v_e;
        perror("pfx");
        fflush(stderr); fclose(stderr);
        return 77;
    }
    TC_RUN(errno_9999); TC_RUN(errno_minus_1); TC_RUN(errno_intmax);
    return tc_end();
}
