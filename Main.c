/*	======================================================================================================================
	Copyright (C) 1998-2001 Nullsoft, Inc.
	
	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.
	
	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	
	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
	======================================================================================================================

	
	C H A N G E   L O G
	
	2.5.d  - ??

	2.6.02 - 2002/06/14	(rar) Lots o stuff has changed.   Includes support for *.css file., change some of the way
						(rar) this app logs stuff.  added some global variables to help reduce redundant code.  It'll get there.	
	2.6.05 - 2002/06/16 (rar) Added support for *.js file.  Incorporated use of the WebDings font for control symbols.
	2.6.10 - 2002/06/17 (rar) removed [p] button which clears the playlist and adds the selected song as the sole song to the liast
						(rar) added [c] in [p]'s place, which allows you to cue a song to play direct after the song that is currently playing.
	2.6.11 - 2002/06/19 (rar) Added Support for rebots.txt file, which basically disallows indexing of anything from this 
	                          "server".  Content resides within this program.
						(rar) Modified Cache expire time (from 1 hour to 1 year)
	2.6.12 - 2002/07/07 (rar) Added \r's to ROBOTS.TXT output string.
	         2002/11/27 (rar) Added Self Installer using NSIS.
	2.6.13 - 2002/11/27 (m?m) Added end-of-playlist actions
						(m?m) Added cover art options
	2.7.1  - 2002/11/29 (rar) Merge Michael Michon's changes into main.
	2.7.2  - 2003/01/11 (rar) added doRecursiveAddDB prototype to list of function prototypes
                        (rar) printf statement to clear the line in the console when MakeDB is run
                        (rar) removed call to link *.js file since functions are only for windows CE and are commented out at 
						      the moment.  For some reason, the functions within were causing javascript errors
    2.7.3  - 2003/01/12 (rar) added abiltiy to rescan files
						(rar) modified the layout to include controls in middle frame
						(rar) added 'listen' feature, which allows user to listen to the shoutcast stream directly from wwwinamp, and not have to visit the shoutcast server
						(rar) sprinkled a few comments here and there
                        (rar) change font size to use %, not px
						(rar) updated copyright statement

						
*/

#define SERV_VER 			"2.7.3 (Built: " __DATE__ " " __TIME__ ")"
#define SERV_NAME			"WWWinamp"
#define SERV_NAME_LONG 		"WWWinamp Remote Control Server"
#define COPYRIGHT 			"(C) Copyright 1998-2001 Nullsoft, Inc. All Rights Reserved."
#define BRANDING 			"Halo 8 Productions"
#define BRANDING_URL 		"http://projects.halo8.net/"
#define BRANDING_COPYRIGHT	"(C) Copyright 2002-2003 Halo 8 Productions, Inc. All Rights Reserved."

#define ADMIN_URL			"/mp3j"
#define STYLE_URL			"/wwwinamp.css"
#define JS_URL				"/wwwinamp.js"
#define ROBOTS_URL			"/robots.txt"
#define CONTROL_URL			"/controls.gif"
#define COVER_URL			"folder.jpg"

/*	======================================================================================================================
	INCLUDES 
*/

#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>     // allows passing an unknown number parameters
#include <signal.h>
#include <process.h>
#include <time.h>

#include "frontend.h"
#include "ipc_pe.h"

#ifndef min
#define min(x,y) (( x ) < ( y ) ? ( x ) : ( y ))
#define max(x,y) (( y ) < ( x ) ? ( x ) : ( y ))
#endif

/*	======================================================================================================================
	TYPES
*/

typedef struct {
	struct sockaddr_in sin;
	int msgsock,s;
	} cnx_inf;

typedef struct {
	char *leading_path;
	char file[MAX_PATH];
	} dbType;

/*	======================================================================================================================
	FUNCTION PROTOTYPES
*/

int		main(int argc, char **argv);
int		LoadConfig(void);
int		getip(int which, struct sockaddr_in *sin);
int		init_socketlib(int which);
int		recv_string(int s, char *str, int maxlen);
int		OpenSocket(short port, int mc);
int		WaitForConnection(int sock,struct sockaddr_in *sin);
int		sock_printf(int sock, char *fmt, ...);
int		sock_send(int sock, char *buffer,int size);
int		myrand(void);
void	mysrand (unsigned int seed);
char	*getTimeStr(long alt_time);
char	*gethost(struct sockaddr_in *sin);
char	*extension(char *fn);
void	encodeLP(char *out, char *in);
void	quit();
void	makeDB();
void	doRecursiveAddDB(char *leading_path, char *this_path);
void	fixstring(char *in, char *out);
void	unfixstring(char *in, char *out, int maxlen);
void	log_printf(char *format, ...);
void	launchthread(void *threadproc, void *data);
void	setnonblock(int msgsock, int i);
void	CloseSocket(int sock);
void	http_handlereq(char *url, char *user_agent, char *encodedlp, int sock, struct sockaddr_in *sin);
void 	http_sendredirect(int sock, char *host, char *fmt, ...);
void	quitthread();
void	showCoverArt(int sock,char* fn,char* leading_path);
unsigned int WINAPI HandleConnection(void *p);

/*	======================================================================================================================
	GLOBAL VARIABLES
*/

CRITICAL_SECTION log_mutex, playlist_mutex;
dbType *database;
short	g_dst_port				= 80;

int		database_used;
int		database_size;
int		g_perform_lookups		= 0;
int		g_show_requests			= 0;
int		g_log					= 1;
int		g_left_refresh			= 60;				// left panel refresh rate
int		g_playing_standby		= 0;
int		g_show_cover_art		= 0;
int		g_rand_next = 1;

char	g_db_path[1024];
char	g_db_filelist[1024];
char	g_ext_types[1024];
char	g_sc_server[1024];							// shoucast server info, null/empty for none
char	g_working_exe[MAX_PATH];
char	g_working_dir[MAX_PATH];
char	g_working_full[MAX_PATH];
char	g_working_name[MAX_PATH];
char	g_log_file[MAX_PATH];
char	g_config_file[MAX_PATH];
char	g_winamp_dir[MAX_PATH];
char	g_filler_stream_url[256];
char	g_dst_ip[128]			= "";
char	g_acclp[512]			= "";
char	g_admlp[512]			= "";
char	g_ext_type_list[1024]	= "mp3";
char	g_cover_art_filename[256] = COVER_URL;

enum eop_modes{
	eop_mode_silence=0,
	eop_mode_random=1,
	eop_mode_stream=2
} g_eop_action=eop_mode_silence;


/*	======================================================================================================================
	FUNCTION IMPLEMENTATIONS
*/

int main(int argc, char **argv) {
	int MainSocket;
	char log_area[16]="[main]", *q;
	struct  sockaddr_in sin;
  
	mysrand( (unsigned)time( NULL ) );

	InitializeCriticalSection(&log_mutex);
	InitializeCriticalSection(&playlist_mutex);

	// nono...if you do q = path....then *q will equal path[0] then they added strlen so it would get to the end of the path
	// so basically it's "while (q >- beginning of string, check if pointer is "\"...if not decrement q.....
	GetModuleFileName( NULL, g_working_full, MAX_PATH );
	// strncpy(s, t, n) copy n chars from s to t
	strncpy( g_working_dir,  g_working_full, MAX_PATH );
	q = g_working_dir + strlen(g_working_dir);
	while (q >= g_working_dir && *q != '\\') q--; *++q=0;
	strncpy( g_working_exe,  g_working_full, MAX_PATH );
	q = g_working_exe + strlen(g_working_exe);
	while (q >= g_working_exe && *q != '\\') q--; strcpy( g_working_exe, ++q );
	strncpy( g_working_name, g_working_exe, MAX_PATH );
	q = g_working_name + strlen(g_working_name);
	while (q >= g_working_name && *q != '.') q--; *q=0;
	
	// Debug Statements
	// printf( " g_working_full: %s\n" " g_working_dir : %s\n" " g_working_exe : %s\n" " g_working_name: %s\n", g_working_full, g_working_dir, g_working_exe, g_working_name );
	
	printf("\n" "-- " SERV_NAME_LONG " " SERV_VER "\n" "-- " BRANDING_COPYRIGHT "\n"
		"-- " COPYRIGHT "\n" "-- Use \"%s filename.ini\" to specify an ini file.\n\n", g_working_exe);

	if (argc > 1) {																// we have a command line argument...
		if ( strstr(argv[1],"\\") ) strcpy(g_config_file,argv[1]);				// appears to be in a separate directory (\\ = \)
		else wsprintf(g_config_file, "%s%s", g_working_dir, argv[1]);			// looks like simple file name
		}
	else wsprintf(g_config_file,"%s%s.ini", g_working_dir, g_working_name );	// we don't have a command line argument... use default...

	if ( LoadConfig() ) quit();

	if (init_socketlib(1) < 0) quit();

	MainSocket = OpenSocket(g_dst_port,32);
	if (MainSocket < 0) {
		log_printf("%s error opening dest socket! FATAL ERROR!\n", log_area);
		quit();
		}
	else log_printf("%s opening socket\n", log_area);


	makeDB();

	log_printf("%s entering server loop\n", log_area);
	while (1) {
		int msgsock;
		cnx_inf *c= (cnx_inf*)calloc( 1, sizeof(cnx_inf) );
		msgsock = WaitForConnection( MainSocket, &sin ); 
		memcpy( &c->sin,&sin,sizeof( sin ) );
		c->msgsock = msgsock;
		c->s = MainSocket;
		launchthread( (void*)HandleConnection, (void *)c );
		}
	CloseSocket(MainSocket);
	init_socketlib(0);
	return 0;
	}

static int n_cnt;


static int in_list(char *list, char *v) {
	while (*list) {
		if (!stricmp(v,list)) return 1;
		list+=strlen(list)+1;
		}
	return 0;
	}

int WaitForConnection(int sock,struct sockaddr_in *sin) {
	int msgsock, length;
	length = sizeof(struct sockaddr_in);
 	msgsock = accept(sock, (struct sockaddr *)sin, &length);
	return msgsock;
	}

unsigned int WINAPI HandleConnection(void *p) {
	cnx_inf *c=(cnx_inf *)p;
	int sock=(int)c->msgsock;
	int i=0;
	char req[1024];
	char user_agent[1024]="";
	char encodedlp[1024]="";
	char buf[1024];
	
	setnonblock(sock,1);
	if (recv_string(sock,req,sizeof(req)) > 2) {
		while (recv_string(sock,buf,sizeof(buf)) > 2) {
			buf[sizeof(buf)-1]=0;
			if (!strnicmp(buf,"User-Agent:",strlen("User-Agent:"))) {
				char *p=buf+strlen("User-Agent:");
				while (*p == ' ') p++;
				strncpy(user_agent,p,sizeof(user_agent)-1);
				user_agent[sizeof(user_agent)-1]=0;
			    }
			if (!strnicmp(buf,"Authorization: Basic ",strlen("Authorization: Basic "))) {
				char *p=buf+strlen("Authorization: Basic ");
				while (*p == ' ') p++;
				strncpy(encodedlp,p,sizeof(encodedlp)-1);
				encodedlp[sizeof(encodedlp)-1]=0;
				}
			}
		if (!strnicmp(req,"GET ",4)) {
			char *p=req+4,*p2,*p3;
			p3=p2=p;
			while (*p2 != ' ' && *p2) p2++;
			if (p2 == p) {
				p="/";
				}
			else {
				if (*p2) *p2=0;
				if (strstr(p,"//")) {
					p=strstr(p,"//");
					p+=3;
					while (*p != '/' && *p) p++;
					if (!*p) p="/";
					}
				}
			setnonblock(sock,0);
			http_handlereq(p, user_agent, encodedlp, sock, &c->sin);
			}
		}
	CloseSocket(sock);
	free(p);
	quitthread();
	return 0;
	}

static void parselist(char *out, char *in) {
	int nt=8, inquotes=0, neednull=0;
	char c;
	while (*in) {
		c=*in++;
		if (c >= 'A' && c <= 'Z') c+='a'-'A';
		if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
			neednull=1;
			*out++=c;
			}
		else if (c == '\"') {
			inquotes=!inquotes;
			if (!inquotes) {
				*out++=0;
				if (!nt--) break;
				neednull=0;
				}
			}
		else {
			if (inquotes) *out++=c;
			else if (neednull) {
				*out++=0;
				if (!nt--) break;
				neednull=0;
				}
			}
		}
	*out++=0;
	*out++=0;
	}

static int in_string(char *string, char *substring) {
	int len = strlen(substring)-2;
	if (len<0) return 1;
	while (*string) {
		if ((string[0]|32) == (substring[0]|32) && (string[1]|32) == (substring[1]|32)) {
			if (len < 1 || !strnicmp(string+2,substring+2,len)) return 1;
			}
		string++;
		}
	return 0;
	}

static int substr_search(char *bigtext, char *littletext_list) { 
	while (*littletext_list) {
		if (!in_string(bigtext,littletext_list)) return 0;
		while (*littletext_list) littletext_list++;
		littletext_list++;
		}
	return 1;  
	}

void fixstring(char *in, char *out) {
	while (*in) {
		if ((*in >= 'A' && *in <= 'Z')||
			(*in >= 'a' && *in <= 'z')||
			(*in >= '0' && *in <= '9')) *out++=*in++;
		else {
			int i=*in++;
			wsprintf(out,"%%%02X",i);
			out += 3;
			}
		}
	*out=0;
	}

void unfixstring(char *in, char *out, int maxlen) {
	while (*in && maxlen) {
		if (*in == '%' && in[1] != '%' && in[1]) {
			int a=0;
			int b=0;
			for ( b = 0; b < 2; b ++) {
				int r=in[1+b];
				if (r>='0'&&r<='9') r-='0';
				else if (r>='a'&&r<='z') r-='a'-10;
				else if (r>='A'&&r<='Z') r-='A'-10;
				else break;
				a*=16;
				a+=r;
				}
			if (b < 2) *out++=*in++;
			else { 
				*out++=a; 
				in += 3;
				}
			}
		else *out++=*in++;
		maxlen--;
		}
	*out=0;
	}

char *getTimeStr(long alt_time) {
	static char timeret[50];
	int alt_hour, alt_min, alt_sec;
	alt_hour	= alt_time/3600;
	alt_min		= (alt_time % 3600) / 60;
	alt_sec		= alt_time % 60;
	if (alt_hour) wsprintf(timeret,"%dh&nbsp;%02dm&nbsp;%02ds", alt_hour, alt_min, alt_sec);
	else wsprintf(timeret,"%dm&nbsp;%02ds", alt_min, alt_sec);
	return timeret;
	}

int getip(int which, struct sockaddr_in *sin) {
	if (which==3) return (sin->sin_addr.s_addr>>24);
	if (which==2) return ((sin->sin_addr.s_addr&0xff0000)>>16);
	if (which==1) return ((sin->sin_addr.s_addr&0xff00)>>8);
	return (sin->sin_addr.s_addr&0xff);
	}

char *gethost(struct sockaddr_in *sin) {
	static char hname[256];
	char ipaddr[16] = "";
	wsprintf(ipaddr, "%d.%d.%d.%d", getip(0,sin),getip(1,sin),getip(2,sin),getip(3,sin));
	if (g_perform_lookups) {
		int ip=inet_addr(ipaddr);
		struct hostent *p = gethostbyaddr((char *)&ip,4,AF_INET);
		if (p) wsprintf(hname,"%s",p->h_name);
		else wsprintf(hname, "%s", ipaddr); 
		}
	else wsprintf(hname, "%s", ipaddr);
	return hname;
	}

int recv_string(int s, char *str, int maxlen) {
	int sleeps=0;
	int p=0;
	do {
		int t;
		do {
			t=recv(s,str+p,1,0);
			if (t != 1) {
				if (!t || (t < 0 && WSAGetLastError() != WSAEWOULDBLOCK)) { str[p]=0; return -1; }
				Sleep(100);
				if (sleeps++ > 10*10) { str[p]=0; return -1; }
				}
			if (str[p] == '\r') t=0;
			} while (t!=1);
		} while (str[p] != '\n' && ++p < maxlen-1);
	str[p--]=0;
	while (str[p] == '\n' && p > 0) {
		str[p--]=0;
		}
	if (p < 0) p = 0;
	return p;
	}

void launchthread(void *threadproc, void *data) {
	_beginthread(threadproc,0,data);
	}

void quitthread() {
	_endthread();
	}

void setnonblock(int msgsock, int i) {
	ioctlsocket(msgsock,FIONBIO,&i);
	}

int OpenSocket(short port, int mc) {
	char *p=g_dst_ip;
	int  sock;
	struct sockaddr_in  sin;
  
	sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock < 0) return -1;
  
	memset((char *) &sin, 0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons( port );
	if (*p)  sin.sin_addr.s_addr = inet_addr(p);
	if (!*p || !sin.sin_addr.s_addr) sin.sin_addr.s_addr=INADDR_ANY;
   
	if (bind (sock,(struct sockaddr *)&sin,sizeof(sin))) {
		CloseSocket(sock);
		return -1;
		}
  
	if (listen(sock,mc)==-1) {
		CloseSocket(sock);
		return -1;
		}
	return sock;
	}

void CloseSocket(int sock) {
	shutdown(sock, 2);
	closesocket(sock);
	}

int init_socketlib(int which) {
	char log_area[16]="[sock]";
	WSADATA wsaData;
	if (which) {
		if (WSAStartup( MAKEWORD(1, 1), &wsaData) ) {
			log_printf( "%s Error initializing winsock on port: %d.\n", log_area, g_dst_port );
			return -1;
			}
		else log_printf( "%s initializing port: %d...\n", log_area, g_dst_port );
		}
	else {
		WSACleanup();
		log_printf( "%s shutting down socket.", log_area );
		}
	return 0;
	}

char *extension(char *fn) {
	char *s = fn + strlen(fn);
	while (s >= fn && *s != '.' && *s != '\\') s--;
	if (s < fn) return "";
	if (*s == '\\') return "";
	return (s+1);
	}

void quit() {
	//printf("Hit any key to close...");
	//fflush(stdout);
	//getch();
	exit(1);
	}

int LoadConfig(void) {
	char log_area[16]="[conf]";
	int line_number;
	FILE *conf=NULL;
	char buffer[1024];
	char *pbuffer = buffer;
	char *tok;
	char *configarray[] = {    
		"Port", 
		"AccessLoginPassword", 
		"AdminLoginPassword",
		"LogFile", 
		"NameLookups",
		"IP",
		"DBPath",
		"Extensions",
		"WinampDir",
		"RefreshRate",
		"DBFileList",
		"ShowRequests",
		"ShoutCastServer",
		"EndOfPlaylistAction",
		"FillerStreamURL",
		"CoverArtFilename",
		"ShowCoverArt"
		};

	conf = fopen(g_config_file,"rb");
	if (!conf) {
		printf( "%s Error: couldn't find %s.\n", log_area, g_config_file );
		return -1;
		}
	line_number=0;
	while (1) {
		int i;
		fgets(buffer, sizeof(buffer), conf);
		if ( feof(conf) ) break;
		while ( buffer[strlen(buffer)-1]=='\n' || buffer[strlen(buffer)-1]=='\r' ) buffer[strlen(buffer)-1]='\0';
		line_number++;
      
		pbuffer=buffer;
		while (*pbuffer == ' ' || *pbuffer == '\t') pbuffer++;
      
		if (!*pbuffer || *pbuffer == ';' || *pbuffer == '[' || *pbuffer == '#') continue; 
      
		tok=pbuffer;
		while (*pbuffer != '=' && *pbuffer != '\r' && *pbuffer) pbuffer++;
		if (!*pbuffer) { 
			printf("%s Error: invalid statement on line %d\n", log_area, line_number );
			return -1;
			}
		*pbuffer++=0;
		for (i=0; i < sizeof(configarray)/sizeof(configarray[0]) && stricmp(configarray[i],tok); i++);
		if (i >= sizeof(configarray)/sizeof(configarray[0])) {
			log_printf("[conf] Error: invalid item on line %d\n", line_number);

			return -1;
			}
      
		tok=pbuffer;
		while (*tok == ' ') tok++;
      
		switch (i) {
			case 0:		g_dst_port = atoi(tok);							break;
			case 1:		encodeLP(g_acclp, tok);							break;
			case 2:		encodeLP(g_admlp, tok);							break;
			case 3:		strcpy(g_log_file, tok);						break;
			case 4:		g_perform_lookups = !!atoi(tok);				break;
			case 5:		if (stricmp(tok,"ANY")) strcpy(g_dst_ip,tok);	break;
			case 6:		strcpy(g_db_path,tok);							break;
			case 7:		strcpy(g_ext_types,tok);						break;
			case 8:		strcpy(g_winamp_dir,tok);						break;
			case 9:		g_left_refresh=atoi(tok);						break;
			case 10:	strcpy(g_db_filelist,tok);						break;
			case 11:	g_show_requests=!!atoi(tok);					break;
			case 12:	strcpy(g_sc_server,tok);						break;
			case 13:	g_eop_action=atoi(tok);							break;
			case 14:	strcpy(g_filler_stream_url,tok);				break;
			case 15: 	strcpy(g_cover_art_filename,tok);				break;
			case 16:	g_show_cover_art=!!atoi(tok);					break;
			default:	break;
			}
		}
	fclose(conf);
	if (!stricmp(g_log_file,"none") || !stricmp(g_log_file,"/dev/null") || strlen(g_log_file) < 1) g_log=0;
	else {
		if (!strstr(g_log_file,"\\")) {
			char a[MAX_PATH] = "";
			strcat(a, g_working_dir);
			strcat(a, g_log_file);
			strcpy(g_log_file, a);
			}
		}
	if (g_ext_types[0]) {
		char *p=g_ext_type_list;
		strcpy(g_ext_type_list,g_ext_types);
		g_ext_type_list[strlen(g_ext_type_list)+1]=0;
		while (p && *p) {
			p=strstr(p,";");
			if (p && *p) *p++=0;
			}
		}
	
	log_printf( "%s Succesfully loaded config from: %s.\n", log_area, g_config_file );
	return 0;
	}

void log_printf(char *format, ...) { 
	char buf[1024]; 
	time_t t; 
	struct tm *tm; 
	va_list ar;
	va_start(ar,format);
	t = time(NULL);
	tm = localtime(&t);
	wsprintf(buf,"[%02d/%02d/%02d %02d:%02d:%02d]%s",
		tm->tm_mon+1, 
		tm->tm_mday, 
		tm->tm_year+1900, 
		tm->tm_hour, 
		tm->tm_min, 
		tm->tm_sec, 
		format);
	EnterCriticalSection(&log_mutex);
	if (g_log) {
		FILE *fp = fopen(g_log_file,"a+");
		if (fp) {
			vfprintf(fp,buf,ar);
			fclose(fp);
			}
		}
	vprintf(buf,ar);
	fflush(stdout);
	LeaveCriticalSection(&log_mutex);
	va_end(ar);
	}

int sock_printf(int sock, char *fmt, ...) {
	char buffer[2048]={0,};
	char *p=buffer;
	int i;
	va_list ar;
	va_start(ar,fmt);
	vsprintf(buffer,fmt,ar);
	va_end(ar);
	i=strlen(buffer);
	while (i > 0) {
		int r=send(sock,p,i,0);
		if (r > 0) i-=r;
		else {
			if (r < 0) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) break;
				else Sleep(250);
				}
			}
		}
	return strlen(buffer)-i;
	}

int sock_send(int sock, char *buffer,int size) {
	char *p=buffer;
	int i=size;
	while (i > 0) {
		int r=send(sock,p,i,0);
		if (r > 0) i-=r;
		else {
			if (r < 0) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) break;
				else Sleep(250);
				}
			}
		}
	return size-i;
	}

void doRecursiveAddDB(char *leading_path, char *this_path) {
	char log_area[20] = "doadddb -->";
	char maskstr[MAX_PATH];
	WIN32_FIND_DATA d;
	HANDLE h;
	char *leading_path_suffixed;
	wsprintf(maskstr,"%s%s",leading_path,leading_path[strlen(leading_path)-1]=='\\' ? "" : "\\");
	leading_path_suffixed=strdup(maskstr);

	if (*this_path) {
		strcat(maskstr,this_path);
		if (this_path[strlen(this_path)-1]!='\\') strcat(maskstr,"\\");
		}
	strcat(maskstr,"*.*");
	h = FindFirstFile(maskstr,&d);
	//printf("%s\n",maskstr);
	if ( h != INVALID_HANDLE_VALUE ) {
		do {
			if ( !(d.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) ) {
				char *p=extension(d.cFileName);
				if (*p && in_list(g_ext_type_list,p)) {
					if (database_used >= database_size) {
						if (database) {
							database_size*=2;
							database = realloc(database,database_size*sizeof(dbType));
							}
						else {
							database_size=32;
							database = malloc(database_size*sizeof(dbType));
							}
						}
					wsprintf(database[database_used].file,"%s%s%s",this_path,this_path[0]?"\\":"",d.cFileName);
					database[database_used].leading_path=leading_path_suffixed;
					database_used++;
					if (!(database_used%16)) {
						int x;
						char s[1024];
						wsprintf(s,"Scanning: %s",d.cFileName);
						x=79-strlen(s); while (x-- > 0) strcat(s," ");
						s[79]=0;
						printf("%s\r",s);
						}
					n_cnt++;
					}
				}
			else {
				if (d.cFileName[0] != '.') {
					char ps[MAX_PATH];
					wsprintf(ps,"%s%s%s",this_path,this_path[0]?"\\":"",d.cFileName);
					doRecursiveAddDB(leading_path,ps);
					}
				}
			} while (FindNextFile(h,&d));
		FindClose(h);
		}
	}

int _compare( const dbType *arg1, const dbType *arg2 ) {
	int i;
	char *p;
	int c1=0,c2=0;
	p=(char *)(arg1->file);
	while (*p) { if (*p == '\\' || *p == '/') c1++; p++; }
	p=(char *)(arg2->file);
	while (*p) { if (*p == '\\' || *p == '/') c2++; p++; }
	i=c1-c2;
	if (!i) i=stricmp(arg1->file,arg2->file);
	return i;
	}

void encodeLP(char *out, char *in) {
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int shift = 0;
	int accum = 0;

	while (*in) {
		if (*in) {
			accum <<= 8;
			shift += 8;
			accum |= *in++;
			}
		while ( shift >= 6 ) {
			shift -= 6;
			*out++ = alphabet[(accum >> shift) & 0x3F];
			}
		}
	if (shift == 4) {
		*out++ = alphabet[(accum & 0xF)<<2];
		*out++ = '=';  
		}
	else 
		if (shift == 2) {
			*out++ = alphabet[(accum & 0x3)<<4];
			*out++ = '=';  
    		*out++ = '=';  
			}
	*out++=0;
	}

void makeDB() {
	static char path_buf[1025];
	char log_area[16]="[mkDB]";
	char *p;
	log_printf( "%s building database...\n", log_area );
	strcpy(path_buf,g_db_path);
	path_buf[strlen(path_buf)+1]=0;
	p=path_buf;
	while ( p && *p ) {
		p = strstr( p, ";" );
    	if ( p && *p ) *p++=0;
		}
	p=path_buf;
	while (*p) {
		int oldpos=database_used;
		n_cnt=0;
		doRecursiveAddDB(p,"");
		// find a better way to clear out the printf'ed songs from doRecursiveAddDB
		printf("                                                                                            \r");											
		log_printf( "%s %d files under %s\n", log_area, n_cnt, p );
		qsort( database+oldpos, database_used-oldpos, sizeof(dbType), _compare );
		p+=strlen(p)+1;
		}
	p=g_db_filelist;
	while (p && *p) {
		p = strstr(p,";");
		if (p&&*p) *p++=0;
		}
	p=g_db_filelist;
	while (*p) {
		FILE *inf=fopen(p,"rt");
		if (inf) {
			int oldpos=database_used;
			char buf[1024];
			char *lp;
			n_cnt=0;
			fgets(buf,1024,inf);
			while (buf[0] && (buf[strlen(buf)-1] == '\r' || buf[strlen(buf)-1] == '\n')) buf[strlen(buf)-1]=0;
			lp=strdup(buf);
			while (!feof(inf)) {
				fgets(buf,1024,inf);
				while (buf[0] && (buf[strlen(buf)-1] == '\r' || buf[strlen(buf)-1] == '\n')) buf[strlen(buf)-1]=0;
				if (buf[0] && !feof(inf)) {
					if (database_used >= database_size) {
						if (database) {
							database_size*=2;
							database = realloc(database,database_size*sizeof(dbType));
							}
						else {
							database_size=32;
							database = malloc(database_size*sizeof(dbType));
							}
						}
					strcpy(database[database_used].file,buf);
					database[database_used].leading_path=lp;
					database_used++;
					n_cnt++;
					}
				}
			log_printf("%s %d files added from %s\n", log_area, n_cnt, p );
			qsort(database+oldpos,database_used-oldpos,sizeof(dbType),_compare);
			fclose(inf);
			}
		p+=strlen(p)+1;
		}
	//log_printf( "%s sorting database...\n", log_area );
	//qsort(database,database_used,sizeof(dbType),_compare);
	}

void http_sendredirect(int sock, char *host, char *fmt, ...) {
	char log_area[16]="[rdir]";
	char buffer[2048]={0,};
	va_list ar;
	va_start(ar,fmt);
	vsprintf(buffer,fmt,ar);
	va_end(ar);
	if (g_show_requests) log_printf( "%s %s \"%s\"\n", log_area, host, buffer );
	sock_printf( sock,
		"HTTP/1.0 302 Found\r\n"
		"Content-type:text/html\r\n"
		"Server: " SERV_NAME SERV_VER "\r\n"
		"Location: %s\r\n"
		"\r\n"
		"<HTML>\r\n<HEAD>\r\n\t<TITLE>Redirect</TITLE>\r\n</HEAD>\r\n\r\n<BODY>\r\nClick <a href=\"%s\">HERE</A> for redirect.\r\n</BODY>\r\n</HTML>\r\n",
		buffer, buffer );
	}

void http_handlereq(char *url, char *user_agent, char *encodedlp, int sock, struct sockaddr_in *sin) {
	int midHeight				= 31;
	int isCE	 				= !!strstr(user_agent,"Windows CE");
	int isStyle					= !strncmp(url, STYLE_URL,	 strlen(STYLE_URL));
	int isJScript				= !strncmp(url, JS_URL,		 strlen(JS_URL));
	int isControls				= !strncmp(url, CONTROL_URL, strlen(CONTROL_URL));
	int isCoverImage			= (strstr(url,g_cover_art_filename)!=NULL);
	int isRobots				= !strncmp(url, ROBOTS_URL,	 strlen(ROBOTS_URL));
	int admreq					= !strncmp(url, ADMIN_URL,	 strlen(ADMIN_URL));
	int is_adm					= admreq && (!g_admlp[0] ||  !strcmp( encodedlp, g_admlp ) || !strcmp( g_admlp, ":" ));
	char thisurl[128]			= "/";
	char log_area[16]			= "[http]";
	char mode[256]				= "";
	char add[256]				= "";
	char opt[256]				= "";
	char search[256]			= "";
	char srchstart[256]			= "";
	char headHTTPOK[256]		= "HTTP/1.0 200 OK\r\n";
	char headServer[256]		= "Server: " SERV_NAME SERV_VER "\r\n";
	char headContentType[256]	= "Content-Type: text/html\r\n";
	char headNoCache[256]		= "Cache-Control: no-cache, must-revalidate\r\nPragma: no-cache\r\nExpires: -1\r\n";
	char headCache[256]			= "Cache-Control: max-age=31536000\r\n";
	char headStyle[256]			= "\t<link rel=\"stylesheet\" href=\"" STYLE_URL "\">\n";
	char *p						= strstr(url,"?");
	struct {
		char *key;	
		char *val;
		} httpgetdata[]= {
		{"m=",mode},
		{"o=",opt},
		{"a=",add},
		{"s=",search},
		{"ss=",srchstart},
		};
	while (p && *p) {
		// if url has no ?, then this will not execute...
		int x;
		for (x = 0; x < sizeof(httpgetdata)/sizeof(httpgetdata[0]) && strncmp( p+1, httpgetdata[x].key, strlen(httpgetdata[x].key) ); x++);
		if ( x < sizeof(httpgetdata)/sizeof(httpgetdata[0]) ) {
			char *p2=httpgetdata[x].val;
			*p2=0;
			strncpy(p2,p+strlen(httpgetdata[x].key)+1,256);
			while (*p2 && *p2 != '&') p2++;
			*p2=0;
			}
		p=strstr(p+1,"&");
		}

	if ( strstr(mode,"#") ) {
		char *q;
		q = mode + strlen(mode);
		while (q >= mode && *q != '#') q--; *q=0;
		}

	if (is_adm) strcpy(thisurl, ADMIN_URL);
	if (g_show_requests) log_printf( "%s %s \"%s\" \"%s\"\n", log_area, gethost(sin), url, user_agent );
	// ROBOTS.TXT file to disallow server pages to be indexed.
	if (isRobots) {
		sock_printf( sock, "%s%sContent-type: text/plain\r\n\r\n", headHTTPOK, headCache );
		sock_printf( sock, "User-agent: *\r\nDisallow: /\r\n" );
		return;
		}
	// STYLE SHEET
	if (isStyle) {
		FILE *style=NULL;
		char stylebuffer[1024];
		char style_file[MAX_PATH]="style.css";

		wsprintf( style_file,"%s%s.css", g_working_dir, g_working_name );
		sock_printf( sock, "%s%sContent-type: text/css\r\n\r\n", headHTTPOK, headCache );
		style = fopen(style_file, "rb");
		if (style) {
			while (1) {
				if (feof(style)) break;
				fgets(stylebuffer, sizeof(stylebuffer), style);
				sock_printf(sock, "%s", stylebuffer);
				}
			fclose(style);
			}
		else {
			log_printf( "%s Error: couldn't find %s .\n", log_area, style_file );
			sock_printf( sock, "body, font { font-family: Tahoma,Verdana,Ariel; font-size: 85%; }\npre { font-family: lucida console; font-size: 10 px; }" );
			}
		return;
		}
	if (isJScript) {
		FILE *jscript=NULL;
		char jscriptbuffer[1024];
		char jscript_file[MAX_PATH]="jscript.js";

		wsprintf( jscript_file,"%s%s.js", g_working_dir, g_working_name );
		sock_printf( sock, "%s%sContent-type: application/x-javascript\r\n\r\n", headHTTPOK, headCache );
		jscript = fopen(jscript_file, "rb");
		if (jscript) {
			while (1) {
				if (feof(jscript)) break;
				fgets(jscriptbuffer, sizeof(jscriptbuffer), jscript);
				sock_printf(sock, "%s", jscriptbuffer);
				}
			fclose(jscript);
			}
		else {
			log_printf( "%s Error: couldn't find %s .\n", log_area, jscript_file );
			sock_printf( sock, "alert('Javascript File Missing');\r\n" );
			}
		return;
		}

	if (isControls) {
		FILE *control=NULL;
		char controlbuffer[1024];
		char control_file[MAX_PATH]="controls.gif";

		wsprintf( control_file,"%s\\controls.gif", g_working_dir );
		sock_printf( sock, "%s%sContent-type: image/gif\r\n\r\n", headHTTPOK, headCache );
		control = fopen(control_file, "rb");
		if (control) {
			int readBytes;
			while (!feof(control)) {
				readBytes=fread(controlbuffer,sizeof(char),1024,control);
				sock_send(sock,controlbuffer,readBytes);
				}

			fclose(control);
			}
		else {
			log_printf( "%s Error: couldn't find %s .\n", log_area, control_file );
			sock_printf( sock, "alert('Javascript File Missing');\r\n" );
			}
		return;
		}
	if (isCoverImage){
		FILE *coverImage=NULL;
		char coverBuffer[1024];
		char coverFilename[MAX_PATH];
		char tmp[MAX_PATH];
		
		strcpy(coverFilename,database[0].leading_path);
		unfixstring(url+1,tmp,sizeof(tmp));
		strcat(coverFilename,tmp);
		sock_printf( sock, "%s%s%s%sContent-type: image/jpeg\r\n\r\n", headHTTPOK, headCache);
		coverImage = fopen(coverFilename, "rb");
		if (coverImage) {
			int readBytes;
			while (!feof(coverImage)) {
				readBytes=fread(coverBuffer,sizeof(char),1024,coverImage);
				sock_send(sock,coverBuffer,readBytes);
				}
			fclose(coverImage);
			}
		else {
			log_printf( "%s Error: couldn't find %s .\n", log_area, coverFilename );
			}
		return;
		}
		
	// check privledges...
	// do not put above downloaded files (css & js)
	if (!is_adm && ((encodedlp[0] && strcmp(encodedlp,g_acclp)) || admreq)) {
		sock_printf( sock, 
			"HTTP/1.0 401 Unauthorized\r\n" 
			"WWW-Authenticate: basic realm=\"" BRANDING "%s\"\r\n" 
			"content-type:text/plain\r\n\r\nAuthorization failed.\r\n"
			, admreq?" Administrator Login":"" );
		return;
		}

	// mode is a string... if we check the 0th byte and it's 0 (null) then
	// the string is empty...   and the if will fail, but we ! it so we it passes
	// the only time mode should be empty is on the initial request (or if a user hits reload)
	if (!mode[0]) {
		sock_printf( sock, "%s%s%s\r\n"
			"<HTML>\n"
			"<HEAD>\n"
			"\t<TITLE>" BRANDING " " SERV_NAME "</TITLE>\n%s"
			"</HEAD>\n\n"
			"<FRAMESET ROWS=\"50%%,%d,50%%\" FRAMEBORDER=\"NO\" FRAMESPACING=\"0\" BORDER=\"0\">\n"
			"\t<FRAME SRC=\"%s?m=left#playing\" NAME=\"wwLeft\">\n"
			"\t<FRAME SRC=\"%s?m=control\" NAME=\"wwControl\" SCROLLING=\"NO\">\n"
			"\t<FRAME SRC=\"%s?m=right\" NAME=\"wwRight\">\n"
			"</FRAMESET>\n\n</HTML>\n",
			headHTTPOK, headServer, headCache, !isCE?headStyle:"", midHeight, thisurl, thisurl, thisurl );
		}
	else {
		// mode is not null... 

		// set up window handlers
		HWND hwnd_wa = FindWindow("Winamp v1.x",NULL);
		HWND hwnd_pe = FindWindow("Winamp PE",NULL);

		// ckeck to see if the option is emtpy.
		if ( opt[0] ) {
			// if we have an option do an action, all actions will redirect the user to the left panel when complete
			if (hwnd_wa) { 
				// handler exists.  Winamp is open
				// atoi-Convert an ASCII string to an integer
				int r=atoi(add);
			
				/* 
					// i have no clue what the for loop checks for, but i removed cuz didn't seem to do anything.
					// used to be with delete logic....
					for (;;) {
						DWORD id1, id2;
						if (!hwnd_pe) break;
						GetWindowThreadProcessId( hwnd_wa, &id1 );
						GetWindowThreadProcessId( hwnd_wa, &id2 );
						if (id1==id2) break;
						}
				*/
					
				
				if		( !strcmp(opt,"rew") && is_adm)		SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON1, 0);
				else if ( !strcmp(opt,"pl") && is_adm)		SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON2, 0); // play would take it out of standby mode
				else if ( !strcmp(opt,"pau") && is_adm)		SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON3, 0);
				else if ( !strcmp(opt,"unp") && is_adm)		SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON3, 0);
				else if ( !strcmp(opt,"st") && is_adm)		SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON4, 0);
				else if ( !strcmp(opt,"for") && is_adm)		SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON5, 0);
				else if ( !strcmp(opt,"random") && is_adm)	SendMessage( hwnd_pe, WM_COMMAND, ID_PE_S_RANDOM, 0); // got this undocumented feature from tag
				else if ( !strcmp(opt,"d") && is_adm) {
					if (hwnd_pe) SendMessage( hwnd_pe, WM_USER, 104, r);
					}
				else if ( !strcmp(opt,"uppl") && is_adm) {
					log_printf( "%s %s\n", "[list]", "Updating Playlist" );	
					}
				else if ( !strcmp(opt,"rsDB") && is_adm) {
					int x;
					log_printf( "%s %s\n", "[rsDB]", "Rescanning files and creating new database..." );
					
					for ( x = 0; x < database_used; x++ ) {
						wsprintf( database[x].file, "" );
						wsprintf( database[x].leading_path, "" );
						}
					database_used=0;
					makeDB();
					}
				else if ( !strcmp(opt,"j") && is_adm) {
					// the user jumped to a file, so we're no longer in standby mode
					g_playing_standby=0;
					SendMessage( hwnd_wa, WM_WA_IPC, r, 121 );
					SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON2, 0 );
					}
				else { 
					// add track

					// this is the ghetto (but less buggy filler track checker...
					/*
					char this_title[2048], *p;

					GetWindowText( hwnd_wa, this_title, sizeof(this_title) );
					p = this_title + strlen(this_title)-8;
					while (p >= this_title) {
						if (!strnicmp(p,"- Winamp",8)) break;
						p--;
				    	}

					if (p >= this_title) p--;
					while (p >= this_title && *p == ' ') p--;
					*++p=0;

					playingStandby=(strstr(this_title,g_filler_stream_title)!=NULL);
					*/
					if ( 0 <= r && r < database_used ) {
						char s[MAX_PATH];
						COPYDATASTRUCT cds;
						strcpy( s, database[r].leading_path );
						strcat( s, database[r].file );
						if (!strcmp(opt,"cue") && is_adm) {
							fileinfo song;
							strcpy(song.file, s);
							song.index = SendMessage( hwnd_wa, WM_WA_IPC, 0, 120 );
							song.index += 1;
							cds.dwData = IPC_PE_INSERTFILENAME;
							cds.lpData = (void *) &song;
							cds.cbData = sizeof(fileinfo);
							SendMessage(hwnd_pe,  WM_COPYDATA, (WPARAM) IPC_PE_INSERTFILENAME, (LPARAM) &cds );
							}
						if (!strcmp(opt,"add") ) {
							cds.dwData = IPC_PLAYFILE;
							cds.lpData = (void *) s;
							cds.cbData = strlen((char *) cds.lpData)+1;
							SendMessage( hwnd_wa, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds );
							}
						if ( !SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_ISPLAYING ) ) SendMessage( hwnd_wa, WM_WA_IPC, 0, IPC_STARTPLAY );
						}

						// if it's playing the standby track
						// just skip to new track
						if (g_playing_standby) SendMessage( hwnd_wa, WM_COMMAND, WINAMP_BUTTON5, 0);
						g_playing_standby=0;
					}
				}
			else {
				if ( !strcmp(opt,"swa") && is_adm ) {
					// no handler exists.  Winamp is not open.  if the option is to start winamp, 
					// then we attempt to start winamp.
					char s[MAX_PATH];
					STARTUPINFO si={sizeof(si),};
					PROCESS_INFORMATION pi;
					wsprintf(s,"%s\\winamp.exe /play",g_winamp_dir);
					log_printf("[admin] Trying to execute %s\n",s);
					CreateProcess( NULL, s, NULL, NULL, 0, 0, NULL, g_winamp_dir, &si, &pi );
					}
				}
			// send http redirect after our action
			http_sendredirect( sock, gethost(sin), "%s?m=%s#playing", thisurl, mode );
			}
		else {// not action
			char symbols[32] = "symbols";
			int useWebDings = 1;

			// mode is not null, but add and opt are... so we are simply displaying a page...

			sock_printf( sock, "%s%s%s\r\n", headHTTPOK, headServer, headNoCache );
			if (!isCE) sock_printf( sock, "<HTML>\r\n" );
			sock_printf(sock, "<HEAD>\n%s", !isCE ? headStyle : "");
			if (!isCE && !strcmp(mode,"left") && g_left_refresh) sock_printf(sock,"\t<META HTTP-EQUIV=\"Refresh\" CONTENT=\"%d; URL=%s\">\n", g_left_refresh, url);
			// commented out javascript call
			// if (!isCE && !strcmp(mode,"left")) sock_printf(sock,"\t<script language=\"JavaScript\" src=\"" JS_URL "\"></script>\r\n");
			sock_printf(sock, "</HEAD>\n\n<BODY CLASS=\"WWWinamp_%s\">\n\n", mode);

			if (!strcmp(mode,"left")) {
				// find the winamp playlist....
				if (hwnd_wa) {
					char fn[MAX_PATH];
					FILE *fp;
					EnterCriticalSection(&playlist_mutex);
					wsprintf( fn, "%s\\winamp.m3u", g_winamp_dir );
					fp = fopen( fn, "rt" );
					if (fp) {
						int x=0;
						int cursong = SendMessage( hwnd_wa, WM_WA_IPC, 0, 120 );
						int numtracks = SendMessage( hwnd_wa, WM_WA_IPC, 0, 124 );
						int listlen = numtracks - cursong;
						int ps=SendMessage(hwnd_wa,WM_WA_IPC,0,IPC_ISPLAYING);
						char tempID3[MAX_PATH];
						
						if (listlen==1 && ps==0) { 
							// if one track left, but stopped (i.e., after last song), either
							// play a random track
							if (g_eop_action==eop_mode_random) { 
								int r = (int)(database_used * (int)myrand() / (double)(RAND_MAX+1));
								char s[MAX_PATH];
								COPYDATASTRUCT cds;
								strcpy( s, database[r].leading_path );
								strcat( s, database[r].file );

								cds.dwData = IPC_PLAYFILE;
								cds.lpData = (void *) s;
								cds.cbData = strlen((char *) cds.lpData)+1;
								SendMessage( hwnd_wa, WM_WA_IPC, 0, 101 ); // clear playlist
								SendMessage( hwnd_wa, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds ); // add new track

								// skip to the new track and play
								SendMessage( hwnd_wa, WM_WA_IPC, 1, 121 );
								SendMessage( hwnd_wa, WM_WA_IPC, numtracks, 102 );
	
								g_playing_standby=1;
								}
							else {
								// play filler stream
								if(g_eop_action==eop_mode_stream){
									char prog[MAX_PATH];
									wsprintf(prog,"%s\\winamp.exe",g_winamp_dir);
									_spawnl(_P_NOWAIT,prog,"booger.mp3",g_filler_stream_url,NULL);
									g_playing_standby=1;
									}
								}
							}

						sock_printf(sock,"<pre>");
						while (1) {
							char tmp[1024]="blah";
							char *p, *tp, *ffile;
							fgets(fn,sizeof(fn),fp);
							if (feof(fp)) break;
							p=fn;
							ffile=fn;
							if (p[0] && p[strlen(p)-1] == '\n') p[strlen(p)-1]=0;
							while (*p == ' ') p++;
							if (*p == '#') {
								if ( !strncmp(p,"#EXTINF",7) ) {
									while (*p != ',') p++;
									strcpy(tempID3,++p);
									}
								continue;
								}
							if ( tempID3[0] ) {
								strcpy(tmp,tempID3);
								tempID3[0]=0;
								}
							else {
								p=fn+strlen(fn);
								while (*p != '\\' && *p != '/' && p >= fn) p--;
								while (*p != '\\' && *p != '/' && p >= fn) p--;
								if (p[0]) p++;
								tp=p;
								while (*tp) tp++;
								while (tp > p && *tp != '.' && *tp != '\\' && *tp != '/') tp--;
								if (tp > p && *tp == '.') *tp=0;
								unfixstring(p,tmp,sizeof(tmp));
								}
								
							if (x == cursong) {
								sock_printf( sock, "<a name=\"playing\">");
								if (!isCE) {
									sock_printf( sock, "<div class=\"current\">");
									sock_printf( sock, "now playing: %d. %s<b class=\"CurrentSong\">%s</b>\n", x+1, is_adm?"[&raquo;<b>&raquo;</b>&raquo;<b>&raquo;</b>] ":"", tmp );
									if(g_show_cover_art && !g_playing_standby) showCoverArt(sock,fn,database[0].leading_path);
									}
								else sock_printf(sock,"%s\n",tmp);
								switch ( SendMessage( hwnd_wa, WM_WA_IPC,0,IPC_ISPLAYING) ) {
									case 0: sock_printf(sock,"%stopped", isCE?"S":"Winamp is s"); break;
									case 1: sock_printf(sock,"%slaying", isCE?"P":"Winamp is p"); break;
									case 3: sock_printf(sock,"%saused",  isCE?"P":"Winamp is p"); break;
									default: break;
									}
								sock_printf( sock, ", <b>%d of %d file%s</b> left in queue\n", listlen, numtracks, numtracks==1?"":"s");
								if (!isCE) sock_printf( sock, "</div>");
								}
							else {
								if (!isCE || (isCE && (x > cursong) && (x < cursong + 10))) {
									sock_printf( sock, "% 6d. ", x+1 );
									if (is_adm) {
										sock_printf(sock,"[<a href=\"%s?m=left&o=j&a=%d\">j</a>]",thisurl,x);
										sock_printf(sock,"[<a href=\"%s?m=left&o=d&a=%d\">d</a>] ",thisurl,x);
										}
									//sock_printf( sock, "%s (%s)\n", tmp, ffile );
									sock_printf( sock, "%s\n", tmp );
									}
								}
							x++;
							}
						sock_printf(sock,"</pre>");
						fclose(fp);          
						LeaveCriticalSection(&playlist_mutex);
						} 
					else if (!isCE) sock_printf(sock,"<B>No Winamp playlist found</B><br><br>");
					}
				else {
					sock_printf( sock, "%s", !isCE?"<B>No Winamp running on server machine</B><br>":"No Winamp - " );
					if (is_adm) sock_printf(sock,"[ <a href=\"%s?m=left&o=swa\">%s</a> ]\n\n",thisurl, !isCE?"Try starting Winamp":"start");
					}
				}
			else if (!strcmp(mode,"control")) {
				/*
				if (is_adm) {
					sock_printf( sock, 
						"[ <a href=\"%s?m=left&o=rew\" class=\"%s\">%s</a> ]"
						"[ <a href=\"%s?m=left&o=pl\"  class=\"%s\">%s</a> ]"
						"[ <a href=\"%s?m=left&o=pau\" class=\"%s\">%s</a> ]"
						"[ <a href=\"%s?m=left&o=st\"  class=\"%s\">%s</a> ]"
						"[ <a href=\"%s?m=left&o=for\" class=\"%s\">%s</a> ]",
						thisurl, symbols, useWebDings ? "&#55;" : "rew",
						thisurl, symbols, useWebDings ? "&#52;" : "play",
						thisurl, symbols, useWebDings ? "&#59;" : "pause",
						thisurl, symbols, useWebDings ? "&#60;" : "stop",
						thisurl, symbols, useWebDings ? "&#56;" : "for",
						);
					}

				if (is_adm) {
					sock_printf( sock, 
						"<img src=\"%s\" width=165 height=30>"
						"<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=rew\">%s</a> | \n"
						"<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=pl\">%s</a>  | \n"
						"<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=pau\">%s</a> | \n"
						"<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=st\">%s</a>  | \n"
						"<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=for\">%s</a> | \n"
						"<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=random\">Rand%s</a> | \n"
						,
						"/controls.gif",
						thisurl, "rew",
						thisurl, "play",
						thisurl, "pause",
						thisurl, "stop",
						thisurl, "forward",
						thisurl, isCE?"":"omize List"
						);
					}
				sock_printf( sock, 
					"<a onClick=\"parent.document.body.rows='45,%d,*'\" href=\"#\">Max Search</a> | \n"
					"<a onClick=\"parent.document.body.rows='50%%,%d,50%%'\" href=\"#\">Default</a> | \n"
					"<a onClick=\"parent.document.body.rows='*,%d,45'\" href=\"#\">Max Playlist</a> | \n"
					,
					midHeight, midHeight, midHeight
					);
				*/
				sock_printf( sock, "<table class=\"resizeControl\">\n<tr><td>\n" );
				if ( is_adm ) {
					sock_printf( sock, 
						"<MAP NAME=\"controls_Map\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"6,6,23,23\"     target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=rew\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"26,6,43,23\"    target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=pl\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"46,6,63,23\"    target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=pau\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"66,6,83,23\"    target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=st\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"86,6,103,23\"   target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=for\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"128,1,156,9\"   onClick=\"parent.document.body.rows='45,%d,*'\" href=\"#\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"128,11,156,18\" onClick=\"parent.document.body.rows='50%%,%d,50%%'\" href=\"#\">\n"
						"<AREA SHAPE=\"rect\" COORDS=\"128,20,156,28\" onClick=\"parent.document.body.rows='*,%d,45'\" href=\"#\">\n"
						"</MAP>"
						"<img src=\"%s\" width=165 height=30 border=0 usemap=\"#controls_Map\">"
						"</td><td>"
						,
						thisurl, thisurl, thisurl, thisurl, thisurl, 
						midHeight, midHeight, midHeight,
						CONTROL_URL
						);
					}
				
				if ( g_sc_server[0] ) sock_printf( sock, " <a target=\"blank\" href=\"%slisten.pls\">Listen</a> | \n", g_sc_server );
				if ( g_sc_server[0] ) sock_printf( sock, " <a target=\"blank\" href=\"%s\">Shoutcast Server</a> | \n", g_sc_server );
				//sock_printf( sock, "<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left#playing\">current</a> | \n", thisurl );
				sock_printf( sock, "<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=uppl\">update</a> | \n", thisurl );
				sock_printf( sock, "<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=random\">Rand%s</a> | \n", thisurl, isCE?"":"omize" );
				if ( is_adm ) sock_printf( sock, "<a target=\"wwLeft\" onclick=\"javascript:window.location.reload()\" href=\"%s?m=left&o=rsDB\">update library</a> | \n", thisurl );
				if ( g_playing_standby ) sock_printf( sock, "Standby Mode | " );
				sock_printf( sock, "</td></tr>\n</table>\n" );
				}
			
			else if (!strcmp(mode,"right")) {
				// right hand side.
				int x, start, showAll=0, nf=0, pagesize = isCE ? 64 : 512;
				char searchstring[1024], *p;
	
				unfixstring(search,searchstring,1024);
				p=searchstring;
	
				while (*p) {
					if (*p == '+') *p=' ';
					p++;
					}
				if (!strcmp(searchstring,"*")) showAll=1;
				if (!isCE) {
					sock_printf(sock,"<table border=0 cellspacing=0 cellpadding=3>\n"
						"<tr valign=\"top\">\n"
						"<form method=\"get\" action=\"%s\">\n"
						"<input type=hidden name=\"m\" value=\"right\">\n"
						"<td><input type=text name=\"s\" size=\"30\" maxlen=\"64\" value=\"%s\"></td>\n"
						"<td><input type=submit value=\"Search\"></td>\n</form>\n",
						thisurl,
						searchstring);
					if (!showAll) sock_printf(sock,"<form method=\"get\" action=\"%s\">\n"
							"<input type=hidden name=\"m\" value=\"right\">\n"
							"<input type=hidden name=\"s\" value=\"*\">\n"
							"<td><input type=submit value=\"Show All\"></td>\n"
							"</form>\n",
							thisurl);
					sock_printf(sock,"</table><BR>\n");
					}
				if ( showAll || strlen(searchstring)>1 ) {
					char last_leading_path[1024], *ssout=(char*)malloc(strlen(searchstring)*2+8);
					parselist(ssout,searchstring);
					strcpy(last_leading_path,"\\\\\\\\\\");
					start=atoi(srchstart);
	
					if (!isCE) sock_printf( sock, "<b>" );
					if (showAll) sock_printf(sock,"Showing all");
					else {
						if (isCE) sock_printf( sock, "search: '%s'", searchstring );
						else sock_printf( sock, "Searching for '%s' in %d item%s.", searchstring, database_used, database_used==1?"":"s" );
						} 
					if (start > 0) sock_printf(sock, " (%s%d)",!isCE?"Starting at ":"start=", start);
					if (!isCE) sock_printf( sock, "...</b>" );
					sock_printf( sock, "..\n\n<p><pre>");

					for (x = 0; x < database_used; x ++) {
						int found=showAll || substr_search(database[x].file,ssout) || substr_search(database[x].leading_path,ssout);
						if (!found) {
							char buf[1024];
							unfixstring(database[x].file,buf,1024);
							found=substr_search(buf,ssout);
							}
						if (found && ++nf >= start && nf < start+pagesize) {
							char leading_path[1024];
							char buf[4096];
							char *tp=buf;
							char *ptr=database[x].file;
							{
							char *p=buf;
							unfixstring(ptr,buf,sizeof(buf));
							while (*p) p++;
							while (p > buf && *p != '\\' && *p !=  '/') p--;
							*p=0;
							strncpy(leading_path,buf,sizeof(leading_path)-1);
							leading_path[sizeof(leading_path)-1]=0;
							if (stricmp(leading_path,last_leading_path)) {
								char fullFilename[MAX_PATH];

								strcpy(last_leading_path,leading_path);
								wsprintf(fullFilename,"%s%s",database[x].leading_path,database[x].file);
								
								sock_printf(sock,"<p>\n");
								if(g_show_cover_art) showCoverArt( sock, fullFilename, database[0].leading_path );
								sock_printf(sock,"<p><b>%s</b>\n",leading_path);
								}
							}
							while (*ptr) ptr++;
							while (*ptr != '\\' && *ptr != '/' && ptr > database[x].file) ptr--;
							if (ptr > database[x].file) ptr--;
							while (*ptr != '\\' && *ptr != '/' && ptr >= database[x].file) ptr--;
							ptr++;
							sock_printf(sock,"% 7d. ",x+1);
							if (is_adm) {
								char f[1024];
								fixstring(database[x].file,f);
								sock_printf(sock,"[<a href=\"%s?m=left&o=cue&a=%d\" target=\"wwLeft\">c</a>] ",thisurl,x);
								}
							unfixstring(ptr,buf,sizeof(buf));
							while (*tp) tp++;
							while (tp > buf && *tp != '.' && *tp != '\\' && *tp != '/') tp--;
							if (tp > buf && *tp == '.') *tp=0;
							while (tp >= buf && *tp != '\\' && *tp != '/') tp--;
							tp++;
							sock_printf(sock,"<a href=\"%s?m=left&o=add&a=%d\" target=\"wwLeft\">%s</a>\n",thisurl,x,tp);
							}
						}
					sock_printf(sock,"</pre>\n\n");

					if (nf) {
						if (start>0) sock_printf( sock, " <b>&laquo;[</b><a href=\"%s?m=right&s=%s&ss=%d\">last%s</a><b>]</b> ",
								thisurl,
								search,
								max(start-pagesize,0),
								isCE?"":" page" );
						sock_printf( sock, "Showing %d of %d item%s",
							min(pagesize,nf-start),
							nf,nf==1?"":"s" );
						if (nf > start+pagesize) sock_printf( sock, " <b>[</b><a href=\"%s?m=right&s=%s&ss=%d\">next%s</a><b>]&raquo;</b>\n\n",
								thisurl,
								search,
								start+pagesize,
								isCE?"":" page" );
						}
					else sock_printf( sock, "<b>No items found</b>\n\n" );
					free(ssout);
					}
				if (!isCE) sock_printf( sock, "\n"
					"<P><div class=\"footer\">" SERV_NAME_LONG "<br>\n"
					BRANDING " " SERV_NAME " " SERV_VER "<BR>\n"
					"<a href=\"" BRANDING_URL "\" target=\"_blank\">" BRANDING_URL "</a><br>\n" 
					BRANDING_COPYRIGHT "<br>\n"
					COPYRIGHT "</div>\n");
				}
			sock_printf( sock, "\n</body>\n</html>\n");
			}
		}
	return;
	}


void showCoverArt(int sock, char* fn, char* leading_path) {
	char coverArtFilePath[MAX_PATH];
	char imageURL[1024];
	char dir[MAX_PATH];
	int lastSlashPos=strlen(fn);
	int foundLastSlash=0;
	FILE* fTmp;
	char* p;

	p=fn+lastSlashPos;
	while (p >= fn) {
		if(*p=='\\'){ // URL-ize backslashes
			*p='/';
			if(!foundLastSlash){
				lastSlashPos=p-fn;
				foundLastSlash=1;
				}
			}
		p--;
		}

	strcpy(dir,fn);
	dir[lastSlashPos+1]=0; // truncate dir string before filename

	strcpy(coverArtFilePath,dir);
	strcat(coverArtFilePath,g_cover_art_filename);

	if(strlen(dir)>=strlen(leading_path)) {
		strcpy(imageURL,dir+strlen(leading_path)); // extract only the suffix, which used to make the request
		}
	strcat(imageURL,g_cover_art_filename);

	if(fTmp=fopen(coverArtFilePath,"r")) {
		sock_printf(sock,"<img align=\"right\" src=\"%s\">\n",imageURL);
		fclose(fTmp);
		}
	}

int myrand(void) {
	g_rand_next = g_rand_next * 1103515245 + 12345;
	return ( (g_rand_next >>16) & RAND_MAX);
	}

void mysrand (unsigned int seed) { 
	g_rand_next = seed;
	}
