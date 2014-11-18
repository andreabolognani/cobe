/* phone-spelling - Convert text to a sequence of key presses
 * Copyright (C) 2014  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

/* Limits for valid input */
#define SPACE        32
#define LOWER_A      97
#define LOWER_Z      122
#define ALPHABET_LEN (LOWER_Z - LOWER_A + 2)

char *PRESSES[] = {
	"2", "22", "222",         /* abc  */
	"3", "33", "333",         /* def  */
	"4", "44", "444",         /* ghi  */
	"5", "55", "555",         /* jkl  */
	"6", "66", "666",         /* mno  */
	"7", "77", "777", "7777", /* pqrs */
	"8", "88", "888",         /* tuv  */
	"9", "99", "999", "9999", /* wxyz */
	"0"                       /*      */
};

void
convert (char *text)
{
	int prev;
	int cur;
	int len;
	int i;

	len = strlen (text);

	for (i = 0; i < len; i++)
	{
		if (text[i] >= LOWER_A && text[i] <= LOWER_Z)
		{
			/* Lowercase letter */
			cur = text[i] - LOWER_A;
		}
		else if (text[i] == SPACE)
		{
			/* Space */
			cur = ALPHABET_LEN - 1;
		}
		else
		{
			/* Anything else is ignored */
			continue;
		}

		/* Look at the first key press in the sequence to detect
		 * whether a pause should be inserted.
		 * Skip this check the very first time */
		if (i > 0 && PRESSES[cur][0] == PRESSES[prev][0])
		{
			printf (" ");
		}

		/* Print the key press sequence */
		printf ("%s", PRESSES[cur]);

		prev = cur;
	}

	printf ("\n");
}

char*
load (char *infile)
{
	char *text;
	char  buffer[BUFFER_SIZE];
	int   len;
	int   count;
	int   fd;
	int   i;

	text = NULL;
	len = 0;
	i = 0;

	/* Open file for input */
	fd = open (infile, O_RDONLY);

	if (fd < 0)
	{
		return NULL;
	}

	while (1)
	{
		count = read (fd, buffer, BUFFER_SIZE);

		/* End of input reached */
		if (count == 0)
		{
			if (text != NULL)
			{
				text[i] = '\0';
			}
			break;
		}

		/* Extend the buffer */
		len = len + count;
		text = realloc (text, len * sizeof (char));

		/* Copy the input and move the cursor */
		memcpy (text + (i * sizeof (char)), buffer, count);
		i += count;
	}

	close (fd);

	return text;
}

int
main (int    argc,
      char **argv)
{
	char *text;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}

	text = load (argv[1]);

	if (text == NULL)
	{
		fprintf (stderr, "%s: Error reading '%s'\n", argv[0], argv[1]);
		return 1;
	}

	convert (text);

	free (text);

	return 0;
}
