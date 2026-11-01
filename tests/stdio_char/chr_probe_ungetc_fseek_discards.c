/* TC
 * id:    chr_probe_ungetc_fseek_discards
 * area:  stdio_char
 * funcs: ungetc fgetc fseek
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.21.9.2p5 (a successful fseek discards pushed-back characters), 7.21.9.5 (rewind)
 * note:  Probes whether fseek(0, SEEK_CUR) / rewind after ungetc(different char) discard the pushed char and re-read the file byte.
 *        The standard says yes; the CRT writes the pushed char into its buffer and may satisfy a within-buffer seek without
 *        re-reading the file, so real Windows may return the pushed char (or a clobbered byte). Unknown: this is the question.
 *        TRIAGE win1093[34])?#934, class B): deviates from C11 7.21.9.2p5: on ucrtbase 10.0.19045 fseek(f,0,SEEK_CUR) after ungetc('Z') does not discard the
 *        pushed-back char: the next fgetc returns 'Z' (got=90) and then 'd' (ungetc overwrote the buffered byte; the seek stays inside the buffer). rewind() does discard.
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_probe_ungetc_fseek_discards_a" TCV_S(TC_CRT_VER) ".tmp"
static FILE *mk(void)
{
    FILE *f = fopen(FN, "wb"); if (f) { fwrite("abcdef", 1, 6, f); fclose(f); }
    return fopen(FN, "rb");
}

TC_CASE(fseek_discards)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), 'c');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    CHECK_EQ_I(fseek(f, 0, SEEK_CUR), 0);
#if TC_AT_LEAST(140)
    /* deviates from C11 7.21.9.2p5: observed ucrtbase 10.0.19045: the in-buffer seek keeps the pushed-back byte */
    CHECK_EQ_I(fgetc(f), 'Z');
#else
    CHECK_EQ_I(fgetc(f), 'c');     /* position 2, pushed 'Z' discarded */
#endif
    CHECK_EQ_I(fgetc(f), 'd');
    fclose(f); remove(FN);
}
TC_CASE(rewind_discards)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    rewind(f);
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(fgetc(f), 'b');
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(fseek_discards); TC_RUN(rewind_discards); return tc_end(); }
