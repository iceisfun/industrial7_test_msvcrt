/* TC
 * id:    fio_wtmpnam_wtempnam
 * area:  stdio_file
 * funcs:
 * procs: _wtmpnam, _wtempnam
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs tmpnam, _wtmpnam; _tempnam, _wtempnam
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

TC_DECL_PROC(wchar_t *, _wtmpnam, (wchar_t *));
TC_DECL_PROC(wchar_t *, _wtempnam, (const wchar_t *, const wchar_t *));
static wchar_t * volatile nullbuf = NULL;
static const wchar_t * volatile dir = L".";
static const wchar_t * volatile pfx = L"tcp";

TC_CASE(wtmpnam)
{
    wchar_t *a, *b; wchar_t first[300];
    if (!tc_p__wtmpnam) TC_SKIP("missing");
    a = tc_p__wtmpnam(nullbuf);
    CHECK(a != NULL);
    if (!a) return;
    CHECK(wcslen(a) > 0 && wcslen(a) < L_tmpnam);
    if (wcslen(a) >= 300) { CHECK(0); return; }
    wcscpy(first, a);
    b = tc_p__wtmpnam(nullbuf);
    CHECK(b != NULL);
    if (b) CHECK(!tc_wcsequ(first, b));
}
TC_CASE(wtempnam)
{
    wchar_t *a, *b;
    if (!tc_p__wtempnam) TC_SKIP("missing");
    _putenv("TMP=");
    a = tc_p__wtempnam(dir, pfx);
    b = tc_p__wtempnam(dir, pfx);
    CHECK(a != NULL && b != NULL);
    if (a) CHECK(wcsstr(a, L"tcp") != NULL);
    if (a && b) CHECK(!tc_wcsequ(a, b));
    free(a); free(b);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_wtmpnam); TC_LOAD_PROC(_wtempnam); TC_RUN(wtmpnam); TC_RUN(wtempnam); return tc_end(); }
