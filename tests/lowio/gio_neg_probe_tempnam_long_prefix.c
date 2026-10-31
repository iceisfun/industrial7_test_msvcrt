/* TC
 * id:    gio_neg_probe_tempnam_long_prefix
 * area:  lowio
 * funcs: _tempnam _wtempnam free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _tempnam (no documented length limit)
 * note:  PROBE: _tempnam with a prefix of 260 and 300 characters. Only asserts no crash and, if non-NULL, that
 *        the string contains the prefix and can be free()d. The real result (NULL or not) is recorded in output.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static void report(const char *tag, void *r)
{
    char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(tag); tc_write(r ? " non-NULL" : " NULL errno=");
    if (!r) { tc_i64(errno, b); tc_write(b); }
    tc_write("\n");
}
static void run(int n, const char *tag)
{
    char pre[400]; char *r; int i;
    for (i = 0; i < n; i++) pre[i] = 'a';
    pre[n] = 0;
    errno = 0;
    r = _tempnam(".", pre);
    report(tag, r);
    if (r) { CHECK(strstr(r, pre) != NULL); free(r); }
}
TC_CASE(prefix_300_chars) { run(300, "narrow300"); }
TC_CASE(prefix_260_boundary) { run(260, "narrow260"); }
TC_CASE(wide_300)
{
    wchar_t pre[400], *r; int i;
    for (i = 0; i < 300; i++) pre[i] = L'a';
    pre[300] = 0;
    errno = 0;
    r = _wtempnam(L".", pre);
    report("wide300", r);
    if (r) { CHECK(wcsstr(r, pre) != NULL); free(r); }
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(prefix_300_chars); TC_RUN(prefix_260_boundary); TC_RUN(wide_300);
    return tc_end();
}
