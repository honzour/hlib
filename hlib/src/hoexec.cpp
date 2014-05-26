#include "hoset.h"
#include "hoexec.h"
#if hplatform==XW
#include <unistd.h>
#include <fcntl.h>
#endif

bool hoExec(const char *path, char *const argv[]) {
#if hplatform==XW
  int pp[2]; // pipa na synchronizaci
  pipe(pp);
  fcntl(pp[1], F_SETFD, FD_CLOEXEC); //  pipa se zavre pri exec

  pid_t r = fork();
  if (r < 0) return false; // neprosel ani fork()
  char c;
  if (!r) {
    close(pp[1]);
    int precteno = read(pp[0], (void *)&c, 1);
    close(pp[0]);
    return !precteno; // kdyz neco z pipy prectu, znamena to, ze syn tam
    // neco poslal a to znamena, ze neprosel exec
  }
  close(pp[0]);
  execvp(path, argv);
  
  write(pp[1], (void *)&c, 1);
  close(pp[1]);
  _exit(0);
  return false;
  
#else
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  memset(&si, 0, sizeof(si));
  return CreateProcess(path, "",  NULL, NULL, false, 0, NULL, NULL, &si, &pi) ? true : false;
#endif
}


HoPipe * hoExecWithPipe(const char *path, char *const argv[]) {
/*
  SECURITY_ATTRIBUTES saAttr; 
  HANDLE hStdout, hChildStdoutRd, hChildStdoutWr, hChildStdinRd, hChildStdinWr;
 
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
  saAttr.bInheritHandle = TRUE; 
  saAttr.lpSecurityDescriptor = NULL; 

  hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
 
  if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) return NULL; 

   SetHandleInformation( hChildStdoutRd, HANDLE_FLAG_INHERIT, 0);

   if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) 
      return NULL; 

   SetHandleInformation( hChildStdinWr, HANDLE_FLAG_INHERIT, 0);

   PROCESS_INFORMATION piProcInfo; 
   STARTUPINFO siStartInfo;
   BOOL bFuncRetn = FALSE; 
 
// Set up members of the PROCESS_INFORMATION structure. 
 
   ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
// Set up members of the STARTUPINFO structure. 
 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   siStartInfo.hStdError = hChildStdoutWr;
   siStartInfo.hStdOutput = hChildStdoutWr;
   siStartInfo.hStdInput = hChildStdinRd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
 
// Create the child process. 
    
   bFuncRetn = CreateProcess(NULL, (char *) path, NULL, NULL, TRUE, DETACHED_PROCESS, NULL, NULL, &siStartInfo, &piProcInfo);
   
   if (bFuncRetn == 0) 
      return NULL;
   else 
   {
      CloseHandle(piProcInfo.hProcess);
      CloseHandle(piProcInfo.hThread);
	  return new HoPipe(hChildStdoutRd, hChildStdinWr);
   }
  */
return NULL; 
}
