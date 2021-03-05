/* core.h implementation
 */
#include "../include/core.h"
#include <stdio.h>
#include <stdlib.h>

/* Shows the help page and exits the whole program.
 */
void core_help()
{
    printf("%s\n\n%s\n%s\n", "Usage: svm [OPTIONS]... FILE",
    "\tFILE        compiled Salt code (scc) file",
    "\t-h, --help  show this page and exit");
    exit(1);
}