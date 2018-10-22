/******************************************************************************
 *	Copyright (C) 2017	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		ALX_INPUT_HPP
	# define	ALX_INPUT_HPP


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
	#include <math.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <stdint.h>


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
extern	"C" {


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	int	alx_sscan_dbl	(double		*dest,
				double		m,
				double		def,
				double		M,
				const char	*str);

	int	alx_sscan_int	(int64_t	*dest,
				double		m,
				int64_t		def,
				double		M,
				const char	*str);

	int	alx_sscan_fname	(const char	*fpath,
				char		*fname,
				bool		exist,
				const char	*str);

	double	alx_getdbl	(double		m,
				double		def,
				double		M,
				const char	*formatA,
				const char	*formatB,
						...);

	int64_t	alx_getint	(double		m,
				int64_t		def,
				double		M,
				const char	*formatA,
				const char	*formatB,
						...);


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
}	/* extern "C" */


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* alx_input.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
