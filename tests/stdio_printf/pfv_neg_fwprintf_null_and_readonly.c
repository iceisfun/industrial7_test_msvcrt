/* TC
 * id:    pfv_neg_fwprintf_null_and_readonly
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fwprintf: NULL stream/format -> invalid parameter handler, -1, EINVAL; output error returns negative
 * note:  null_stream/null_fmt/readonly_ret are documented (MS docs fwprintf); the EBADF errno and ferror assertions on a write to a read-only FILE are observed CRT behaviour, not documented.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static FILE *open_ro(void) { FILE *f = fopen("tc_pfv_fwro.tmp", "w"); if (!f) return NULL; fclose(f); return fopen("tc_pfv_fwro.tmp", "r"); }
TC_CASE(null_stream) { FILE *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = fwprintf(f, L"a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt) { const wchar_t *volatile fm = NULL; int n; tc_iph_count = 0; errno = 0; n = fwprintf(stdout, fm); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(readonly_ret) { FILE *f = open_ro(); int n; CHECK(f != NULL); if (!f) return; n = fwprintf(f, L"hello"); CHECK(n < 0); fclose(f); remove("tc_pfv_fwro.tmp"); }
TC_CASE(readonly_ferror) { FILE *f = open_ro(); CHECK(f != NULL); if (!f) return; fwprintf(f, L"hello"); CHECK(ferror(f) != 0); fclose(f); remove("tc_pfv_fwro.tmp"); }
TC_CASE(readonly_errno) { FILE *f = open_ro(); CHECK(f != NULL); if (!f) return; errno = 0; fwprintf(f, L"hello"); CHECK_EQ_I(errno, EBADF); fclose(f); remove("tc_pfv_fwro.tmp"); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_stream); TC_RUN(null_fmt); TC_RUN(readonly_ret); TC_RUN(readonly_ferror); TC_RUN(readonly_errno); return tc_end(); }
