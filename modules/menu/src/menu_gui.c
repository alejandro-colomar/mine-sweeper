/******************************************************************************
 *	Copyright (C) 2015	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include <gtk/gtk.h>
		/* INFINITY */
	#include <math.h>
		/* true & false */
	#include <stdbool.h>
		/* sprintf() */
	#include <stdio.h>
		/* srand() */
	#include <stdlib.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include "about.h"
	#include "game_iface.h"
	#include "save.h"
	#include "score.h"
	#include "start.h"

	#include "menu_iface.h"

	#include "menu_gui.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	ROWS_GUI_MAX	(22)
#if (ROWS_GUI_MAX > ROWS_MAX)
#	error	"rows max (tui)"
#endif

	# define	COLS_GUI_MAX	(33)
#if (COLS_GUI_MAX > COLS_MAX)
#	error	"cols max (tui)"
#endif


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
struct	Button_Data {
	GtkWidget	*ptr;
	char		text [80];
	int		num;
	int		*sw;
};

struct	Label_Data {
	GtkWidget	*ptr;
	char		text [80];
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
GtkWidget	*window_gui;
bool		gui_is_open;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Init & cleanup */
static	gboolean	delete_event	(GtkWidget	*widget,
					GdkEvent	*event,
					void		*data);
	/* Selection */
static	void	callback_button		(GtkWidget	*widget,
					void		*data);
	/* Submenus */
static	void	menu_gui_continue	(void);
static	void	menu_gui_select		(void);
static	void	menu_gui_level		(void);
static	void	menu_gui_custom		(void);
static	void	menu_gui_devel		(void);
static	void	menu_gui_verbose	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	menu_gui_init		(void)
{
	/* Window */
	window_gui	= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gui_is_open	= true;

	/* Quit */
	g_signal_connect(window_gui, "delete-event", G_CALLBACK(delete_event), NULL);

	/* Title.  PROG_VERSION defined in Makefile */
	char		title [80];
	sprintf(title, "mine-sweeper %s", PROG_VERSION);
	gtk_window_set_title(GTK_WINDOW(window_gui), title);

	/* Border */
	gtk_container_set_border_width(GTK_CONTAINER(window_gui), 20);
}

void	menu_gui_cleanup	(void)
{
	/* Destroy window */
	gtk_widget_destroy(window_gui);
}

void	menu_gui		(void)
{
	bool			wh;
	int			sw;
	GtkWidget		*box;
	GtkWidget		*separator[3];
	struct Label_Data	label;
	struct Button_Data	button [4];

	/* Text */
	sprintf(label.text, "Main menu");
	sprintf(button[1].text, "Continue");
	sprintf(button[2].text, "Disclaimer of warranty");
	sprintf(button[3].text, "Terms and conditions");
	sprintf(button[0].text, "Exit program");

	/* Data */
	button[1].num	= 1;
	button[2].num	= 2;
	button[3].num	= 3;
	button[0].num	= 0;
	button[1].sw	= &sw;
	button[2].sw	= &sw;
	button[3].sw	= &sw;
	button[0].sw	= &sw;

	/* Menu loop */
	wh	= true;
	while (wh && gui_is_open) {

		/* Generate widgets */
		box		= gtk_vbox_new(false, 0);
		label.ptr	= gtk_label_new(label.text);
		separator[0]	= gtk_hseparator_new();
		button[1].ptr	= gtk_button_new_with_label(button[1].text);
		separator[1]	= gtk_hseparator_new();
		button[2].ptr	= gtk_button_new_with_label(button[2].text);
		button[3].ptr	= gtk_button_new_with_label(button[3].text);
		separator[2]	= gtk_hseparator_new();
		button[0].ptr	= gtk_button_new_with_label(button[0].text);

		/* Events */
		g_signal_connect(button[1].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[1]);
		g_signal_connect(button[2].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[2]);
		g_signal_connect(button[3].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[3]);
		g_signal_connect(button[0].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[0]);

		/* Container */
		gtk_container_add(GTK_CONTAINER(window_gui), box);

		/* Box */
		gtk_box_pack_start(GTK_BOX(box), label.ptr, false, false, 0);
		gtk_box_pack_start(GTK_BOX(box), separator[0], false, false, 5);
		gtk_box_pack_start(GTK_BOX(box), button[1].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), separator[1], false, false, 5);
		gtk_box_pack_start(GTK_BOX(box), button[2].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), button[3].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), separator[2], false, false, 5);
		gtk_box_pack_start(GTK_BOX(box), button[0].ptr, true, true, 0);

		/* Show */
		gtk_widget_show(label.ptr);
		gtk_widget_show(separator[0]);
		gtk_widget_show(button[1].ptr);
		gtk_widget_show(separator[1]);
		gtk_widget_show(button[2].ptr);
		gtk_widget_show(button[3].ptr);
		gtk_widget_show(separator[2]);
		gtk_widget_show(button[0].ptr);
		gtk_widget_show(box);
		gtk_widget_show(window_gui);

		/* GTK loop */
		sw	= 0;
		gtk_main();

		/* Clear window */
		gtk_widget_destroy(box);

		/* Selection */
		switch (sw) {
		case 0:
			wh	= false;
			break;
		case 1:
			menu_gui_continue();
			break;
		case 2:
			print_disclaim();
			break;
		case 3:
			print_license();
			break;
		}
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	Cleanup	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	gboolean	delete_event	(GtkWidget	*widget,
					GdkEvent	*event,
					void		*data)
{
	gui_is_open	= false;
	g_print ("delete event occurred\n");

	gtk_main_quit();

	/*
	 * true to block closing
	 * the window will be closed by menu_gui_cleanup()
	 */
	return	true;
}

/*	*	*	*	*	*	*	*	*	*
 *	*	Selection	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	callback_button		(GtkWidget	*widget,
					void		*data)
{
	struct Button_Data	*button;

	button	= ((struct Button_Data *)data);
	
	*(button->sw)	= button->num;

	gtk_main_quit();
}

/*	*	*	*	*	*	*	*	*	*
 *	*	Submenus	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	menu_gui_continue	(void)
{
	bool			wh;
	int			sw;
	GtkWidget		*box;
	GtkWidget		*separator[4];
	struct Label_Data	label;
	struct Button_Data	button [7];

	/* Text */
	sprintf(label.text, "Game menu");
	sprintf(button[1].text, "Start");
	sprintf(button[2].text, "Select map");
	sprintf(button[3].text, "Change difficulty");
	sprintf(button[5].text, "Hi scores");
	sprintf(button[6].text, "DEVEL");
	sprintf(button[0].text, "Back");

	/* Data */
	button[1].num	= 1;
	button[2].num	= 2;
	button[3].num	= 3;
	button[4].num	= 4;
	button[5].num	= 5;
	button[6].num	= 6;
	button[0].num	= 0;
	button[1].sw	= &sw;
	button[2].sw	= &sw;
	button[3].sw	= &sw;
	button[4].sw	= &sw;
	button[5].sw	= &sw;
	button[6].sw	= &sw;
	button[0].sw	= &sw;

	/* Menu loop */
	wh	= true;
	while (wh && gui_is_open) {
		/* Text */
		sprintf(button[4].text, "Change file name (File: \"%s\")", saved_name);

		/* Generate widgets */
		box		= gtk_vbox_new(false, 0);
		label.ptr	= gtk_label_new(label.text);
		separator[0]	= gtk_hseparator_new();
		button[1].ptr	= gtk_button_new_with_label(button[1].text);
		separator[1]	= gtk_hseparator_new();
		button[2].ptr	= gtk_button_new_with_label(button[2].text);
		button[3].ptr	= gtk_button_new_with_label(button[3].text);
		button[4].ptr	= gtk_button_new_with_label(button[4].text);
		button[5].ptr	= gtk_button_new_with_label(button[5].text);
		separator[2]	= gtk_hseparator_new();
		button[6].ptr	= gtk_button_new_with_label(button[6].text);
		separator[3]	= gtk_hseparator_new();
		button[0].ptr	= gtk_button_new_with_label(button[0].text);

		/* Events */
		g_signal_connect(button[1].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[1]);
		g_signal_connect(button[2].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[2]);
		g_signal_connect(button[3].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[3]);
		g_signal_connect(button[4].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[4]);
		g_signal_connect(button[5].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[5]);
		g_signal_connect(button[6].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[6]);
		g_signal_connect(button[0].ptr, "clicked",
				G_CALLBACK(callback_button), (void *)&button[0]);

		/* Container */
		gtk_container_add(GTK_CONTAINER(window_gui), box);

		/* Box */
		gtk_box_pack_start(GTK_BOX(box), label.ptr, false, false, 0);
		gtk_box_pack_start(GTK_BOX(box), separator[0], false, false, 5);
		gtk_box_pack_start(GTK_BOX(box), button[1].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), separator[1], false, false, 5);
		gtk_box_pack_start(GTK_BOX(box), button[2].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), button[3].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), button[4].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), button[5].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), separator[2], false, false, 5);
		gtk_box_pack_start(GTK_BOX(box), button[6].ptr, true, true, 0);
		gtk_box_pack_start(GTK_BOX(box), separator[3], false, false, 5);
		gtk_box_pack_start(GTK_BOX(box), button[0].ptr, true, true, 0);

		/* Show */
		gtk_widget_show(label.ptr);
		gtk_widget_show(separator[0]);
		gtk_widget_show(button[1].ptr);
		gtk_widget_show(separator[1]);
		gtk_widget_show(button[2].ptr);
		gtk_widget_show(button[3].ptr);
		gtk_widget_show(button[4].ptr);
		gtk_widget_show(button[5].ptr);
		gtk_widget_show(separator[2]);
//		gtk_widget_show(button[6].ptr);
		gtk_widget_show(separator[3]);
		gtk_widget_show(button[0].ptr);
		gtk_widget_show(box);
		gtk_widget_show(window_gui);

		/* GTK loop */
		sw	= 0;
		gtk_main();

		/* Selection */
		switch (sw) {
		case 0:
			gtk_widget_destroy(box);
			wh	= false;
			break;

		case 1:
			gtk_widget_destroy(box);
//			start_switch();
			break;

		case 2:
			gtk_widget_destroy(box);
			menu_gui_select();
			break;

		case 3:
			gtk_widget_destroy(box);
			menu_gui_level();
			break;

		case 4:
			save_clr();
//			alx_w_getfname(saved_path, saved_name, true, w2, r2,
//								txt[0], NULL);
			gtk_widget_destroy(box);
			break;

		case 5:
			gtk_widget_destroy(box);
			read_scores();
			break;
/*
		case 6:
			gtk_widget_destroy(box);
			menu_gui_devel();
			break;
*/
		}
	}
}

static	void	menu_gui_select	(void)
{
#if 0
	/* Menu dimensions & options */
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	h	= 9;
	w	= 70;
	r	= 1;
	c	= (80 - w) / 2;
	int	N;
	N	= 3;
	struct Alx_Menu	mnu[3]	= {
		{6, 4, "[0]	Back"},
		{2, 4, "[1]	New map"},
		{4, 4, "[2]	Load map"}
	};

	/* Menu loop */
	int	sw;
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
#endif
}

static	void	menu_gui_level	(void)
{
#if 0
	/* Menu dimensions & options */
	WINDOW	*win;
	int	h;
	int	w;
	h	= 10;
	w	= 70;
	int	N;
	N	= 5;
	struct Alx_Menu	mnu[5]	= {
		{7, 4, "[0]	Back"},
		{2, 4, "[1]	Beginner"},
		{3, 4, "[2]	Intermediate"},
		{4, 4, "[3]	Expert"},
		{5, 4, "[4]	Custom"}
	};

	/* Menu loop */
	int	sw;
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
#endif
}

static	void	menu_gui_custom	(void)
{
#if 0
	/* Menu dimensions & options */
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	h	= 16;
	w	= 76;
	r	= 1;
	c	= (80 - w) / 2;
	int	N;
	N	= 4;
	struct Alx_Menu	mnu[4]	= {
		{8, 4, "[0]	Back"},
		{2, 4, "[1]	Change rows:"},
		{4, 4, "[2]	Change columns:"},
		{6, 4, "[3]	Change proportion of mines:"}
	};

	/* Input box */
	int	w2;
	int	r2;
	w2	= w - 8;
	r2	= r + h - 5;
	char	*txt[]	= {
		"Rows:",
		"Columns:",
		"Proportion:"
	};

	/* Menu */
	win	= newwin(h, w, r, c);

	/* Menu loop */
	bool	wh;
	int	sw;
	wh	= true;
	while (wh) {
		mvwprintw(win, mnu[1].r, mnu[1].c, "%s rows\t\t(%i)", mnu[1].t, menu_iface_variables.rows);
		mvwprintw(win, mnu[2].r, mnu[2].c, "%s cols\t\t(%i)", mnu[2].t, menu_iface_variables.cols);
		mvwprintw(win, mnu[3].r, mnu[3].c, "%s p\t(%lf)", mnu[3].t, menu_iface_variables.p);
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

	/* Cleanup */
	alx_win_del(win);
#endif
}

static	void	menu_gui_devel	(void)
{
#if 0
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	h	= 12;
	w	= 50;
	r	= 1;
	c	= (80 - w) / 2;
	int	N;
	N	= 2;
	struct Alx_Menu	mnu[2]	= {
		{5, 4, "[0]	Back"},
		{2, 4, "[1]	Change seed (srand)"}
	};

	/* Input box */
	int	w2;
	int	r2;
	w2	= w - 8;
	r2	= r + h - 5;
	char	*txt[]	= {"Seed:"};

	/* Menu */
	win	= newwin(h, w, r, c);

	/* Menu loop */
	bool	wh;
	int	sw;
	wh	= true;
	int	seed;
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

	/* Cleanup */
	alx_win_del(win);
#endif
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
