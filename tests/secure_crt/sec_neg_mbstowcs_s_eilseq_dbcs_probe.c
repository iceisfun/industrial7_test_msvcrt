/* TC
 * id:    sec_neg_mbstowcs_s_eilseq_dbcs_probe
 * area:  secure_crt
 * funcs: mbstowcs_s
 * procs: setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs mbstowcs_s (EILSEQ for invalid multibyte)
 * note:  PROBE: under setlocale(LC_ALL, ".932") (SKIP if unavailable), lead 0x81 followed by 0x20 / by NUL is invalid and believed to give EILSEQ (return value only; dest reset is probed in sec_neg_mbwc_s_dest_reset_probe).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
typedef char *(__cdecl *setlocale_t)(int, const char *);
static setlocale_t p_setlocale;
static volatile size_t v_sz = 8;
static int enter(void) { return p_setlocale && p_setlocale(LC_ALL, ".932") != NULL; }
TC_CASE(lead_byte_0x81_then_0x20_EILSEQ) { wchar_t b[8]; size_t n = 99; static const char s[] = "\x81\x20";
    if (!enter()) { if (p_setlocale) p_setlocale(LC_ALL, "C"); TC_SKIP(".932 unavailable"); } tc_iph_install(); b[0] = L'x';
    CHECK_EQ_I(mbstowcs_s(&n, b, v_sz, s, 8), EILSEQ); p_setlocale(LC_ALL, "C"); }
TC_CASE(truncated_lead_byte_at_end_EILSEQ) { wchar_t b[8]; size_t n = 99; static const char s[] = "a\x81";
    if (!enter()) { if (p_setlocale) p_setlocale(LC_ALL, "C"); TC_SKIP(".932 unavailable"); } tc_iph_install(); b[0] = L'x';
    CHECK_EQ_I(mbstowcs_s(&n, b, v_sz, s, 8), EILSEQ); p_setlocale(LC_ALL, "C"); }
TC_CASE(locale_restored_to_C) { char *r; if (!p_setlocale) TC_SKIP("no setlocale"); p_setlocale(LC_ALL, "C"); r = p_setlocale(LC_ALL, NULL); CHECK_STR(r, "C"); }
int main(void) { tc_begin(); p_setlocale = (setlocale_t)tc_crt_proc("setlocale"); TC_RUN(lead_byte_0x81_then_0x20_EILSEQ);
    TC_RUN(truncated_lead_byte_at_end_EILSEQ); TC_RUN(locale_restored_to_C); return tc_end(); }
