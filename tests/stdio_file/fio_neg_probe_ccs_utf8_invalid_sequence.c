/* TC
 * id:    fio_neg_probe_ccs_utf8_invalid_sequence
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fgetwc fclose ferror feof
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen (ccs=UTF-8), fgetwc
 * note:  Probes how a ccs=UTF-8 text stream reacts to malformed UTF-8: (a) 41 C3 28 -> L'A' then WEOF (+EILSEQ),
 *        (b) lone 0x80 continuation -> WEOF (+EILSEQ), (c) 3-byte sequence truncated by EOF (E2 82) -> WEOF with
 *        eof or error state set. errno for (c) is deliberately not asserted.
 *        OBSERVED (Win10 22H2, class A): 41 C3 28 reads L'A', U+FFFD, L'(', WEOF with errno 0 (raw: c2 == WEOF; errno == EILSEQ got=0). Lone 0x80 -> WEOF/EILSEQ and truncated E2 82 -> WEOF/ferror (EINVAL) unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define NAME "tc_fio_u8bad_" TC_S1(TC_CRT_VER) ".tmp"

static void put(const char *n, const void *d, DWORD len)
{
    DWORD w; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, len, &w, NULL); CloseHandle(h); }
}

TC_CASE(invalid_continuation)
{
    static const unsigned char b[] = { 0x41, 0xC3, 0x28 };
    FILE *f; wint_t c1, c2;
    put(NAME, b, 3);
    f = fopen(NAME, "r, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    c1 = fgetwc(f); errno = 0; c2 = fgetwc(f);
    CHECK_EQ_I(c1, L'A');
    CHECK_EQ_U(c2, 0xFFFD);   /* observed: bad lead C3 followed by 28 -> U+FFFD, no errno, no WEOF */
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(fgetwc(f), L'(');   /* the 0x28 is not swallowed */
    CHECK(fgetwc(f) == WEOF);
    fclose(f);
}
TC_CASE(lone_continuation)
{
    static const unsigned char b[] = { 0x80 };
    FILE *f; wint_t c;
    put(NAME, b, 1);
    f = fopen(NAME, "r, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    errno = 0; c = fgetwc(f);
    CHECK(c == WEOF);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f);
}
TC_CASE(truncated_at_eof)
{
    static const unsigned char b[] = { 0xE2, 0x82 };
    FILE *f; wint_t c;
    put(NAME, b, 2);
    f = fopen(NAME, "r, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f);
    CHECK(c == WEOF);
    CHECK(feof(f) != 0 || ferror(f) != 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(invalid_continuation); TC_RUN(lone_continuation); TC_RUN(truncated_at_eof);
    DeleteFileA(NAME);
    return tc_end();
}
