/**
 * The os module provides cross platform compatibilty between Linux and 
 * Windows, like the sleep() function.
 *
 * @author bellrise
 */
#ifndef OS_H_
#define OS_H_

#ifndef SVM_VERSION
  #define SVM_VERSION "undefined"
#endif

/* Include different headers for Windows & Linux. */
#if defined(_WIN32)
  #include <Windows.h>
  
  /* Sleep for n miliseconds */
  #define os_sleep(ms) Sleep(ms)

#elif defined(__linux__)
  #include <unistd.h>
  #define OS linux

  /* Sleep for n miliseconds */
  #define os_sleep(ms) usleep((double) ms * 1000.f)

#endif

#endif // OS_H_