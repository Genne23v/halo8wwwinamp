#ifndef __IPC_PE_H 
#define __IPC_PE_H 
 
#define IPC_PE_GETCURINDEX        100 
#define IPC_PE_GETINDEXTOTAL      101 
#define IPC_PE_GETINDEXINFO       102 
#define IPC_PE_GETINDEXINFORESULT 103 
#define IPC_PE_DELETEINDEX        104 
#define IPC_PE_SWAPINDEX          105 
#define IPC_PE_INSERTFILENAME     106 
#define IPC_PE_GETDIRTY           107 
#define IPC_PE_SETCLEAN           108 
#define ID_PE_S_RANDOM                  40212  /* playlist randomize */
 
typedef struct { 
     char file[MAX_PATH]; 
     int index; 
     } fileinfo; 
 
typedef struct { 
     HWND callback; 
     int index; 
     } callbackinfo; 
 
#endif
