/* TC
 * id:    sec_neg_wcstok_s_args
 * area:  secure_crt
 * funcs: wcstok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s, wcstok_s
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const wchar_t *v_delim;

TC_CASE(null_delim_returns_null_EINVAL) { wchar_t s[] = L"a b"; wchar_t *ctx = NULL; v_delim = NULL; RESET();
    CHECK(wcstok_s(s, (const wchar_t *)v_delim, &ctx) == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_context_returns_null_EINVAL) { wchar_t s[] = L"a b"; wchar_t **volatile pc = NULL; RESET();
    CHECK(wcstok_s(s, L" ", (wchar_t **)pc) == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_counts) { wchar_t s[] = L"a b"; wchar_t *ctx = NULL; wchar_t **volatile pc = NULL; v_delim = NULL; RESET();
    wcstok_s(s, (const wchar_t *)v_delim, &ctx); CHECK_EQ_I(tc_iph_count, 1);
    wcstok_s(s, L" ", (wchar_t **)pc); CHECK_EQ_I(tc_iph_count, 2); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_delim_returns_null_EINVAL); TC_RUN(null_context_returns_null_EINVAL); TC_RUN(handler_counts); return tc_end(); }
