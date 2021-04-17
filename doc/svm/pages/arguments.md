# Command line arguments

The SVM binary comes with some command line arguments you can pass to change
the functionality.

## Usage

<pre>
svm [OPTION].. FILE
</pre>

## Arguments

<pre>
FILE                      the compiled salt executable
-h, --help                show this and exit
-v, --version             show the version and exit
-m, --mem-used            show the amount of memory used at the end
-d, --allow-debug         allow debug output from MLMAP & TRACE
-l, --limit-mem [KB]      limit the memory usage to a certain amount of kilobytes
-D, --disassemble         disassemble the passed file
</pre>

This will be updated as SVM gets more arguments. If a new version of SCC ever
gets created, SVM will support older versions of SCC for a certain period of time
before they get deprecated. This will allow you to use the newest stable version
of SVM without the need of downgrading to a more buggy version.

## Example

Execute Program.scc, while limiting it to 16 KB of memory, also allowing debug output.

```bash
$ svm --limit-mem 16 -d Program.scc
```
