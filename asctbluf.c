/* $title:       'AscTblUF.C ===== User functions for ASCII Table =====' */
/* $subtitle:   'Elipse Ltd.  [jms]                Revised: 1995 Mar 14' */


#define INCL_WINDIALOGS         /*   Dialog boxes                     */
#define INCL_WINLISTBOXES       /*   List box controls                */
#define INCL_WINWINDOWMGR       /*   Window functions                 */
#define INCL_WINSHELLDATA       /*   Profile functions                */
#include <os2.h>                /* Presentation Manager include file  */

#include <stdio.h>
#include <string.h>
#include "asctbl.ids"
#include "asctbl.ext"

extern long Argc;
extern char **Argv;

static HINI open_profile( HAB hab );


/* SHOW_ASCII_TABLE ===== Write to listbox ===== $pageif:6 */

void show_ascii_table( PGPFPARMS pGpfParms )
    {                                   /* show_ascii_table */
    HWND hwnd = pGpfParms->hwnd;
    char buf[32];
    int i;
    memset( buf, '\0', sizeof( buf ) );

    for ( i = 0;  i < 256;  ++i )
        {
        sprintf( buf, " %3d   %02x   %c", i, i, i );
        WinSendDlgItemMsg( hwnd, ID_LISTBOX, LM_INSERTITEM, MPFROM2SHORT( LIT_END, 0 ), MPFROMP( buf ) );
        }
    }                                   /* show_ascii_table */


/* SET_POSITION ===== Of window from previous display ===== $pageif:6 */

void set_position( PGPFPARMS pGpfParms )
    {                                   /* set_position */
    HINI hini = open_profile( pGpfParms->hab );
    if (hini != (HINI) 0)
        {
        HWND hwnd = GpfGetHwndFrame( pGpfParms->hwnd );
        long x, y;
        x = PrfQueryProfileInt( hini, "Position", "Horizontal", -1 );
        y = PrfQueryProfileInt( hini, "Position",  "Vertical",  -1 );
        PrfCloseProfile( hini );
        if ((x > 0) && (y > 0))
            WinSetWindowPos( hwnd, HWND_TOP, x, y, 0, 0, SWP_MOVE );
        }
    }                                   /* set_position */


/* SAVE_POSITION ===== Of window for next display ===== $pageif:6 */

void save_position( PGPFPARMS pGpfParms )
    {                                   /* save_position */
    HWND hwnd = GpfGetHwndFrame( pGpfParms->hwnd );
    HINI hini;
    SWP swp;
    WinQueryWindowPos( hwnd, &swp );
    hini = open_profile( pGpfParms->hab );
    if (hini != (HINI) 0)
        {
        char buf[16];
        memset( buf, '\0', sizeof( buf ) );
        sprintf( buf, "%d", swp.x );
        PrfWriteProfileString( hini, "Position", "Horizontal", buf );
        sprintf( buf, "%d", swp.y );
        PrfWriteProfileString( hini, "Position",  "Vertical",  buf );
        PrfCloseProfile( hini );
        }
    }                                   /* save_position */


/* OPEN_PROFILE ===== In correct directory ===== $pageif:6 */

static HINI open_profile( HAB hab )
    {                                   /* open_profile */
    char filebuf[260], *p;
    memset( filebuf, '\0', sizeof( filebuf ) );
    strcpy( filebuf, Argv[0] );
    strupr( filebuf );
    p = strstr( filebuf, ".EXE" );
    if (p != NULL)
        {
        *(p+1) = 'I';
        *(p+2) = 'N';
        *(p+3) = 'I';       
        *(p+4) = '\0';
        return PrfOpenProfile( hab, filebuf );
        }
    else
        return (HINI) 0;
    }                                   /* open_profile */
