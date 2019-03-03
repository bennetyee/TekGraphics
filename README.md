# Tektronix Graphics

This repository contains some old code

- `fgplot.c`: converts from Tek "Fast Graphics" format data to Tek
  401x terminal graphics escape sequences.

- `sleepycat.c`: displays data slowly.  I think the default slowdown
  approximates the rate at which an 4010 terminal could draw.  A 4014
  was noticeably faster, but as far as I know no timing data exists.

- `FastGraphicsData` contains some old vector graphics images that
  were available within Tektronix (circa 1980).

To build, just run `make`.

To view the graphics, run an `xterm`; `gnome-terminal` or other
terminal emulators have dropped Tektronix graphics emulation.  Within
the `xterm`, just run `fgplot` with input redirected, e.g., `fgplot <
FastGraphicsData/file.11`.
