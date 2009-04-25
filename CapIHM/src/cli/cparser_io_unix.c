/**
 * \file     cparser_io_unix.c
 * \brief    Unix-specific parser I/O routines
 * \version  \verbatim $Id: cparser_io_unix.c 120 2009-03-29 00:02:21Z henry $ \endverbatim
 */
/*
 * Copyright (c) 2008, Henry Kwok
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the project nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY HENRY KWOK ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL HENRY KWOK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "cparser.h"
#include "cparser_io.h"
#include "cparser_priv.h"

/**
 * \brief    Enable/disable canonical mode.
 * \details  Note that this call must be made first with enable=0.
 *
 * \param    parser Pointer to the parser.
 * \param    enable 1 to enable; 0 to disable.
 */
static void
cparser_term_set_canonical (cparser_t *parser, int enable)
{
    static struct termios old_term;
    static int old_term_set = 0;
    struct termios new_term;

    if (!VALID_PARSER(parser)) return;

    if (enable) {
        assert(old_term_set);
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        old_term_set = 0;
    } else {
        assert(0 == STDIN_FILENO);
        tcgetattr(STDIN_FILENO, &old_term);
        new_term = old_term;
        new_term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
        old_term_set = 1;
    }
}

static void
cparser_unix_getch (cparser_t *parser, int *ch, cparser_char_t *type)
{
    assert(VALID_PARSER(parser) && ch && type);
    *type = CPARSER_CHAR_UNKNOWN;
    *ch = getchar();
    if ('' == *ch) {
        *ch = getchar();
        if ('[' == *ch) {
            *ch = getchar();
            switch (*ch) {
                case 'A':
                    *type = CPARSER_CHAR_UP_ARROW;
                    break;
                case 'B':
                    *type = CPARSER_CHAR_DOWN_ARROW;
                    break;
                case 'C':
                    *type = CPARSER_CHAR_RIGHT_ARROW;
                    break;
                case 'D':
                    *type = CPARSER_CHAR_LEFT_ARROW;
                    break;
            }
        }
    } else if (isalnum(*ch) || ('\n' == *ch) ||
               ispunct(*ch) || (' ' == *ch) ||
               (*ch == parser->cfg.ch_erase) ||
               (*ch == parser->cfg.ch_del) ||
               (*ch == parser->cfg.ch_help) ||
               (*ch == parser->cfg.ch_complete)) {
        *type = CPARSER_CHAR_REGULAR;
    }
}

static void
cparser_unix_printc (const cparser_t *parser, const char ch)
{
    ssize_t wsize;
    assert(parser);
    wsize = write(parser->cfg.fd, &ch, 1);
}

static void
cparser_unix_prints (const cparser_t *parser, const char *s)
{
    ssize_t wsize;
    assert(parser && s);
    wsize = write(parser->cfg.fd, s, strlen(s));
}

static void 
cparser_unix_io_init (cparser_t *parser)
{
    cparser_term_set_canonical(parser, 0);
}

static void
cparser_unix_io_cleanup (cparser_t *parser)
{
    cparser_term_set_canonical(parser, 1);
}

void
cparser_io_config (cparser_t *parser)
{
    assert(parser);
    parser->cfg.io_init    = cparser_unix_io_init;
    parser->cfg.io_cleanup = cparser_unix_io_cleanup;
    parser->cfg.getch      = cparser_unix_getch;
    parser->cfg.printc     = cparser_unix_printc;
    parser->cfg.prints     = cparser_unix_prints;
}
