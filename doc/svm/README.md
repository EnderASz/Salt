# SVM Documentation, for 0.13

The Salt Virtual Machine is a real-time interpreter for compiled Salt code. Each SCC file
is made out of the header and the instructions. Detailed information how such SCC file
is built and what instructions are avaible, you should see the doc/scc page. That part
is not included in the svm subdirectory, because its used for writing your own Salt compiler,
which often has nothing to do with SVM itself.

## Index

* [Command line arguments](pages/arguments.md)

## Info

Field                           | Value
--------------------------------|-----------------
Version                         | 0.13
Default supported SCC format    | SCC3
Max registers                   | 255
Dependencies                    | `libc`, `scc.h`
Tested compilers                | `gcc`, `clang`


## Ports

System	                  | Works?
--------------------------|------------
Linux	                  | Yes
Win32	                  | Yes
Win64	                  | Yes
SerenityOS	              | Almost, needs more work
