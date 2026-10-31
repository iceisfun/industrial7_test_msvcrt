/* TC
 * id:    sec_neg_overlap_detect_probe
 * area:  secure_crt
 * funcs:
 * procs: strcpy_s strcat_s strncpy_s memcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strcpy_s: behaviour undefined if source and destination overlap
 * note:  PROBE: overlapping buffers are documented undefined; asserts only no crash, return in {0,EINVAL,ERANGE}, handler count <= 1. Real-Windows value is in the actual results (release CRTs vs ucrt may detect overlap).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile size_t v_n;
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
#define strcpy_s tc_p_strcpy_s
#define strcat_s tc_p_strcat_s
#define strncpy_s tc_p_strncpy_s
#define memcpy_s tc_p_memcpy_s
#define OKRET(r) ((r) == 0 || (r) == EINVAL || (r) == ERANGE)

TC_CASE(strcpy_s_src_inside_dest) { char b[16] = "abcdef"; int r; RESET(); r = strcpy_s(b, sizeof b, b + 2);
    CHECK(OKRET(r)); CHECK(tc_iph_count <= 1); }
TC_CASE(strcat_s_src_inside_dest) { char b[16] = "abc"; int r; RESET(); r = strcat_s(b, sizeof b, b + 1);
    CHECK(OKRET(r)); CHECK(tc_iph_count <= 1); }
TC_CASE(strncpy_s_overlap) { char b[16] = "abcdef"; int r; v_n = 4; RESET(); r = strncpy_s(b, sizeof b, b + 2, v_n);
    CHECK(OKRET(r)); CHECK(tc_iph_count <= 1); }
TC_CASE(memcpy_s_overlap_forward) { char b[16] = "abcdefgh"; int r; v_n = 4; RESET(); r = memcpy_s(b + 2, 8, b, v_n);
    CHECK(OKRET(r)); CHECK(tc_iph_count <= 1); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strcat_s); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(memcpy_s);
    if (!tc_p_strcpy_s || !tc_p_strcat_s || !tc_p_strncpy_s || !tc_p_memcpy_s) return 4;
    TC_RUN(strcpy_s_src_inside_dest); TC_RUN(strcat_s_src_inside_dest); TC_RUN(strncpy_s_overlap); TC_RUN(memcpy_s_overlap_forward); return tc_end(); }
