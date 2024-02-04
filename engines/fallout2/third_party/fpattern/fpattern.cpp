/*******************************************************************************
* fpattern.c
*	Functions for matching filename patterns to filenames.
*
* Usage
*	(See "fpattern.h".)
*
* Notes
*	These pattern matching capabilities are modeled after those found in
*	the UNIX command shells.
*
*	`DELIM' must be defined to 1 if pathname separators are to be handled
*	explicitly.
*
* History
*	1.0, 1997-01-03, David Tribble.
*	First cut.
*
*	1.1, 1997-01-03, David Tribble.
*	Added the SUB pattern character.
*	Added function fpattern_matchn().
*
*	1.2, 1997-01-12, David Tribble.
*	Fixed missing lowercase matching cases.
*
*	1.3, 1997-01-13, David Tribble.
*	Pathname separator code is now controlled by DELIM macro.
*
*	1.4, 1997-01-14, David Tribble.
*	Added the QUOTE macro.
*
*	1.5, 1997-01-15, David Tribble.
*	Handles the special case of empty pattern and empty filename.
*
*	1.6, 1997-01-26, David Tribble.
*	Changed the range negation character from '^' to '!', ala Unix.
*
*	1.7, 1997-08-02, David Tribble.
*	Uses the 'FPAT_XXX' constants.
*
*	1.8, 1998-06-28, David Tribble.
*	Minor fixes for MS-VC++ (5.0).
*
*	1.9, 2001-11-21, David Tribble.
*	Minor fixes for Win32 compilations.
*
* Limitations
*	This code is copyrighted by the author, but permission is hereby granted
*	for its unlimited use provided that the original copyright and
*	authorship notices are retained intact.
*
*	Queries about this source code can be sent to: <david@tribble.com>
*
* Copyright 1997-2001 by David R. Tribble, all rights reserved.
*/

#include "fallout2/third_party/fpattern/fpattern.h"

#include "common/debug.h"
#include "common/system.h"

namespace Fallout2 {


/* System includes */
/*
#include <ctype.h>
#include <stddef.h>


#if TEST
 #include <locale.h>
 #include <stdio.h>
 #include <string.h>
#endif
*/

/*
#if defined(unix) || defined(_unix) || defined(__unix)
 #define UNIX	1
 #define DOS	0
#elif defined(__MSDOS__) || defined(_WIN32)
 #define UNIX	0
 #define DOS	1
#else
 #error Cannot ascertain the O/S from predefined macros
#endif
*/
#define DOS 1


/* Local constants */

/*
#ifndef nullptr
 #define nullptr		((void *) 0)
#endif

#ifndef false
 #define false		0
#endif

#ifndef true
 #define true		1
#endif

#if TEST
 #define SUB		'~'
#else
 #define SUB		FPAT_CLOSP
#endif
*/
#define SUB         FPAT_CLOSP

#ifndef DELIM
#define DELIM		0
#endif

#define DEL			FPAT_DEL

#if UNIX
#define DEL2		FPAT_DEL
#else /*DOS*/
#define DEL2		FPAT_DEL2
#endif

#if UNIX
#define QUOTE		FPAT_QUOTE
#else /*DOS*/
#define QUOTE		FPAT_QUOTE2
#endif


/* Local function macros */

#if UNIX
#define lowercase(c)	(c)
#else /*DOS*/
#define lowercase(c)	tolower(c)
#endif


/*------------------------------------------------------------------------------
* fpattern_isvalid()
*	Checks that filename pattern 'pat' is a well-formed pattern.
*
* Returns
*	True (1) if 'pat' is a valid filename pattern, otherwise false (0).
*
* Caveats
*	If 'pat' is null, false (0) is returned.
*
*	If 'pat' is empty (""), true (1) is returned, and it is considered a
*	valid (but degenerate) pattern (the only filename it matches is the
*	empty ("") string).
*/

int fpattern_isvalid(const char *pat) {
	int	len;


	debug("fpattern_isvalid: pat=%p:\"%s\"", pat, pat ? pat : "");

	/* Check args */
	if (pat == nullptr) {
		debug("Null pattern");
		return false;
	}

	/* Verify that the pattern is valid */
	for (len = 0;  pat[len] != '\0';  len++) {
		switch (pat[len]) {
		case FPAT_SET_L:
			/* Char set */
			len++;
			if (pat[len] == FPAT_SET_NOT)
				len++;			/* Set negation */

			while (pat[len] != FPAT_SET_R) {
				if (pat[len] == QUOTE)
					len++;		/* Quoted char */
				if (pat[len] == '\0') {
					debug("Missing '%c'", FPAT_SET_R);
					return false;	/* Missing closing bracket */
				}
				len++;

				if (pat[len] == FPAT_SET_THRU) {
					/* Char range */
					len++;
					if (pat[len] == QUOTE)
						len++;		/* Quoted char */
					if (pat[len] == '\0') {
						debug("Missing '%c%c'",
						      FPAT_SET_THRU, FPAT_SET_R);
						return false;	/* Missing closing bracket */
					}
					len++;
				}

				if (pat[len] == '\0') {
					debug("Missing '%c'", FPAT_SET_R);
					return false;	/* Missing closing bracket */
				}
			}
			break;

		case QUOTE:
			/* Quoted char */
			len++;
			if (pat[len] == '\0') {
				debug("Missing quoted char");
				return false;		/* Missing quoted char */
			}
			break;

		case FPAT_NOT:
			/* Negated pattern */
			len++;
			if (pat[len] == '\0') {
				debug("Missing negated subpattern");
				return false;		/* Missing subpattern */
			}
			break;

		default:
			/* Valid character */
			break;
		}
	}

	debug("fpattern_isvalid: return %d", len);
	return true;
}


/*------------------------------------------------------------------------------
* fpattern_submatch()
*	Attempts to match subpattern 'pat' to subfilename 'fname'.
*
* Returns
*	True (1) if the subfilename matches, otherwise false (0).
*
* Caveats
*	This does not assume that 'pat' is well-formed.
*
*	If 'pat' is empty (""), the only filename it matches is the empty ("")
*	string.
*
*	Some non-empty patterns (e.g., "") will match an empty filename ("").
*/

static int fpattern_submatch(const char *pat, const char *fname) {
	int		fch;
	int		pch;
	int		i;
	int		yes, match;
	int		lo, hi;

	debug("fpattern_submatch: fname=\"%s\", pat=\"%s\"", fname, pat);

	/* Attempt to match subpattern against subfilename */
	while (*pat != '\0') {
		fch = *fname;
		pch = *pat;
		pat++;

		switch (pch) {
		case FPAT_ANY:
			/* Match a single char */
			#if DELIM
			if (fch == DEL  ||  fch == DEL2  ||  fch == '\0') {
			#else
			if (fch == '\0') {
			#endif
				debug("match=F");
				return false;
			}
			fname++;
			break;

		case FPAT_CLOS:
			/* Match zero or more chars */
			i = 0;
			#if DELIM
			while (fname[i] != '\0'  &&
			        fname[i] != DEL  &&  fname[i] != DEL2)
				i++;
			#else
			while (fname[i] != '\0')
				i++;
			#endif
			while (i >= 0) {
				if (fpattern_submatch(pat, fname+i)) {
					debug("submatch=T for +%d", i);
					return true;
				}
				i--;
			}
			return false;

		case SUB:
			/* Match zero or more chars */
			i = 0;
			while (fname[i] != '\0'  &&
				#if DELIM
			       fname[i] != DEL  &&  fname[i] != DEL2  &&
				#endif
			       fname[i] != '.')
				i++;
			while (i >= 0) {
				if (fpattern_submatch(pat, fname+i))
					return true;
				i--;
			}
			return false;

		case QUOTE:
			/* Match a quoted char */
			pch = *pat;
			if (lowercase(fch) != lowercase(pch)  ||  pch == '\0')
				return false;
			fname++;
			pat++;
			break;

		case FPAT_SET_L:
			/* Match char set/range */
			yes = true;
			if (*pat == FPAT_SET_NOT) {
				pat++;
				yes = false;	/* Set negation */
			}

			/* Look for [s], [-], [abc], [a-c] */
			match = !yes;
			while (*pat != FPAT_SET_R  &&  *pat != '\0') {
				if (*pat == QUOTE)
					pat++;	/* Quoted char */

				if (*pat == '\0')
					break;
				lo = *pat++;
				hi = lo;

				if (*pat == FPAT_SET_THRU) {
					/* Range */
					pat++;

					if (*pat == QUOTE)
						pat++;	/* Quoted char */

					if (*pat == '\0')
						break;
					hi = *pat++;
				}

				if (*pat == '\0')
					break;

				/* Compare character to set range */
				if (lowercase(fch) >= lowercase(lo)  &&
				        lowercase(fch) <= lowercase(hi))
					match = yes;
			}

			if (!match) {
				debug("match=F");
				return false;
			}

			if (*pat == '\0')
				return false;		/* Missing closing bracket */

			fname++;
			pat++;
			break;

		case FPAT_NOT:
			/* Match only if rest of pattern does not match */
			if (*pat == '\0')
				return false;		/* Missing subpattern */
			i = fpattern_submatch(pat, fname);
			debug("submatch=%c", "FT"[!!i]);
			return !i;

		#if DELIM
		case DEL:
		#if DEL2 != DEL
		case DEL2:
		#endif
			/* Match path delimiter char */
			if (fch != DEL  &&  fch != DEL2)
				return false;
			fname++;
			break;
		#endif

		default:
			/* Match a (non-null) char exactly */
			if (lowercase(fch) != lowercase(pch)) {
				debug("fch:'%c' != pch:'%c'", fch, pch);
				return false;
			}
			fname++;
			break;
		}
	}

	/* Check for complete match */
	if (*fname != '\0')
		return false;

	/* Successful match */
	debug("fpattern_submatch: pass");
	return true;
}


/*------------------------------------------------------------------------------
* fpattern_match()
*	Attempts to match pattern 'pat' to filename 'fname'.
*
* Returns
*	True (1) if the filename matches, otherwise false (0).
*
* Caveats
*	If 'fname' is null, false (0) is returned.
*
*	If 'pat' is null, false (0) is returned.
*
*	If 'pat' is empty (""), the only filename it matches is the empty string
*	("").
*
*	If 'fname' is empty, the only pattern that will match it is the empty
*	string ("").
*
*	If 'pat' is not a well-formed pattern, false (0) is returned.
*
*	Upper and lower case letters are treated the same; alphabetic characters
*	are converted to lower case before matching occurs.  Conversion to lower
*	case is dependent upon the current locale setting.
*/

int fpattern_match(const char *pat, const char *fname) {
	int	rc;

	debug("fpattern_match: fname=%p:\"%s\", pat=%p:\"%s\"",
	      fname, fname ? fname : "", pat, pat ? pat : "");

	/* Check args */
	if (fname == nullptr)
		return false;

	if (pat == nullptr)
		return false;

	/* Verify that the pattern is valid, and get its length */
	if (!fpattern_isvalid(pat))
		return false;

	/* Attempt to match pattern against filename */
	if (fname[0] == '\0')
		return (pat[0] == '\0');	/* Special case */
	rc = fpattern_submatch(pat, fname);

	debug("fpattern_match: return %c", "FT"[!!rc]);
	return rc;
}


/*------------------------------------------------------------------------------
* fpattern_matchn()
*	Attempts to match pattern 'pat' to filename 'fname'.
*	This operates like fpattern_match() except that it does not verify that
*	pattern 'pat' is well-formed, assuming that it has been checked by a
*	prior call to fpattern_isvalid().
*
* Returns
*	True (1) if the filename matches, otherwise false (0).
*
* Caveats
*	If 'fname' is null, false (0) is returned.
*
*	If 'pat' is null, false (0) is returned.
*
*	If 'pat' is empty (""), the only filename it matches is the empty ("")
*	string.
*
*	If 'pat' is not a well-formed pattern, unpredictable results may occur.
*
*	Upper and lower case letters are treated the same; alphabetic characters
*	are converted to lower case before matching occurs.  Conversion to lower
*	case is dependent upon the current locale setting.
*
* See also
*	fpattern_match().
*/

int fpattern_matchn(const char *pat, const char *fname) {
	int	rc;

	debug("fpattern_matchn: fname=%p:\"%s\", pat=%p:\"%s\"",
	      fname, fname ? fname : "", pat, pat ? pat : "");

	/* Check args */
	if (fname == nullptr)
		return false;

	if (pat == nullptr)
		return false;

	/* Assume that pattern is well-formed */

	/* Attempt to match pattern against filename */
	rc = fpattern_submatch(pat, fname);

	debug("fpattern_matchn: return %c", "FT"[!!rc]);
	return rc;
}

}  // namespace Fallout2