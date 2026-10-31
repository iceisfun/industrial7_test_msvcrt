/* TC
 * id:    loc_neg_mbrtoc32_invalid_utf8
 * area:  locale
 * funcs: mbrtoc32 mbrtoc16 setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.28.1.1/7.28.1.2 (encoding error: (size_t)-1, errno = EILSEQ)
 * note:  Skipped when setlocale(LC_ALL, ".UTF-8") fails. Each call uses a fresh zeroed state.
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

static int utf8(void) { volatile const char *n = ".UTF-8"; return setlocale(LC_ALL, (const char *)n) != NULL; }
static size_t m32(const char *s, size_t n, int *e) { mbstate_t st; char32_t c = 0; size_t r; memset(&st, 0, sizeof st); errno = 0; r = mbrtoc32(&c, s, n, &st); *e = errno; return r; }
static size_t m16(const char *s, size_t n, int *e) { mbstate_t st; char16_t c = 0; size_t r; memset(&st, 0, sizeof st); errno = 0; r = mbrtoc16(&c, s, n, &st); *e = errno; return r; }
#define SKIPNO() do { if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); } } while (0)

TC_CASE(ff_minus1) { int e; static const char s[] = "\xFF"; SKIPNO(); CHECK_EQ_U(m32(s, 1, &e), (size_t)-1); setlocale(LC_ALL, "C"); }
TC_CASE(stray_continuation_minus1) { int e; static const char s[] = "\x80"; SKIPNO(); CHECK_EQ_U(m32(s, 1, &e), (size_t)-1); setlocale(LC_ALL, "C"); }
TC_CASE(bad_continuation_minus1) { int e; static const char s[] = "\xC3\x28"; SKIPNO(); CHECK_EQ_U(m32(s, 2, &e), (size_t)-1); setlocale(LC_ALL, "C"); }
TC_CASE(errno_eilseq) { int e; static const char s[] = "\xFF"; SKIPNO(); m32(s, 1, &e); CHECK_EQ_I(e, EILSEQ); m32("\xC3\x28", 2, &e); CHECK_EQ_I(e, EILSEQ); setlocale(LC_ALL, "C"); }
TC_CASE(recovers_with_fresh_state) { int e; mbstate_t st; char32_t c = 0; SKIPNO();
    m32("\xFF", 1, &e); memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, "A", 1, &st), 1); CHECK_EQ_U(c, U'A'); setlocale(LC_ALL, "C"); }
TC_CASE(mbrtoc16_same_inputs) { int e; SKIPNO();
    CHECK_EQ_U(m16("\xFF", 1, &e), (size_t)-1); CHECK_EQ_I(e, EILSEQ);
    CHECK_EQ_U(m16("\x80", 1, &e), (size_t)-1); CHECK_EQ_I(e, EILSEQ);
    CHECK_EQ_U(m16("\xC3\x28", 2, &e), (size_t)-1); CHECK_EQ_I(e, EILSEQ); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(ff_minus1); TC_RUN(stray_continuation_minus1); TC_RUN(bad_continuation_minus1); TC_RUN(errno_eilseq); TC_RUN(recovers_with_fresh_state); TC_RUN(mbrtoc16_same_inputs); return tc_end(); }
