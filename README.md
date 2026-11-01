# msvcrt / ucrtbase test corpus

About 6,000 small C tests for the Microsoft C runtimes (ucrtbase, msvcr120/110/100/90/80, msvcrt.dll), built with
mingw-w64 and meant to be run on **real Windows**. Wine is only a smoke runner here: a test failing on Wine is not wrong
for that reason alone. The test format and the source-of-truth rules are in [SCHEMA.md](SCHEMA.md).

```
python3 tools/tc.py lint
python3 tools/tc.py build [FILTER] [--crt ucrtbase]
python3 tools/tc.py run   [FILTER]            # Wine smoke run; needs WINEPREFIX (use ./.pfx) and the caps below
python3 tools/tc.py ingest DIR                # summarise results copied back from a real Windows run
```

`tools/run_on_windows.ps1` runs the built tree on Windows. `build/`, `.pfx/` and every Wine prefix file are generated and
git-ignored; rebuild with `tc.py build`.

## Memory hazards: read this before running anything

An early unattended run of this corpus under Wine exhausted the host's 128 GB and took the machine (and its X session)
down. Huge-allocation negative tests are the main suspects: Windows fails a 64 TB request instantly, but Wine stalls on
it, and nothing bounded the damage. Two guards now exist. Use both.

1. **Per-test RAM cap in `tools/tc.py`.** Each test runs in its own `systemd-run --user --scope` with
   `MemoryMax=${TC_MEMMAX:-4G}` and `MemorySwapMax=0`. A runaway test is OOM-killed and reported as `CRASH`.
   On timeout (`TC_TIMEOUT`, default 60 s) the whole scope is SIGKILLed, so Wine children cannot linger.
   `TC_MEMMAX=0` disables the cap (do not, on a machine you care about). This needs a running `systemd --user`.
2. **Hazard cases are compiled out by default.** A case that starts with `TC_HAZARD("reason")` is skipped
   (`TC SKIP ... hazard, build with TC_HAZARD=1`) unless the corpus is built with `TC_HAZARD=1 python3 tools/tc.py build`.
   Build with it on real Windows, where those requests fail fast. Current hazard cases (all a 1<<46 = 64 TB
   `_aligned_malloc`):
   - `tests/heap/hp_neg_aligned_malloc_huge.c` (`one_shl_46`; a `1<<42` case now runs by default)
   - `tests/ucrt_only/ucr_neg_aligned_malloc_huge.c` (`size_16tib`)
   - `tests/ucrt_only/ucr_neg_aligned_malloc_huge_errno_probe.c` (`errno_16tib`, `iph_count`)

**Never add `MemoryHigh`.** With swap off it throttles a process instead of killing it. That stall shows up as memory
pressure on `user@UID.service`, and `systemd-oomd` (50 % pressure for 20 s) then kills the whole user manager, which
takes down dbus and the desktop session. This happened once during testing. Use `MemoryMax` alone.

When you add a test that requests a very large size, run it under the cap first, and wrap any case that stalls or
allocates in `TC_HAZARD(...)`.

## Status (honest)

- 5,985 tests, lint clean (`tools/tc.py lint`). ucrtbase build: 5,605 of 5,605 tests build (`tools/tc.py build --crt ucrtbase`);
  hazard cases stay compiled out (`TC SKIP ... hazard`). The other per-CRT targets (msvcrt, msvcr80..120) were not rebuilt or
  run on Windows in this pass; their `#else` / `TC_CRT_VER < 140` expectations are unverified.
- **Real Windows 10 22H2 x64 (ucrtbase), first contact93[34])?#933, corpus 9b9a139):** 5,604 built, PASS 4,811 (85.8 %),
  FAIL 763, CRASH 28, TIMEOUT 2; 2 build defects (`tm_neg_asctime_s_fields`, `ucr_neg_aligned_malloc_huge`).
- **After the Win10 triage93[34])?#934, branch the Win10 triage branch, payload built from commit abf620e09):** every one of the
  793 non-pass rows plus the 2 build defects is classified in `results/triage-win10.tsv` (one row each; per-shard ledgers in
  `results/triage-win10.d/`). Full reference run: **PASS 5,605 of 5,605 built tests (100 %)**, no FAIL, CRASH or TIMEOUT; repeated
  from a clean overlay with identical per-test results (runs `triage-run-1` and `triage-run-2`).
  Ledger classes: A test bug 438, F harness/environment 199, B UCRT deviates from ISO C 67, D export-name probe 50,
  C parameter validation that crashes on Windows 34, G unknowable without more data (demoted to probe, kept out of the bar) 8,
  E hang bounded 4, build defect 2. The 8 class G tests run and pass (they record or skip the undeterminable datum); excluding
  them the bar is 5,597 of 5,597.
- Harness facts found on the way: the mingw-w64 startup code installs a no-op invalid-parameter handler (children that assert the
  CRT default handler call `tc_iph_default()`); `tools/tc.py` now defines `TC_UCRT` for ucrtbase as this README always said
  (before, every `#ifdef TC_UCRT` branch was dead; 6 such branches were wrong and are fixed in the ledger as
  `PASS(integ-regress)`).
- Reference VM pins (VM pins): Windows 10 Home 22H2 build 19045.2965 (buildlab 19041.1.amd64fre.vb_release.191206-1406),
  ntdll 10.0.19041.2788, ntoskrnl 10.0.19041.2965, kernelbase 10.0.19041.2788; base image sha256
  a1f7e36d2c93ce0fe5b5099e3748a9ffaf4fffcf39b7d3ba386b0eced23e18e9 (ISO sha256
  a6f470ca6d331eb353b815c043e327a347f594f37ff525f17764738fe812852e); virtualised
  4 GB / 4 vCPU, guest clock pinned to 2026-09-30T12:00:00. Results depend on these pins: CPU-dispatch,
  build/UBR and the pinned clock/time zone are flagged in the affected test notes.
- Many tests remain `probe` or `inferred` where only the ucrtbase branch was observed; Windows results stay the source of truth:
  fix tests, not the reverse.
- Wine smoke runs show many failures that need triage into test bugs and genuine Wine differences. Wine does not
  implement the `_aligned_*recalloc` family, so those tests CRASH there by design. Older results files under `results/` are
  from interrupted runs and are not a baseline.
- Function coverage: 865 of 1,213 exported functions; 348 are uncovered (gap fill was cut short by an agent cap).
