/******************************************************************************
 *	Copyright (C) 2015	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "mine-sweeper/menu/parser.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "libalx/base/stdlib/seed.h"

#include "mine-sweeper/about/about.h"
#include "mine-sweeper/ctrl/start.h"
#include "mine-sweeper/game/iface.h"
#include "mine-sweeper/menu/iface.h"
#include "mine-sweeper/player/iface.h"
#include "mine-sweeper/save/save.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define OPT_LIST	"xhLuv""a:b:f:i:p:r:s:"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	parse_rows		(char *argument);
static	void	parse_columns		(char *argument);
static	void	parse_file		(char *argument);
static	void	parse_iface		(char *argument);
static	void	parse_proportion	(char *argument);
#if 0
static	void	parse_rand_seed		(char *argument);
#endif
static	void	parse_start		(char *argument);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	parser	(int argc, char *argv[])
{
	int	opt		= 0;
	int	opt_index	= 0;

	const struct option	long_options[]	= {
		/* Standard */
		{"exit",		no_argument,		0,	'x'},
		{"help",		no_argument,		0,	'h'},
		{"license",		no_argument,		0,	'L'},
		{"usage",		no_argument,		0,	'u'},
		{"version",		no_argument,		0,	'v'},
		/* Non-standard */
		{"rows",		required_argument,	0,	'a'},
		{"columns",		required_argument,	0,	'b'},
		{"file",		required_argument,	0,	'f'},
		{"iface",		required_argument,	0,	'i'},
		{"proportion",		required_argument,	0,	'p'},
//		{"rand-seed",		required_argument,	0,	'r'},
		{"start",		required_argument,	0,	's'},
		/* Null */
		{0,			0,			0,	0}
	};

	while ((opt = getopt_long(argc, argv, OPT_LIST, long_options,
						&opt_index)) != -1) {

		switch (opt) {
		/* Standard */
		case 'x':
			flag_exit	= true;
			break;
		case 'h':
			print_share_file(SHARE_HELP);
			exit(EXIT_SUCCESS);
		case 'L':
			print_share_file(SHARE_LICENSE);
			exit(EXIT_SUCCESS);
		case 'u':
			print_share_file(SHARE_USAGE);
			exit(EXIT_SUCCESS);
		case 'v':
			print_version();
			exit(EXIT_SUCCESS);

		/* Non-standard */
		case 'a':
			parse_rows(optarg);
			break;
		case 'b':
			parse_columns(optarg);
			break;
		case 'f':
			parse_file(optarg);
			break;
		case 'i':
			parse_iface(optarg);
			break;
		case 'p':
			parse_proportion(optarg);
			break;
# if 0
// for DEVEL
		case 'r':
			parse_rand_seed(optarg);
			break;
# endif
		case 's':
			parse_start(optarg);
			break;
		case '?':
			/* getopt_long already printed an error message. */
		default:
			print_share_file(SHARE_USAGE);
			exit(EXIT_FAILURE);
		}
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	parse_rows		(char *argument)
{

	menu_iface_variables.rows	= atoi(argument);
	if ((menu_iface_variables.rows < 2)  ||
			(menu_iface_variables.rows > ROWS_MAX)) {
		printf("--rows argument not valid\n");
		printf("It must be an integer [%i U %i]\n", 2, ROWS_MAX);
		exit(EXIT_FAILURE);
	}
}

static	void	parse_columns		(char *argument)
{

	menu_iface_variables.cols	= atoi(argument);
	if ((menu_iface_variables.cols < 2)  ||
			(menu_iface_variables.cols > COLS_MAX)) {
		printf("--columns argument not valid\n");
		printf("It must be an integer [%i U %i]\n", 2, COLS_MAX);
		exit(EXIT_FAILURE);
	}
}

static	void	parse_file		(char *argument)
{
	FILE	*fp;

	// FIXME
	fp	= fopen(argument, "r");
	if (!fp)
		goto err_fp;
	fclose(fp);

	saved_path[0]	= '\0';
	snprintf(saved_name, FILENAME_MAX, argument);

	return;

err_fp:
	printf("--file argument not valid\n");
	printf("It must be a valid file name (relative to saved dir)\n");
	exit(EXIT_FAILURE);
}

static	void	parse_iface		(char *argument)
{

	menu_iface_mode		= atoi(argument);
	player_iface_mode	= menu_iface_mode;
	if ((menu_iface_mode < MENU_IFACE_CLUI)  ||
			(menu_iface_mode > MENU_IFACE_GUI)) {
		printf("--iface argument not valid\n");
		printf("It must be an integer [%i U %i]\n",
					MENU_IFACE_CLUI, MENU_IFACE_GUI);
		exit(EXIT_FAILURE);
	}
}

static	void	parse_proportion	(char *argument)
{

	menu_iface_variables.p	= atof(argument);
	if ((menu_iface_variables.p < 0)  ||
			(menu_iface_variables.p > 1)) {
		printf("--proportion argument not valid\n");
		printf("It must be a real [0 U 1]\n");
		exit(EXIT_FAILURE);
	}
}
#if 0
static	void	parse_rand_seed		(char *argument)
{
	int	seed;

	seed	= atof(argument);
	srand(seed);
}
#endif
static	void	parse_start		(char *argument)
{

	start_mode	= atoi(argument);
	if ((start_mode < START_FOO)  ||  (start_mode > START_LOAD)) {
		printf("--start argument not valid\n");
		printf("It must be an integer [%i U %i]\n",
					START_FOO, START_LOAD);
		exit(EXIT_FAILURE);
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
