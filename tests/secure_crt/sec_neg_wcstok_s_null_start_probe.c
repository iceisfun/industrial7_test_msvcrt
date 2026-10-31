/* TC
 * id:    sec_neg_wcstok_s_null_start_probe
 * area:  secure_crt
 * funcs: wcstok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok_s, wcstok_s (str NULL continues from *context)
 * note:  PROBE: wcstok_s(NULL, delim, &ctx) with ctx==NULL and no prior state is believed to be an invalid parameter
 *        (returns NULL, EINVAL, handler once); once the string is exhausted a NULL str returns NULL without error.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static wchar_t *volatile v_str;

TC_CASE(null_str_null_ctx) { wchar_t *ctx = NULL; v_str = NULL; RESET();
    CHECK(wcstok_s(v_str, L" ", &ctx) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_str_after_exhausted) { wchar_t s[] = L"a"; wchar_t *ctx = NULL, *t;
    t = wcstok_s(s, L" ", &ctx); CHECK(t != NULL); t = wcstok_s(NULL, L" ", &ctx); CHECK(t == NULL); RESET();
    v_str = NULL; t = wcstok_s(v_str, L" ", &ctx); CHECK(t == NULL); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_str_null_ctx); TC_RUN(null_str_after_exhausted); return tc_end(); }
