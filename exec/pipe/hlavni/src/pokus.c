//#include <windows.h>
#include <stdio.h>

//HANDLE hSaveStdout, hChildStdoutRd, hChildStdoutWr, hChildStdoutRdDup;

int main(void) {
/*
	PROCESS_INFORMATION pi; 
	STARTUPINFO si; 
	BOOL b;
	SECURITY_ATTRIBUTES saAttr; 
	char str[1024];
   DWORD precteno;
//   BOOL fSuccess; 
 
// Set the bInheritHandle flag so pipe handles are inherited. 
 
   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
   saAttr.bInheritHandle = TRUE; 
   saAttr.lpSecurityDescriptor = NULL; 


     ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	puts("spoustec");
 
 

   // The steps for redirecting child process's STDOUT: 
   //     1. Save current STDOUT, to be restored later. 
	hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE); 

   //     2. Create anonymous pipe to be STDOUT for child process. 

	if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) return 1;
	
       
   //     3. Set STDOUT of the parent process to be write handle to 
   //        the pipe, so it is inherited by the child process. 

	 if (! SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr)) return 2;

   //     4. Create a noninheritable duplicate of the read handle and
   //        close the inheritable read handle. 

	 DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
        GetCurrentProcess(), &hChildStdoutRdDup , 0,
        FALSE,
        DUPLICATE_SAME_ACCESS);
    
    CloseHandle(hChildStdoutRd);



 
   b = CreateProcess( NULL, // No module name (use command line). 
        "C:\\honza\\c\\pipy\\client\\Debug\\client.exe", // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi );             // Pointer to PROCESS_INFORMATION structure.


   if (! SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout)) return 2;

   
   b = ReadFile(hChildStdoutRdDup, (LPVOID) str, sizeof(str) - 1, &precteno, NULL); 

   printf("%i\n", b);
   if (b) {
	   str[precteno] = 0;
	   printf("'%s'\n", str);
   }

*/   
   return 0;
}
