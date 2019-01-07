/******************************************************************************
 *	Copyright (C) 2015	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* INFINITY */
	#include <math.h>
	#include <ncurses.h>
	#include <stdbool.h>
		/* srand() */
	#include <stdlib.h>

/* libalx --------------------------------------------------------------------*/
	#include "alx_ncur.h"

/* Project -------------------------------------------------------------------*/
	#include "about.h"
	#include "game_iface.h"
	#include "save.h"
	#include "score.h"
	#include "start.h"

/* Module --------------------------------------------------------------------*/
	#include "menu_iface.h"

	#include "menu_tui.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	ROWS_TUI_MAX	(22)
#if (ROWS_TUI_MAX > ROWS_MAX)
#	error	"rows max (tui)"
#endif

	# define	COLS_TUI_MAX	(33)
#if (COLS_TUI_MAX > COLS_MAX)
#	error	"cols max (tui)"
#endif

	# define	BUFF_SIZE_TEXT	(1048576)


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	menu_tui_continue	(void);
static	void	menu_tui_select		(void);
static	void	menu_tui_level		(void);
static	void	menu_tui_custom		(void);
#if 0
static	void	menu_tui_devel		(void);
#endif
#if 0
static	void	menu_tui_verbose	(void);
#endif


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	menu_tui		(void)
{
	int	h;
	int	w;
	int	N;
	bool	wh;
	int	sw;

	alx_resume_curses();

	/* Menu dimensions & options */
	h	= 10;
	w	= 34;
	N	= 4;
	static	const struct Alx_Menu	mnu[4]	= {
		{7, 4, "[0]	Exit program"},
		{2, 4, "[1]	Continue"},
		{4, 4, "[2]	Disclaimer of warranty"},
		{5, 4, "[3]	Terms and conditions"}
	};

	/* Menu */
	wh	= true;
	while (wh) {
		/* Menu loop */
		sw	= alx_menu(h, w, N, mnu, "MENU:");

		/* Selection */
		switch (sw) {
		case 0:
			wh	= false;
			break;
		case 1:
			menu_tui_continue();
			break;
		case 2:
			alx_pause_curses();
			print_share_file(SHARE_DISCLAIMER);
			getchar();
			alx_resume_curses();
			break;
		case 3:
			alx_pause_curses();
			print_share_file(SHARE_LICENSE);
			getchar();
			alx_resume_curses();
			break;
		}
	}

	alx_pause_curses();
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	menu_tui_continue	(void)
{
	char	str [BUFF_SIZE_TEXT];
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	int	N;
	int	w2;
	int	r2;
	bool	wh;
	int	sw;

	/* Menu dimensions & options */
	h	= 18;
	w	= 50;
	r	= 1;
	c	= (80 - w) / 2;
	/* N = 7 if DEVEL option is enabled */
	N	= 6;
	static	const struct Alx_Menu	mnu[6]	= {
		{11, 4, "[0]	Back"},
		{2, 4, "[1]	Start"},
		{4, 4, "[2]	Select map"},
		{5, 4, "[3]	Change difficulty"},
		{6, 4, "[4]	Change file name"},
		{7, 4, "[5]	Hi scores"}/*,
		{9, 4, "[6]	DEVEL"}*/
	};

	/* Input box */
	w2	= w - 8;
	r2	= r + h - 5;
	static	const char	*txt[]	= {"File name:"};

	/* Menu */
	wh	= true;
	while (wh) {
		/* Menu loop */
		win	= newwin(h, w, r, c);
		mvwprintw(win, mnu[4].r, mnu[4].c, "%s (File: \"%s\")", mnu[4].t, saved_name);
		wrefresh(win);
		sw	= alx_menu_2(win, N, mnu, "CONTINUE:");

		/* Selection */
		switch (sw) {
		case 0:
			alx_win_del(win);
			wh	= false;
			break;
		case 1:
			alx_win_del(win);
			alx_pause_curses();
			start_switch();
			alx_resume_curses();
			break;
		case 2:
			alx_win_del(win);
			menu_tui_select();
			break;
		case 3:
			alx_win_del(win);
			menu_tui_level();
			break;
		case 4:
			save_clr();
			alx_w_getfname(saved_path, saved_name, true, w2, r2,
								txt[0], NULL);
			alx_win_del(win);
			break;
		case 5:
			alx_win_del(win);
			alx_pause_curses();
			snprint_scores(str, BUFF_SIZE_TEXT);
			printf("%s", str);
			getchar();
			alx_resume_curses();
			break;
/*
		case 6:
			alx_win_del(win);
			menu_tui_devel();
			break;
*/
		}
	}
}

static	void	menu_tui_select	(void)
{
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	int	N;
	int	sw;

	/* Menu dimensions & options */
	h	= 9;
	w	= 70;
	r	= 1;
	c	= (80 - w) / 2;
	N	= 3;
	static	const struct Alx_Menu	mnu[3]	= {
		{6, 4, "[0]	Back"},
		{2, 4, "[1]	New map"},
		{4, 4, "[2]	Load map"}
	};

	/* Menu loop */
	win	= newwin(h, w, r, c);
	mvwprintw(win, mnu[2].r, mnu[2].c, "%s (File: \"%s\")", mnu[1].t, saved_name);
	wrefresh(win);
	sw	= alx_menu_2(win, N, mnu, "SELECT MAP:");
	alx_win_del(win);

	/* Selection */
	switch (sw) {
	case 1:
		start_mode =	START_RAND;
		break;
	case 2:
		start_mode =	START_LOAD;
		break;
	}

}

static	void	menu_tui_level	(void)
{
	int	h;
	int	w;
	int	N;
	int	sw;

	/* Menu dimensions & options */
	h	= 10;
	w	= 70;
	N	= 5;
	static	const struct Alx_Menu	mnu[5]	= {
		{7, 4, "[0]	Back"},
		{2, 4, "[1]	Beginner"},
		{3, 4, "[2]	Intermediate"},
		{4, 4, "[3]	Expert"},
		{5, 4, "[4]	Custom"}
	};

	/* Menu loop */
	sw	= alx_menu(h, w, N, mnu, "SELECT LEVEL:");

	/* Selection */
	switch (sw) {
	case 1:
		menu_iface_variables.level	= GAME_IFACE_LEVEL_BEGINNER;
		break;

	case 2:
		menu_iface_variables.level	= GAME_IFACE_LEVEL_INTERMEDIATE;
		break;

	case 3:
		menu_iface_variables.level	= GAME_IFACE_LEVEL_EXPERT;
		break;

	case 4:
		menu_iface_variables.level	= GAME_IFACE_LEVEL_CUSTOM;
		menu_tui_custom();
		break;
	}

}

static	void	menu_tui_custom	(void)
{
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	int	N;
	int	w2;
	int	r2;
	bool	wh;
	int	sw;

	/* Menu dimensions & options */
	h	= 16;
	w	= 76;
	r	= 1;
	c	= (80 - w) / 2;
	N	= 4;
	static	const struct Alx_Menu	mnu[4]	= {
		{8, 4, "[0]	Back"},
		{2, 4, "[1]	Change rows:"},
		{4, 4, "[2]	Change columns:"},
		{6, 4, "[3]	Change proportion of mines:"}
	};

	/* Input box */
	w2	= w - 8;
	r2	= r + h - 5;
	static	const char	*txt[]	= {
		"Rows:",
		"Columns:",
		"Proportion:"
	};

	/* Menu */
	win	= newwin(h, w, r, c);

	/* Menu loop */
	wh	= true;
	while (wh) {
		mvwprintw(win, mnu[1].r, mnu[1].c, "%s rows\t\t(%i)",
					mnu[1].t, menu_iface_variables.rows);
		mvwprintw(win, mnu[2].r, mnu[2].c, "%s cols\t\t(%i)",
					mnu[2].t, menu_iface_variables.cols);
		mvwprintw(win, mnu[3].r, mnu[3].c, "%s p\t(%lf)",
					mnu[3].t, menu_iface_variables.p);
		wrefresh(win);

		/* Selection */
		sw	= alx_menu_2(win, N, mnu, "Custom:");

		switch (sw) {
		case 0:
			wh	= false;
			break;
		case 1:
			menu_iface_variables.rows	= alx_w_getint(w2, r2,
					txt[sw - 1], 2, menu_iface_variables.rows, ROWS_TUI_MAX, NULL);
			break;
		case 2:
			menu_iface_variables.cols	= alx_w_getint(w2, r2,
					txt[sw - 1], 2, menu_iface_variables.cols, COLS_TUI_MAX, NULL);
			break;
		case 3:
			menu_iface_variables.p		= alx_w_getdbl(w2, r2,
					txt[sw - 1], 0, menu_iface_variables.p, 1, NULL);
			break;
		}

	}

	alx_win_del(win);
}
#if 0
static	void	menu_tui_devel	(void)
{
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	int	N;
	int	w2;
	int	r2;
	bool	wh;
	int	sw;
	int	seed;

	h	= 12;
	w	= 50;
	r	= 1;
	c	= (80 - w) / 2;
	N	= 2;
	static	const struct Alx_Menu	mnu[2]	= {
		{5, 4, "[0]	Back"},
		{2, 4, "[1]	Change seed (srand)"}
	};

	/* Input box */
	w2	= w - 8;
	r2	= r + h - 5;
	static	const char	*txt[]	= {"Seed:"};

	/* Menu */
	win	= newwin(h, w, r, c);

	/* Menu loop */
	wh	= true;
	while (wh) {
		/* Selection */
		sw	= alx_menu_2(win, N, mnu, "DEVELOPER OPTIONS:");

		switch (sw) {
		case 0:
			wh	= false;
			break;
		case 1:
			seed	= alx_w_getint(w2, r2, txt[0],
						-INFINITY, 1, INFINITY, NULL);
			srand(seed);
			break;
		}
	}

	alx_win_del(win);
}
#endif


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
