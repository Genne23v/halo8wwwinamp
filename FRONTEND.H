//Psuedo IPC for frontends for Winamp v1.55+ (< 2.0)


#define WM_WA_IPC WM_USER
// messages are sent to the winamp window using:
//result = SendMessage(hwnd_winamp,WM_WA_IPC,command_data,command);


/* Messages available to send */

#define IPC_GETVERSION 0
/*
        IPC_GETVERSION is sent to the window, and the return value is the version
                Version 1.55 = 0x1551
                Version 1.6b = 0x16A0
                Version 1.60 = 0x16AF
                Version 1.61 = 0x16B0
                Version 1.62 = 0x16B1
                Version 1.64 = 0x16B3
        the command_data parameter is 0.
        so, 
        if (SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_GETVERSION) != 0x1551)
                MessageBox(NULL,"Error, Winamp 1.55 not found","Warning",MB_OK);
*/


#define IPC_PLAYFILE 100
/*
        IPC_PLAYFILE is sent to the window for each char of a null terminated string of a file to ADD
        to the playlist (it doesn't change the playing status)
        for example:
                char *file = "C:\\download\\booga.mp3";
                int x;
                for (x = 0; x <= strlen(file); x ++)
                        PostMessage(hwnd_winamp,WM_WA_IPC,(LPARAM)file[x],IPC_PLAYFILE);
        will add "C:\download\booga.mp3" to the end of the playlist
*/

#define IPC_DELETE 101
/* 
        IPC_DELETE deletes the internal Winamp playlist.
                SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_DELETE);
*/

#define IPC_STARTPLAY 102
/* 
        IPC_STARTPLAY starts the playing.
                SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_STARTPLAY);
*/

#define IPC_CHDIR 103
/*
        IPC_CHDIR is sent to the window for each char of a null terminated string of a directory to change to
        for example:
                char *dir = "C:\\Download";
                int x;
                for (x = 0; x <= strlen(file); x ++)
                        PostMessage(hwnd_winamp,WM_WA_IPC,(LPARAM)dir[x],IPC_PLAYFILE);
        will change the winamp process to "C:\download" (useful for relative pathnames and loading playlists)

*/
#define IPC_ISPLAYING 104
/*
        IPC_ISPLAYING returns the status of playback.
        If it returns 1, it is playing. if it returns 3, it is paused, if it returns 0, it is not playing.
        If it returns something other than 1,3,or 0, something is screwed.
        isplaying = SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_ISPLAYING);
*/


#define IPC_GETOUTPUTTIME 105
/*
        IPC_GETOUTPUTTIME returns the position in milliseconds of the current song.
        Returns -1 if not playing or error.
        song_pos = SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_GETOUTPUTTIME);
*/

#define IPC_JUMPTOTIME 106
/*
        *ONLY AVAILABLE IN v1.60+*
        IPC_JUMPTOTIME sets the position in milliseconds of the current song (approximately)
        Returns -1 if not playing, 1 on eof, or 0 if successful
        SendMessage(hwnd_winamp,WM_WA_IPC,new_song_pos,IPC_JUMPTOTIME);
*/


// THESE MIGHT CHANGE in the future :)
//Also, you can send standard WM_COMMAND messages to the Winamp window (for other controls), including
// Send using SendMessage(hwnd_winamp,WM_COMMAND,WINAMP_OPTIONS_EQ/*orwhatever*/,0);

// toggles the EQ window
#define WINAMP_OPTIONS_EQ               40036
// toggles the playlist window
#define WINAMP_OPTIONS_PLEDIT           40040
// turns the volume up a little
#define WINAMP_VOLUMEUP                 40058
// turns the volume down a little
#define WINAMP_VOLUMEDOWN               40059
// fast forwards 5 seconds
#define WINAMP_FFWD5S                   40060
// rewinds 5 seconds
#define WINAMP_REW5S                    40061
// the following are the five main control buttons, with optionally shift or control pressed
// (for the exact functions of each, just try it out)
#define WINAMP_BUTTON1                  40044
#define WINAMP_BUTTON2                  40045
#define WINAMP_BUTTON3                  40046
#define WINAMP_BUTTON4                  40047 //0x9C6F
#define WINAMP_BUTTON5                  40048
#define WINAMP_BUTTON1_SHIFT            40144
#define WINAMP_BUTTON2_SHIFT            40145
#define WINAMP_BUTTON3_SHIFT            40146
#define WINAMP_BUTTON4_SHIFT            0x9CD3
#define WINAMP_BUTTON5_SHIFT            40148
#define WINAMP_BUTTON1_CTRL             40154
#define WINAMP_BUTTON2_CTRL             40155
#define WINAMP_BUTTON3_CTRL             40156
#define WINAMP_BUTTON4_CTRL             40157
#define WINAMP_BUTTON5_CTRL             40158
// pops up the load file(s) box
#define WINAMP_FILE_PLAY                40029
// pops up the preferences
#define WINAMP_OPTIONS_PREFS            40012
// toggles always on top
#define WINAMP_OPTIONS_AOT              40019
// pops up the about box :)
#define WINAMP_HELP_ABOUT               40041


// hope this helps, 
// -Justin [justin@nullsoft.com]
