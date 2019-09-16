Consider putting additional code here. My implementation has the following
files, though you're free to things however you want"

  - `src/utils/flags.{h,cpp}` - Code for defining and parsing command-line
     flags.

  - `src/utils/sort_functions.{h,cpp}` - Code defining comparator functions
    for ordering processes by PID, memory usage, CPU usage, and time running.

  - `src/utils/statistics.{h,cpp}` - Code related to performing calculations
    like determining CPU utilization (there aren't too many calculations to
    do).

  - `src/utils/formatting.{h,cpp}` - Code to help with formatting numbers in
    readable formats, e.g. human-readably bytes (B, KiB, MiB, GiB, etc) and
    time strings (seconds, minutes, hours, days, etc).

Any test classes that you add under this directory will be automatically
included in the `make test` target. I want to encourage you to add unit tests of
your own. They're really helpful, =)
