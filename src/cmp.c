/*
 * Copyright (c) 1987, 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Modified by Yuri Baranov, Feb 2001. OS/2 distribution.
 * Purpose of modification: to make sure c_regular() is never called
 * c_regular() version from FreeBSD distribution uses memory-mapped
 * files and is not portable. In this distribution it is replaced by
 * dummy function just for linkability.
 *
 * All copyright/disclaimers notices apply as above
 */
#ifndef lint
static const char copyright[] =
"@(#) Copyright (c) 1987, 1990, 1993, 1994\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static const char sccsid[] = "@(#)cmp.c	8.3 (Berkeley) 4/2/94";
#endif /* not lint */

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "extern.h"

int	lflag, sflag;

static void usage __P((void));

int
main(argc, argv)
	int argc;
	char *argv[];
{
	struct stat sb1, sb2;
	off_t skip1, skip2;
	int ch, fd1, fd2, special;
	char *file1, *file2;

	while ((ch = getopt(argc, argv, "-ls")) != -1)
		switch (ch) {
		case 'l':		/* print all differences */
			lflag = 1;
			break;
		case 's':		/* silent run */
			sflag = 1;
			break;
		case '-':		/* stdin (must be after options) */
			--optind;
			goto endargs;
		case '?':
		default:
			usage();
		}
endargs:
	argv += optind;
	argc -= optind;

	if (lflag && sflag)
		errx(ERR_EXIT, "only one of -l and -s may be specified");

	if (argc < 2 || argc > 4)
		usage();

	/* Backward compatibility -- handle "-" meaning stdin. */
	special = 1; /*   modification for OS/2*/
	if (strcmp(file1 = argv[0], "-") == 0) {
		special = 1;
		fd1 = 0;
		file1 = "stdin";
	}
	else if ((fd1 = open(file1, O_RDONLY, 0)) < 0) {
		if (!sflag)
			err(ERR_EXIT, "%s", file1);
		else
			exit(1);
	}
	if (strcmp(file2 = argv[1], "-") == 0) {
		if (special)
			errx(ERR_EXIT,
				"standard input may only be specified once");
		special = 1;
		fd2 = 0;
		file2 = "stdin";
	}
	else if ((fd2 = open(file2, O_RDONLY, 0)) < 0) {
		if (!sflag)
			err(ERR_EXIT, "%s", file2);
		else
			exit(1);
	}

	skip1 = argc > 2 ? strtol(argv[2], NULL, 0) : 0;
	skip2 = argc == 4 ? strtol(argv[3], NULL, 0) : 0;

	if (!special) {
		if (fstat(fd1, &sb1)) {
			if (!sflag)
				err(ERR_EXIT, "%s", file1);
			else
				exit(1);
		}
		if (!S_ISREG(sb1.st_mode))
			special = 1;
		else {
			if (fstat(fd2, &sb2)) {
				if (!sflag)
					err(ERR_EXIT, "%s", file2);
				else
					exit(1);
			}
			if (!S_ISREG(sb2.st_mode))
				special = 1;
		}
	}

	if (special)
		c_special(fd1, file1, skip1, fd2, file2, skip2);
	else
		c_regular(fd1, file1, skip1, sb1.st_size,
		    fd2, file2, skip2, sb2.st_size);
	exit(0);
}

static void
usage()
{

	(void)fprintf(stderr,
	    "usage: cmp [-l | -s] file1 file2 [skip1 [skip2]]\n");
	exit(ERR_EXIT);
}
