/* TC
 * id:    sec_neg_tok_s_no_error_cases
 * area:  secure_crt
 * funcs: strtok_s wcstok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s, wcstok_s ("returns NULL when there are no more tokens")
 * note:  Inputs that look odd but are NOT errors: the invalid-parameter handler must not run and errno must stay 0.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static const char *volatile v_d;
static const wchar_t *volatile v_wd;

TC_CASE(strtok_empty_string) { char s[4] = ""; char *c = NULL; RESET(); CHECK(strtok_s(s, ",", &c) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(strtok_delims_only) { char s[8] = ",,,"; char *c = NULL; RESET(); CHECK(strtok_s(s, ",", &c) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(strtok_empty_delim_set) { char s[8] = "abc"; char *c = NULL; char *t; v_d = ""; RESET();
    t = strtok_s(s, v_d, &c); CHECK_STR(t, "abc"); CHECK(strtok_s(NULL, v_d, &c) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(wcstok_empty_string) { wchar_t s[4] = L""; wchar_t *c = NULL; RESET(); CHECK(wcstok_s(s, L",", &c) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(wcstok_delims_only) { wchar_t s[8] = L",,,"; wchar_t *c = NULL; RESET(); CHECK(wcstok_s(s, L",", &c) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(wcstok_empty_delim_set) { wchar_t s[8] = L"abc"; wchar_t *c = NULL; wchar_t *t; v_wd = L""; RESET();
    t = wcstok_s(s, v_wd, &c); CHECK_WSTR(t, L"abc"); CHECK(wcstok_s(NULL, v_wd, &c) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(strtok_empty_string); TC_RUN(strtok_delims_only); TC_RUN(strtok_empty_delim_set);
    TC_RUN(wcstok_empty_string); TC_RUN(wcstok_delims_only); TC_RUN(wcstok_empty_delim_set); return tc_end(); }
