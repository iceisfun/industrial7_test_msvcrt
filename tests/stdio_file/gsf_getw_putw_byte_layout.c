/* TC
 * id:    gsf_getw_putw_byte_layout
 * area:  stdio_file
 * funcs: _putw _getw fopen fread fwrite fclose
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _getw, _putw (binary int-sized word; _getw returns EOF at end, use feof/ferror to tell)
 * note:  Little-endian layout holds on all Windows targets.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

#define S2(x) #x
#define S1(x) S2(x)
static const char *fn = "tc_gsf_getw_putw_byte_layout_" S1(TC_CRT_VER) ".tmp";
static volatile int vw;

TC_CASE(putw_bytes)
{
    FILE *f = fopen(fn, "wb"); unsigned char b[8]; size_t n;
    CHECK(f != NULL); if (!f) return;
    vw = 0x12345678;
    CHECK_EQ_I(_putw(vw, f), 0x12345678);
    fclose(f);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    n = fread(b, 1, sizeof b, f);
    CHECK_EQ_U(n, 4);
    CHECK_EQ_U(b[0], 0x78); CHECK_EQ_U(b[1], 0x56); CHECK_EQ_U(b[2], 0x34); CHECK_EQ_U(b[3], 0x12);
    fclose(f); remove(fn);
}
TC_CASE(putw_return_value)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    vw = 7;       CHECK_EQ_I(_putw(vw, f), 7);
    vw = INT_MIN; CHECK_EQ_I(_putw(vw, f), INT_MIN);
    vw = 0;       CHECK_EQ_I(_putw(vw, f), 0);
    fclose(f); remove(fn);
}
TC_CASE(getw_roundtrip_negative_and_max)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    vw = INT_MIN; _putw(vw, f); vw = INT_MAX; _putw(vw, f); vw = 0; _putw(vw, f); vw = -1; _putw(vw, f);
    fclose(f);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), INT_MIN);
    CHECK_EQ_I(_getw(f), INT_MAX);
    CHECK_EQ_I(_getw(f), 0);
    CHECK_EQ_I(_getw(f), -1);
    CHECK(feof(f) == 0);
    fclose(f); remove(fn);
}
TC_CASE(multiple_words_sequence)
{
    FILE *f = fopen(fn, "wb"); unsigned char b[16];
    CHECK(f != NULL); if (!f) return;
    vw = 1; _putw(vw, f); vw = 0x01020304; _putw(vw, f); vw = 3; _putw(vw, f);
    fclose(f);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), 1); CHECK_EQ_I(_getw(f), 0x01020304); CHECK_EQ_I(_getw(f), 3);
    rewind(f);
    CHECK_EQ_U(fread(b, 1, sizeof b, f), 12);
    CHECK_EQ_U(b[4], 4); CHECK_EQ_U(b[7], 1);
    fclose(f); remove(fn);
}
TC_CASE(getw_at_eof_sets_feof)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    vw = 5; _putw(vw, f);
    fclose(f);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), 5);
    CHECK(feof(f) == 0);
    CHECK_EQ_I(_getw(f), EOF);
    CHECK(feof(f) != 0);
    fclose(f); remove(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(putw_bytes); TC_RUN(putw_return_value); TC_RUN(getw_roundtrip_negative_and_max);
    TC_RUN(multiple_words_sequence); TC_RUN(getw_at_eof_sets_feof);
    return tc_end();
}
