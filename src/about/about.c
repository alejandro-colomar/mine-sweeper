/******************************************************************************
 *	Copyright (C) 2015	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "mine-sweeper/about/about.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define ALX_NO_PREFIX
#include <libalx/base/compiler/size.h>
#include <libalx/base/errno/error.h>
#include <libalx/base/stdio/printf/sbprintf.h>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define SHARE_DIR		"" INSTALL_SHARE_DIR "/mine-sweeper/"
#define SHARE_COPYRIGHT_FILE	"" SHARE_DIR "/COPYRIGHT.txt"
#define SHARE_DISCLAIMER_FILE	"" SHARE_DIR "/DISCLAIMER.txt"
#define SHARE_HELP_FILE		"" SHARE_DIR "/HELP.txt"
#define SHARE_LICENSE_FILE	"" SHARE_DIR "/LICENSE.txt"
#define SHARE_USAGE_FILE	"" SHARE_DIR "/USAGE.txt"


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	print_share_file	(int file)
{
	char	*fname;
	char	cmd[_POSIX_ARG_MAX];

	switch (file) {
	case SHARE_COPYRIGHT:
		fname	= SHARE_COPYRIGHT_FILE;
		break;
	case SHARE_DISCLAIMER:
		fname	= SHARE_DISCLAIMER_FILE;
		break;
	case SHARE_HELP:
		fname	= SHARE_HELP_FILE;
		break;
	case SHARE_LICENSE:
		fname	= SHARE_LICENSE_FILE;
		break;
	case SHARE_USAGE:
		fname	= SHARE_USAGE_FILE;
		break;
	}

	if (sbprintf(cmd, NULL, "less %s", fname))
		goto err;
	if (system(cmd))
		goto err;

	return;
err:
	perrorx(fname);
}

void	print_version		(void)
{

	printf("%s %s\n\n", program_invocation_short_name, PROG_VERSION);
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
