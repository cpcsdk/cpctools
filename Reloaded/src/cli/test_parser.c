/**
 * \file     test_parser.c
 * \brief    Test program for parser library. 
 * \details  This is a test program with a simple CLI that serves as a demo 
 *           as well.
 * \version  \verbatim $Id: test_parser.c 127 2009-03-29 03:01:37Z henry $ \endverbatim
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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"

/** Zeroize a data structure */
#define BZERO_OUTPUT memset(output, 0, sizeof(output)); output_ptr = output

extern char output[2000], *output_ptr;
extern int interactive;
int num_passed = 0, num_failed =0;

/**
 * Feed a string into the parser (skipping line buffering) 
 */
static void
feed_parser (cparser_t *parser, const char *str)
{
    int m, n;
    m = strlen(str);
    for (n = 0; n < m; n++) {
        cparser_input(parser, str[n], CPARSER_CHAR_REGULAR);
    }
}

/**
 * Update pass/fail counters and display a status string 
 */
static void
update_result (char *got, const char *expected, const char *test)
{
    int failed, n;

    failed = strcmp(got, expected);
    if (failed) {
        for (n = 0; n < strlen(expected); n++) {
            if (got[n] != expected[n]) {
                printf("Index %d: expected=%c  got=%c\n", n, expected[n], 
                       got[n]);
            }
        }
    }
    if (!failed) {
        printf("\nPASS: %s\n", test);
        num_passed++;
    } else {
        printf("\nFAIL: %s\n", test);
        num_failed++;
    }
    fflush(stdout);
}

static void
test_printc (const cparser_t *parser, const char ch)
{
    output_ptr += sprintf(output_ptr, "%c", ch);
}

static void
test_prints (const cparser_t *parser, const char *s)
{
    output_ptr += sprintf(output_ptr, "%s", s);
}

/**
 * \brief    Entry point of the program.
 *
 * \param    argc Number of arguments.
 * \param    argv An array of argument strings.
 *
 * \return   Return 0.
 */
int
main (int argc, char *argv[])
{
    cparser_t parser;
    char *config_file = NULL;
    int ch, debug = 0, n;
    cparser_printc_fn printc_fn;
    cparser_prints_fn prints_fn;
    cparser_result_t rc;

    while (-1 != (ch = getopt(argc, argv, "pic:d"))) {
        switch (ch) {
            case 'p':
                printf("pid = %d\n", getpid());
                break;
            case 'i':
                interactive = 1;
                break;
            case 'c':
                config_file = optarg;
                break;
            case 'd':
                debug = 1;
                break;
        }
    }

    parser.cfg.root = &cparser_root;
    parser.cfg.ch_complete = '\t';
    /* 
     * Instead of making sure the terminal setting of the target and 
     * the host are the same. ch_erase and ch_del both are treated
     * as backspace.
     */
    parser.cfg.ch_erase = '\b';
    parser.cfg.ch_del = 127;
    parser.cfg.ch_help = '?';
    parser.cfg.flags = (debug ? CPARSER_FLAGS_DEBUG : 0);
    strcpy(parser.cfg.prompt, "TEST>> ");
    parser.cfg.fd = STDOUT_FILENO;
    cparser_io_config(&parser);

    if (CPARSER_OK != cparser_init(&parser.cfg, &parser)) {
        printf("Fail to initialize parser.\n");
	return -1;
    }
    if (interactive) {
        if (config_file) {
            (void)cparser_load_cmd(&parser, config_file);
        }
        cparser_run(&parser);
    } else {
        /* Run the scripted tests */
        /* Test command execution without trailing space */
        BZERO_OUTPUT;
        feed_parser(&parser, "show employees\n");
        update_result(output, "No employee in the roster.\n",
                      "command without trailing space");

        /* Test command execution with trailing space */
        BZERO_OUTPUT;
        feed_parser(&parser, "show employees \n");
        update_result(output, "No employee in the roster.\n",
                      "command with trailing space");

        /* Test 'shortest unique keyword' without trailing space  */
        BZERO_OUTPUT;
        feed_parser(&parser, "sh employees a\n");
        update_result(output, "No employee in the roster.\n",
                      "shortest unique keyword without trailing space");

        /* Test 'shortest unique keyword' with trailing space */
        BZERO_OUTPUT;
        feed_parser(&parser, "sh employees a \n");
        update_result(output, "No employee in the roster.\n",
                      "shortest unique keyword with trailing space");

        /* Test command completion */
        BZERO_OUTPUT;
        feed_parser(&parser, "sh\temp\t a\t\n");
        update_result(output, "No employee in the roster.\n",
                      "command completion");

        /* Test cparser_submode_enter() and cparse_submode_exit() */
        feed_parser(&parser, "employee 0x1\n");
        feed_parser(&parser, "name bob\n");
        feed_parser(&parser, "date-of-birth 11 1 1970\n");
        feed_parser(&parser, "height 70\n");
        feed_parser(&parser, "weight 165\n");
        feed_parser(&parser, "exit\n");

        feed_parser(&parser, "employee 0x3\n");
        feed_parser(&parser, "name john\n");
        feed_parser(&parser, "date-of-birth 8 17 1959\n");
        feed_parser(&parser, "height 80\n");
        feed_parser(&parser, "weight 220\n");
        feed_parser(&parser, "exit\n");

        BZERO_OUTPUT;
        feed_parser(&parser, "show employees-by-id 0x0 0x1\n");
        update_result(output, 
                      "bob\n   ID: 0x00000001\n   Height:  70\"   Weight: 165 lbs.\n", 
                      "prefixing commands");
        
        /* Test optional parameters */
        BZERO_OUTPUT;
        feed_parser(&parser, "show employees-by-id 0x0\n");
        update_result(output, 
                      "bob\n   ID: 0x00000001\n   Height:  70\"   Weight: 165 lbs.\n" 
                      "john\n   ID: 0x00000003\n   Height:  80\"   Weight: 220 lbs.\n",
                      "optional parameter");

        /* Test the left arrow key */
        BZERO_OUTPUT;
        feed_parser(&parser, "shaw employees-by-id 0x0");
        for (n = 0; n < 20; n++) {
            rc = cparser_input(&parser, 0, CPARSER_CHAR_LEFT_ARROW);
            if (CPARSER_OK != rc) {
                break;
            }
        }
        if (CPARSER_OK == rc) {
            feed_parser(&parser, "\b\bow\n");
            update_result(output, 
                          "bob\n   ID: 0x00000001\n   Height:  70\"   Weight: 165 lbs.\n" 
                          "john\n   ID: 0x00000003\n   Height:  80\"   Weight: 220 lbs.\n",
                          "left arrow #1");
        } else {
            printf("\nFAIL: left arrow #1\n");
            fflush(stdout);
            num_failed++;
        }

        BZERO_OUTPUT;
        for (n = 0; n < 10; n++) {
            rc = cparser_input(&parser, 0, CPARSER_CHAR_LEFT_ARROW);
            if (CPARSER_OK != rc) {
                break;
            }
        }
        if (CPARSER_OK == rc) {
            feed_parser(&parser, "\n");
            update_result(output, "", "left arrow #2");
        } else {
            printf("\nFAIL: left arrow #2\n");
            fflush(stdout);
            num_failed++;
        }
        
        /* Test right arrow key */
        BZERO_OUTPUT;
        feed_parser(&parser, "shaw employees-by-id 0x0");
        for (n = 0; n < 20; n++) {
            rc = cparser_input(&parser, 0, CPARSER_CHAR_LEFT_ARROW);
            if (CPARSER_OK != rc) {
                break;
            }
        }
        if (CPARSER_OK == rc) {
            feed_parser(&parser, "\b\bow");
            for (n = 0; n < 20; n++) {
                rc = cparser_input(&parser, 0, CPARSER_CHAR_RIGHT_ARROW);
                if (CPARSER_OK != rc) {
                    break;
                }
            }
        } else {
            printf("\nFAIL: left arrow #1\n");
            num_failed++;
        }
        if (CPARSER_OK == rc) {
            feed_parser(&parser, " 0x1\n");
            update_result(output, 
                      "bob\n   ID: 0x00000001\n   Height:  70\"   Weight: 165 lbs.\n", 
                      "right arrow #1");
        }

        BZERO_OUTPUT;
        feed_parser(&parser, "show employees-by-id 0x2");
        for (n = 0; n < 20; n++) {
            rc = cparser_input(&parser, 0, CPARSER_CHAR_LEFT_ARROW);
            if (CPARSER_OK != rc) {
                break;
            }
        }
        if (CPARSER_OK == rc) {
            for (n = 0; n < 5; n++) {
                rc = cparser_input(&parser, 0, CPARSER_CHAR_RIGHT_ARROW);
                if (CPARSER_OK != rc) {
                    break;
                }
            }
        } else {
            printf("\nFAIL: right arrow #2\n");
            num_failed++;
        }
        if (CPARSER_OK == rc) {
            feed_parser(&parser, "\n");
            update_result(output, 
                          "john\n   ID: 0x00000003\n   Height:  80\"   Weight: 220 lbs.\n",
                          "right arrow #2");
        }

        /* Test context-sensitive help */
        /* Save the original I/O functions and put in our own to trap output */
        prints_fn = parser.cfg.prints;
        printc_fn = parser.cfg.printc;
        parser.cfg.printc = test_printc;
        parser.cfg.prints = test_prints;

        BZERO_OUTPUT;
        feed_parser(&parser, "s?");
        update_result(output, "s\nshow\nsave\nTEST>> s", 
                      "context-sensitive help #1");

        /* Reset the output buffer */
        feed_parser(&parser, "\n");
        BZERO_OUTPUT;

        feed_parser(&parser, "s\t");
        update_result(output, "s\nshow\nsave\nTEST>> s", 
                      "context-sensitive help #2");

        /* Test incomplete commands */
        feed_parser(&parser, "\n"); /* flush out the last incomplete command */
        BZERO_OUTPUT;
        feed_parser(&parser, "s\n");
        update_result(output, "s\n        ^Incomplete command\n\nTEST>> ",
                      "Incomplete command #1");

        BZERO_OUTPUT;
        feed_parser(&parser, "show \n");
        update_result(output, 
                      "show \n           ^Incomplete command\n\nTEST>> ",
                      "Incomplete command #2");

        BZERO_OUTPUT;
        feed_parser(&parser, "show em\n");
        update_result(output, 
                      "show em\n              ^Incomplete command\n\nTEST>> ",
                      "Incomplete command #3");

        /* Test invalid commands */
        feed_parser(&parser, "\n"); /* flush out the last incomplete command */
        BZERO_OUTPUT;
        feed_parser(&parser, "xyz\n");
        update_result(output, "xyz\n       ^Parse error\nTEST>> ",
                      "Invalid command #1");

        BZERO_OUTPUT;
        feed_parser(&parser, "show xyz\n");
        update_result(output, "show xyz\n            ^Parse error\nTEST>> ",
                      "Invalid command #2");
        
        /* 
         * Test cparser_help_cmd() with and without a filter string.
         * This implicitly tests cparser_walk() as well.
         */
        BZERO_OUTPUT;
        feed_parser(&parser, "help\n");
        update_result(output, "help\n"
                      "List a summary of employees.\r\n  show employees \r\n\n"
                      "List detail records of all employees.\r\n  show employees all \r\n\n"
                      "List all employees within a certain range of employee ids\r\n  show employees-by-id <UINT:min> { <UINT:max> } \r\n\n"
                      "Add a new employee or enter the record of an existing employee\r\n  employee <HEX:id> \r\n\n"
                      "Delete an existing employee\r\n  no employee <HEX:id> \r\n\n"
                      "Save the current roster to a file\r\n  save roster <STRING:filename> \r\n\n"
                      "Load roster file\r\n  load roster <FILE:filename> \r\n\n"
                      "List all available commands with a substring 'filter' in it.\r\n  help { <STRING:filter> } \r\n\n"
                      "Leave the database\r\n  quit \r\n\n"
                      "TEST>> ",
                      "help summary #1");

        BZERO_OUTPUT;
        feed_parser(&parser, "help roster\n");
        update_result(output, "help roster\n"
                      "Save the current roster to a file\r\n  save roster <STRING:filename> \r\n\n"
                      "Load roster file\r\n  load roster <FILE:filename> \r\n\n"
                      "TEST>> ",
                      "help summary #2");

        printf("Total=%d  Passed=%d  Failed=%d\n", num_passed + num_failed,
               num_passed, num_failed);
    }
    return 0;
}
