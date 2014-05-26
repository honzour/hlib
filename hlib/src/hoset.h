#ifndef HoSetH
#define HoSetH
#define XW 0
#define MW 1
#ifdef unix
#define hplatform XW
#else
#define hplatform MW
#endif

#include <stdio.h>
// stdio i pro windows kvuli snprintf
#if hplatform==MW
#include <windows.h>
#endif

#define HoDebug

#ifdef HoDebug
/*void HoWarningPom(char *f);
void HoPrintPom(char *f);*/
#define HoWarning(f) HoWarningPom(f) 
#define HoPrint(f) HoPrintPom(f)
#if hplatform==XW
#define HoAssert(f) if(!(f)){\
 fprintf(stderr,"HoAssertion failed %s, file %s, line %d\n", \
#f, __FILE__, __LINE__);\
fflush(stderr);\
exit(1);\
}
#elif hplatform==MW
#define HoAssert(f) if(!(f)){\
char HoAsserts[256];\
 sprintf(HoAsserts,"HoAssertion failed %s, file %s, line %d", \
#f, __FILE__, __LINE__);\
MessageBeep(-1);\
MessageBox(NULL,HoAsserts,"HoAssert",MB_OK);\
exit(1);\
}
#endif
#else
#define HoAssert(f)
#define HoWarning(f)
#define HoPrint(f)
#endif
#endif

