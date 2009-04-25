/**
 * \file     test_fsm.c
 * \brief    Test program for verifying parser FSM.
 * \details  There are three states - WHITESPACE, TOKEN, ERROR and three 
 *           types of inputs CHARACTER, ERASE and SPACE. The following 
 *           is a complete list of state transition.
 *
 *            1. (WHITESPACE, SPACE) -> WHITESPACE
 *            2. (WHITESPACE, ERASE) -> WHITESPACE
 *            3. (WHITESPACE, ERASE) -> TOKEN
 *            4. (WHITESPACE, CHAR)  -> CHAR
 *            5. (WHITESPACE, CHAR)  -> ERROR
 *            6. (TOKEN,      SPACE) -> WHITESPACE
 *            7. (TOKEN,      SPACE) -> ERROR
 *            8. (TOKEN,      CHAR)  -> TOKEN
 *            9. (TOKEN,      CHAR)  -> ERROR
 *           10. (TOKEN,      ERASE) -> TOKEN
 *           11. (TOKEN,      ERASE) -> WHITESPACE
 *           12. (ERROR,      SPACE) -> ERROR
 *           13. (ERROR,      CHAR)  -> ERROR
 *           14. (ERROR,      ERASE) -> ERROR
 *           15. (ERROR,      ERASE) -> WHITESPACE
 *           16. (ERROR,      ERASE) -> TOKEN
 *
 * \version  \verbatim $Id: test_fsm.c 115 2009-03-27 09:34:52Z henry $ \endverbatim
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
#include "cparser_fsm.h"
#include "cparser_tree.h"
#include "cparser_io.h"

/** 
 * \brief    Function pointer for a test.
 */
typedef int (*test_fn_t)(void);

/**
 * Initialize the parser. This should be the first function called by
 * every test.
 *
 * \param   parser Pointer to the parser structure to be initialized.
 */
static void
test_init_parser (cparser_t *parser)
{
    cparser_result_t rc;

    memset(parser, 0, sizeof(*parser));
    parser->cfg.root = &cparser_root;
    parser->cfg.ch_complete = '\t';
    parser->cfg.ch_erase = '\b';
    parser->cfg.ch_help = '?';
    parser->cfg.flags = 0;
    cparser_io_config(parser);
    strcpy(parser->cfg.prompt, "TEST>> ");
    parser->cfg.fd = STDOUT_FILENO;
    rc = cparser_init(&parser->cfg, parser);
    assert(CPARSER_OK == rc);
}

/**
 * Feed a test string into the parser FSM character by character.
 *
 * \param    parser Pointer to the parser structure to be initialized.
 * \param    input  Pointer to a string of user input.
 *
 * \return   1 if succeeded; 0 if failed.
 */
static int 
test_input (cparser_t *parser, char *input)
{
    cparser_result_t rc;
    int n;

    assert(parser && input);
    for (n = 0; n < strlen(input); n++) {
        rc = cparser_fsm_input(parser, input[n]);
        if (CPARSER_OK != rc) {
            printf("ERROR: Fail to input %d-th character.\n", n);
            return 0;
        }
    }
    return 1;
}

int
test_state_transition (const char *input, const cparser_state_t final_state,
                       const short token_tos, const cparser_token_t *tokens,
                       const short current_pos, const short last_good)
{
    cparser_t parser;
    int n;

    test_init_parser(&parser);

    if (CPARSER_STATE_WHITESPACE != parser.state) {
        return 0;
    }
    if (!test_input(&parser, (char *)input)) {
        return 0;
    }
    if (final_state != parser.state) {
        printf("ERROR: Unexpected state. (expected:%d  got:%d)\n",
               final_state, parser.state);
        return 0;
    }
    /* Check against expected token stack */
    if (token_tos != parser.token_tos) {
        printf("ERROR: Unexpected token stack size. (expected:%d  got:%d)\n",
               token_tos, parser.token_tos);
        return 0;
    }
    for (n = 0; n <= token_tos; n++) {
        if (tokens[n].token_len != parser.tokens[n].token_len) {
            printf("ERROR: Token %d length mismatch.\n", n);
            return 0;
        }
        if (tokens[n].begin_ptr != parser.tokens[n].begin_ptr) {
            printf("ERROR: Token %d begin pointer mismatch.\n", n);
            return 0;
        }
        if (tokens[n].parent != parser.tokens[n].parent) {
            printf("ERROR: Token %d parent pointer mismatch.\n", n);
            return 0;
        }
        if (tokens[n].token_len && 
            strcmp(tokens[n].buf, parser.tokens[n].buf)) {
            printf("ERROR: Token %d buffer mismatch.\n", n);
            return 0;
        }
    }
    if (current_pos != parser.current_pos) {
        printf("ERROR: Current position is incorrect (expect:%d  got:%d).\n", 
               current_pos, parser.current_pos);
        return 0;
    }
    if (last_good != parser.last_good) {
        printf("ERROR: Last good position is incorrect (expect:%d  got:%d).\n", 
               last_good, parser.last_good);
        return 0;
    }
    return 1;
}

/**
 * Get a SPACE in WHITESPACE state. We should end in WHITESPACE and
 * the token stack should be completely empty.
 */ 
static int 
test_whitespace_space_whitespace (void)
{
    cparser_token_t token = { -1, 0, "", NULL };
    return test_state_transition("     ", CPARSER_STATE_WHITESPACE, 0, 
                                 &token, 5, 4);
}

/**
 * Get a character that is part of a command in WHITESPACE state. 
 * We should end up in TOKEN. There should be one partially complete token.
 */
int
test_whitespace_char_token (void)
{
    cparser_token_t token = { 1, 1, "s", NULL };
    return test_state_transition(" s", CPARSER_STATE_TOKEN, 0, 
                                 &token, 2, 1);
}

/**
 * Get characters from characters of a valid command in TOKEN state. 
 * We should end up in TOKEN again. There should be one partially 
 * complete token.
 */
int 
test_token_char_token (void)
{
    cparser_token_t token = { 2, 4, "show", NULL };
    return test_state_transition("  show", CPARSER_STATE_TOKEN, 0,
                                 &token, 6, 5);
}

/**
 * Get a SPACE in TOKEN with a unique matched keyword. We should end 
 * up in WHITESPACE again. There should be one complete token.
 */
int
test_token_space_whitespace (void)
{
    cparser_token_t tokens[2] = { { 0, 4, "show", &cparser_root },
                                  { -1, 0, "", NULL } };
    return test_state_transition("show ", CPARSER_STATE_WHITESPACE, 1, 
                                 tokens, 5, 4);
}

/**
 * Get a character that does not match any commands in WHITEPSACE. We should
 * end up in ERROR. Token stack should be empty.
 */
int 
test_whitespace_char_error (void)
{
    cparser_token_t token = { -1, 0, "", NULL };
    return test_state_transition("z", CPARSER_STATE_ERROR, 0,
                                 &token, 1, -1);
}

/**
 * Get an invalid character in TOKEN. We should end up in ERROR. Token 
 * stack should have a partial token.
 */
int
test_token_char_error (void)
{
    cparser_token_t token = { 0, 1, "s", NULL };
    return test_state_transition("sz", CPARSER_STATE_ERROR, 0, &token, 2, 0);
}

/**
 * Get a space in TOKEN that results in multiple matches. Token should
 * have a partial token.
 */
int
test_token_space_error (void)
{
    cparser_token_t token = { 0, 1, "s", NULL };
    return test_state_transition("s ", CPARSER_STATE_ERROR, 0, &token, 2, 0);
}

/**
 * Get a SPACE in ERROR. Token stack should be empty. We should stay in ERROR.
 */
int
test_error_space_error (void)
{
    cparser_token_t token = { -1, 0, "", NULL };
    return test_state_transition("z ", CPARSER_STATE_ERROR, 0, &token, 2, -1);
}

/**
 * Get a character in ERROR. Token stack should be empty. We shuld stay in ERROR.
 */
int 
test_error_char_error (void)
{
    cparser_token_t token = { -1, 0, "", NULL };
    return test_state_transition("xyz", CPARSER_STATE_ERROR, 0, &token, 3, -1);
}

/**
 * Insert 2 spaces and erase one. Token stack should be empty.
 */
int 
test_whitespace_erase_whitespace (void)
{
    cparser_token_t token = { -1, 0, "", NULL };
    return test_state_transition("  \b", CPARSER_STATE_WHITESPACE, 0, &token, 1, 0);
}

/**
 * Insert a valid token (CHARs) and one SPACE and then erase one SPACE.
 * Token stack should have one incomplete token.
 */
int 
test_whitespace_erase_token (void)
{
    cparser_token_t token = { 0, 4, "show", NULL };
    return test_state_transition("show \b", CPARSER_STATE_TOKEN, 0, &token, 4, 3);
}

/**
 * Insert a valid token and erase one CHAR. Token should have one 
 * incomplete token.
 */
int
test_token_erase_token (void)
{
    cparser_token_t token = { 0, 3, "sho", NULL };
    return test_state_transition("show\b", CPARSER_STATE_TOKEN, 0, &token, 3, 2);
}

/**
 * Insert a valid token, one SPACE then one character of next valid token,
 * and then erase it. Token stack should have one complete and one incomplete
 * tokens.
 */
int
test_token_erase_whitespace (void)
{
    cparser_token_t tokens[2] = { { 0, 4, "show", &cparser_root },
                                  { -1, 0, "", NULL } };
    return test_state_transition("show e\b", CPARSER_STATE_WHITESPACE, 1, tokens, 5, 4);
}

int
test_error_erase_error (void)
{
    cparser_token_t token = { -1, 0, "", NULL };
    return test_state_transition("xyz\b", CPARSER_STATE_ERROR, 0, &token, 2, -1);
}

int
test_error_erase_whitespace (void)
{
    cparser_token_t token1 = { -1, 0, "", NULL };
    cparser_token_t token2[2] = { { 0, 4, "show", &cparser_root },
                                  { -1, 0, "", NULL } };
    if (!test_state_transition("x\b", CPARSER_STATE_WHITESPACE, 0, &token1, 0, -1)) {
        return 0;
    }
    return test_state_transition("show x\b", CPARSER_STATE_WHITESPACE, 1, token2, 5, 4);
}

int 
test_error_erase_token (void)
{
    cparser_token_t token = { 0, 2, "sh", NULL };
    return test_state_transition("shx\b", CPARSER_STATE_TOKEN, 0, &token, 2, 1);
}

int
main (int argc, char *argv[])
{
    int n, rc, num_passed = 0, num_failed = 0;
    struct {
        char      *name;
        test_fn_t test_fn;
    } testcases[] = {
        /* These are the normal transition for accepting a valid command */
        { "(WHITESPACE, SPACE) -> WHITESPACE", test_whitespace_space_whitespace },
        { "(WHITESPACE, CHAR)  -> TOKEN", test_whitespace_char_token },
        { "(TOKEN,      CHAR)  -> TOKEN", test_token_char_token },
        { "(TOKEN,      SPACE) -> WHITESPACE", test_token_space_whitespace },

        /* These are transitions that lead to errors */
        { "(WHITESPACE, CHAR)  -> ERROR", test_whitespace_char_error },
        { "(TOKEN,      CHAR)  -> ERROR", test_token_char_error },
        { "(TOKEN,      SPACE) -> ERROR", test_token_space_error },
        { "(ERROR,      SPACE) -> ERROR", test_error_space_error },
        { "(ERROR,      CHAR)  -> ERROR", test_error_char_error },
        
        /* These are transitions involved in erase */
        { "(WHITESPACE, ERASE) -> WHITESPACE", test_whitespace_erase_whitespace },
        { "(WHITESPACE, ERASE) -> TOKEN", test_whitespace_erase_token },
        { "(TOKEN,      ERASE) -> TOKEN", test_token_erase_token },
        { "(TOKEN,      ERASE) -> WHITESPACE", test_token_erase_whitespace },
        { "(ERROR,      ERASE) -> ERROR", test_error_erase_error },
        { "(ERROR,      ERASE) -> WHITESPACE", test_error_erase_whitespace },
        { "(ERROR,      ERASE) -> TOKEN", test_error_erase_token }
    };
    for (n = 0; n < (sizeof(testcases)/sizeof(testcases[0])); n++) {
        rc = testcases[n].test_fn();
        printf("%s: %s\n", rc ? "PASS" : "FAIL", testcases[n].name);
        if (rc) {
            num_passed++;
        } else {
            num_failed++;
        }
    }

    printf("Total=%d  Passed=%d  Failed=%d\n", num_passed + num_failed,
           num_passed, num_failed);
    return 0;
}
