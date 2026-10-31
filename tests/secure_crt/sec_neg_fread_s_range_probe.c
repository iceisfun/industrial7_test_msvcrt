/* TC
 * id:    sec_neg_fread_s_range_probe
 * area:  secure_crt
 * funcs: fread_s fopen_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   fread_s buffer-too-small path
 * note:  PROBE: fread_s(buf, 4, 1, 8, f) returns 0 with errno == ERANGE (buffer smaller than size*count). Believed from CRT behaviour; not
 *        clearly in the MS docs error table. Handler invocation not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_sec_neg_fread" TC_S1(TC_CRT_VER) ".tmp"
static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static FILE *mk(void) { FILE *f = NULL; fopen_s(&f, FN, "wb+"); if (f) { fwrite("0123456789abcdef", 1, 16, f); rewind(f); } return f; }
TC_CASE(bufsize_lt_size_times_count_ERANGE_returns_0) { char b[4]; FILE *f = mk(); ini(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread_s(b, 4, 1, 8, f), 0); CHECK_EQ_I(errno, ERANGE); fclose(f); remove(FN); }
int main(void) { tc_begin(); TC_RUN(bufsize_lt_size_times_count_ERANGE_returns_0); return tc_end(); }
