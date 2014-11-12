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
#include <string.h>

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

int
main (int    argc,
      char **argv)
{
	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s TEXT\n", argv[0]);
		return 1;
	}

	convert (argv[1]);

	return 0;
}
