/**
 * The precompiler.h header file includes all functions responsible for
 * preparing Salt code to compilation.
 * 
 * @author EnderASz
 */
#ifndef CORE_PRECOMPILER_H_
#define CORE_PRECOMPILER_H_

#include <string>
using std::string;

namespace salt::precompiler
{

    /**
     * Paste init.salt from standard library to the code given by reference.
     * This function modifies code "in-place".
     */
    void init_paste(string& code);

    /**
     * Clean comments from code given by reference.
     * This function modifies code "in-place".
     */
    void clean_comments(string& code);
    
} // salt::core::precompiler

#endif // CORE_PRECOMPILER_H_