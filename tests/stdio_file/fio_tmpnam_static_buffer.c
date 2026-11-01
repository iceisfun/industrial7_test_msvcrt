/* TC
 * id:    fio_tmpnam_static_buffer
 * area:  stdio_file
 * funcs: tmpnam
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs tmpnam; C11 7.21.4.4
 * note:  TRIAGE win10, class A): ucrtbase 10.0.19045 tmpnam(NULL) returns the same static buffer holding GetTempPath()+"sNNN.N" (43 chars, observed in a scratch
 *        probe: ...\Temp\s690.1 then ...\s690.2, same pointer), which is longer than the toolchain's L_tmpnam (14). The bound is therefore _MAX_PATH (260) on UCRT; the static-buffer
 *        identity and name-change assertions are unchanged. Build dependence: newer Win11 ucrtbase builds use GetTempPath2W (SystemTemp when running as SYSTEM); only 19045 observed. [unverified] that the real UCRT stdio.h defines L_tmpnam as 260.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static char * volatile nullbuf = NULL;
/* ucrtbase tmpnam(NULL) returns an absolute temp-dir path (~43 chars); the toolchain's L_tmpnam is only 14, but the UCRT static buffer is _MAX_PATH-sized. */
#if TC_AT_LEAST(140)
#define TC_LTMPNAM 260
#else
#define TC_LTMPNAM L_tmpnam
#endif

TC_CASE(static_buffer)
{
    char *a, *b; char first[300];
    a = tmpnam(nullbuf);
    CHECK(a != NULL);
    if (!a) return;
    CHECK(strlen(a) < TC_LTMPNAM);
    CHECK(strlen(a) > 0);
    if (strlen(a) >= sizeof first) { CHECK(0); return; }
    strcpy(first, a);
    b = tmpnam(nullbuf);
    CHECK(b != NULL);
    CHECK_EQ_P(a, b);
    if (b) {
        CHECK(!tc_strequ(first, b));  /* the shared buffer now holds a new name */
        CHECK(strlen(b) < TC_LTMPNAM);
    }
}
int main(void) { tc_begin(); TC_RUN(static_buffer); return tc_end(); }
