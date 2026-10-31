/* TC
 * id:    sec_neg_tok_s_context_untouched_probe
 * area:  secure_crt
 * funcs: strtok_s wcstok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok_s, wcstok_s (NULL delimiter is an invalid parameter -> EINVAL)
 * note:  PROBE: after the EINVAL for a NULL delimiter, *context keeps its prior (sentinel) value and the source string is not
 *        modified (parameter validation runs before any tokenizing). The EINVAL itself is documented and checked separately.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>

static const char *volatile v_d;
static const wchar_t *volatile v_wd;
static char sentinel_c[4];
static wchar_t sentinel_w[4];

TC_CASE(strtok_einval) { char s[] = "a,b"; char *c = sentinel_c; v_d = NULL; tc_iph_count = 0; errno = 0;
    CHECK(strtok_s(s, v_d, &c) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strtok_ctx_and_src_untouched_probe) { char s[] = "a,b"; char *c = sentinel_c; v_d = NULL;
    strtok_s(s, v_d, &c); CHECK(c == sentinel_c); CHECK_STR(s, "a,b"); }
TC_CASE(wcstok_einval) { wchar_t s[] = L"a,b"; wchar_t *c = sentinel_w; v_wd = NULL; tc_iph_count = 0; errno = 0;
    CHECK(wcstok_s(s, v_wd, &c) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcstok_ctx_and_src_untouched_probe) { wchar_t s[] = L"a,b"; wchar_t *c = sentinel_w; v_wd = NULL;
    wcstok_s(s, v_wd, &c); CHECK(c == sentinel_w); CHECK_WSTR(s, L"a,b"); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(strtok_einval); TC_RUN(strtok_ctx_and_src_untouched_probe); TC_RUN(wcstok_einval); TC_RUN(wcstok_ctx_and_src_untouched_probe); return tc_end(); }
