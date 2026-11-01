/* TC
 * id:    gio_neg_probe_tempnam_long_prefix
 * area:  lowio
 * funcs: _tempnam _wtempnam free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _tempnam (no documented length limit)
 * note:  PROBE result (class C). OBSERVED on Win10 22H2 ucrtbase: _tempnam(".", prefix) with a 260- or 300-char narrow prefix
 *        crashes the process (access violation, exit 0xC0000005; no NULL/ENOMEM failure), while _wtempnam(".", 300 wide chars)
 *        succeeds and returns a path containing the prefix. Each case runs in a child process.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* Child name: n300 / n260 (narrow prefix length) or w300 (wide). Exit code: 0 = non-NULL result containing the prefix (freed),
 * 1 = NULL, 2 = non-NULL without the prefix. Class C: some of these fault on Win10 22H2 ucrtbase, so every case runs in a child. */
static int child_main(const char *c)
{
    int n = (c[1] - '0') * 100 + (c[2] - '0') * 10 + (c[3] - '0'), i;
    tc_iph_install();
    if (c[0] == 'w') {
        wchar_t pre[400], *r;
        for (i = 0; i < n; i++) pre[i] = L'a';
        pre[n] = 0;
        r = _wtempnam(L".", pre);
        if (!r) return 1;
        if (!wcsstr(r, pre)) return 2;
        free(r); return 0;
    } else {
        char pre[400], *r;
        for (i = 0; i < n; i++) pre[i] = 'a';
        pre[n] = 0;
        r = _tempnam(".", pre);
        if (!r) return 1;
        if (!strstr(r, pre)) return 2;
        free(r); return 0;
    }
}
static void run(const char *name, DWORD want)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    char b[24]; tc_i64((long long)code, b); tc_write("TC NOTE " TC_TEST_ID " "); tc_write(name); tc_write(" child code="); tc_write(b); tc_write("\n");
    CHECK(ran); CHECK_EQ_U(code, want);
}
TC_CASE(prefix_300_chars) { run("n300", 0xC0000005u); }   /* Class C: observed crash */
TC_CASE(prefix_260_boundary) { run("n260", 0xC0000005u); }   /* Class C: observed crash */
TC_CASE(wide_300) { run("w300", 0); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(prefix_300_chars); TC_RUN(prefix_260_boundary); TC_RUN(wide_300);
    return tc_end();
}
