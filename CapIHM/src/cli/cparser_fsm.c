/**
 * \file     cparser_fsm.c
 * \brief    Parser state machine implementation.
 * \version  \verbatim $Id: cparser_fsm.c 120 2009-03-29 00:02:21Z henry $ \endverbatim
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
#include <string.h>
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_fsm.h"

#define INSERT_TOK_STK(t, ch)                           \
    (t)->buf[(t)->token_len] = (ch); (t)->token_len++
#define DELETE_TOK_STK(t)                                               \
    (t)->token_len = ((t)->token_len ? (t)->token_len - 1 : 0) ;        \
    (t)->buf[(t)->token_len] = '\0';

int
cparser_match (const char *token, const int token_len, cparser_node_t *parent,
               cparser_node_t **match, int *is_complete)
{
    int num_matches = 0, local_is_complete;
    cparser_node_t *child;
    cparser_result_t rc;

    assert(token && parent && match && is_complete);
    *match = NULL;
    *is_complete = 0;
    for (child = parent->children; NULL != child; child = child->sibling) {
	local_is_complete = 0;
        rc = cparser_match_fn_tbl[child->type](token, token_len, child, 
                                               &local_is_complete);
        if (CPARSER_OK == rc) {
            num_matches++;
            /* 
             * Return only the highest priority match unless the lower 
             * priority match is complete and the higher one is only 
             * partially matched.
             */
            if (!(*match)) {
                /* Return only the highest priority match */
                *match = child;
                *is_complete = local_is_complete;
            } else {
                if (!(*is_complete) && local_is_complete) {
                    *match = child;
                    *is_complete = local_is_complete;
                }
            }
        }
    }

#ifdef SHORTEST_UNIQUE_KEYWORD
    /* 
     * There is a only one match and it is a keyword. Consider this a 
     * complete match.
     */
    if ((1 == num_matches) && (CPARSER_NODE_KEYWORD == (*match)->type)) {
        *is_complete = 1;
    }
#endif /* SHORTEST_UNIQUE_KEYWORD */
    return num_matches;
}

/**
 * Reset the token stack in parser FSM.
 *
 * \param    parser Pointer to the parser structure.
 *
 * \return   None.
 */
static void
cparser_token_stack_reset (cparser_t *parser)
{
    int n;
    cparser_token_t *token;

    assert(VALID_PARSER(parser));
    parser->last_good   = -1;
    parser->current_pos = 0;
    parser->token_tos   = 0;
    for (n = 0; n < CPARSER_MAX_NUM_TOKENS; n++) {
        token = &parser->tokens[n];
        token->begin_ptr = -1;
        token->token_len = 0;
        token->parent    = NULL;
        memset(token->buf, 0, sizeof(token->buf));
    }
}

/**
 * Process a BS in WHITESPACE state. 
 *
 * \details  There are two possibilities: 1) after erasing a character, 
 *           we are in the end of a token -> erase character/TOKEN, 
 *           2) after erasing a character, the previous character is 
 *           still a SPC -> erase character/WHITESPACE.
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [BS].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_ws_erase (cparser_t *parser, const char ch, int *ch_processed)
{
    cparser_token_t *token;

    assert(parser && ch_processed);
    if (parser->current_pos > 0) {
	*ch_processed = 1;
        parser->current_pos--;
        if (0 < parser->token_tos) {
            token = &parser->tokens[parser->token_tos-1];
            if (token->begin_ptr + token->token_len >= parser->current_pos) {
                parser->cur_node = token->parent;
                token->parent    = NULL;

                /* Pop the token on top of the stack */
                token = CUR_TOKEN(parser);
                token->begin_ptr = -1;
                token->token_len = 0;
                token->parent    = NULL;
                token->buf[0]    = '\0';
                parser->token_tos--;
                return CPARSER_STATE_TOKEN;
            }
	}
    } else {
	*ch_processed = 0;
    }
    return CPARSER_STATE_WHITESPACE;
}

/**
 * Process a SPC in WHITESPACE state.
 *
 * \details  There are one possibility: 1) there is space left -> insert 
 *           SPC/WHITESPACE.
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_ws_space (cparser_t *parser, const char ch, int *ch_processed)
{
    assert(parser && ch_processed);
    *ch_processed = 1;
    return CPARSER_STATE_WHITESPACE;
}

/**
 * Process a character in WHITESPACE state.
 *
 * \details  There are two possibilities: 1) the new character is a valid 
 *           token -> insert char/TOKEN, 2) the new character is an invalid 
 *           token -> insert char/ERROR.
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_ws_char (cparser_t *parser, const char ch, int *ch_processed)
{
    cparser_node_t *match;
    int is_complete;
    cparser_token_t *token;

    assert(parser && ch_processed);
    *ch_processed = 1;

    if (!cparser_match(&ch, 1, parser->cur_node, &match, &is_complete)) {
	return CPARSER_STATE_ERROR; /* no token match */
    }

    token = CUR_TOKEN(parser);
    token->begin_ptr = parser->current_pos;

    /* A valid token found. Add to token stack */
    INSERT_TOK_STK(token, ch);
    return CPARSER_STATE_TOKEN;
}

/**
 * Process a BS in TOKEN state.
 *
 * \details  There are two possibilities: 1) This is not the last character
 *           of the token -> delete character/TOKEN, 2) This is the last
 *           character of the token -> delete character/WHITESPACE.
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_tok_erase (cparser_t *parser, const char ch, int *ch_processed)
{
    cparser_token_t *token;

    assert(parser && ch_processed);
    token = CUR_TOKEN(parser);
    DELETE_TOK_STK(token);
    parser->current_pos--;
    *ch_processed = 1;
    if (!token->token_len) {
        token->begin_ptr = -1;
	return CPARSER_STATE_WHITESPACE;
    }
    return CPARSER_STATE_TOKEN;
}

/**
 * Process a SPC in TOKEN state.
 *
 * \details  There are two possibilities: 1) The token was unique and 
 *           complete -> insert character, update cur_node/WHITESPACE,
 *           2) Otherwise -> insert character/ERROR.
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_tok_space (cparser_t *parser, const char ch, int *ch_processed)
{
    cparser_node_t *match;
    int is_complete;
    cparser_token_t *token;

    assert(parser && (' ' == ch) && ch_processed);
    *ch_processed = 1;
    token = CUR_TOKEN(parser);
    if ((1 <= cparser_match(token->buf, token->token_len, 
                            parser->cur_node, &match, &is_complete)) && 
	(is_complete)) {
        /* Save the parent node for this token and "close" the token */
        token->parent = parser->cur_node;
	token->buf[token->token_len] = '\0';

        /* Push it into the stack */
	parser->token_tos++;
	assert(CPARSER_MAX_NUM_TOKENS > parser->token_tos);
        token = CUR_TOKEN(parser);
        assert(-1 == token->begin_ptr);
        assert(0 == token->token_len);
        assert(NULL == token->parent);

        parser->cur_node = match;

	return CPARSER_STATE_WHITESPACE;
    }
    return CPARSER_STATE_ERROR;
}

/**
 * Process a character in TOKEN state.
 *
 * \details  There are three possibilities: 1) there are too many
 *           characters in the token already -> do nothing/
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_tok_char (cparser_t *parser, const char ch, int *ch_processed)
{
    cparser_node_t *match;
    int is_complete;
    cparser_token_t *token;

    assert(parser && ch_processed);
    *ch_processed = 1;

    token = CUR_TOKEN(parser);
    if (token->token_len < CPARSER_MAX_TOKEN_SIZE) {
        INSERT_TOK_STK(token, ch);
    } else {
        return CPARSER_STATE_ERROR;
    }
    if (!cparser_match(token->buf, token->token_len, parser->cur_node, 
                       &match, &is_complete)) {
        DELETE_TOK_STK(token);
        return CPARSER_STATE_ERROR;
    }

    return CPARSER_STATE_TOKEN;
}

/**
 * Process a BS in ERROR state.
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_err_erase (cparser_t *parser, const char ch, int *ch_processed)
{
    assert(parser && ch_processed);
    *ch_processed = 1;
    assert(0 < parser->current_pos); /* impossible to get to error state 
                                      * on an empty string.
                                      */
    parser->current_pos--;
    if ((parser->last_good + 1) == parser->current_pos) {
        cparser_token_t *token;
        token = CUR_TOKEN(parser);
        if (token->begin_ptr + token->token_len >= parser->current_pos) {
            return CPARSER_STATE_TOKEN;
        }
        return CPARSER_STATE_WHITESPACE;
    }
    return CPARSER_STATE_ERROR;
}

/**
 * Process a SPC in ERROR state.
 *
 * \details  There is one possibility: 1) -> insert SPC/ERROR
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_err_space (cparser_t *parser, const char ch, int *ch_processed)
{
    assert(parser && ch_processed);
    *ch_processed = 1;
    return CPARSER_STATE_ERROR;
}

/**
 * Process a character in ERROR state.
 *
 * \details  There is one possibility: 1) for any characters
 *           -> insert character/ERROR
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Character to be input which must [SPC].
 *
 * \retval   ch_processed 1 if the character is used; 0 if the character 
 *                        is rejected by the parser FSM.
 * \return   New parser state.
 */
static cparser_state_t
cparser_err_char (cparser_t *parser, const char ch, int *ch_processed)
{
    assert(parser && ch_processed);
    *ch_processed = 1;
    return CPARSER_STATE_ERROR;
}

/* Define a table of function based on the (state, input type) */
typedef cparser_state_t (*cparser_state_func)(cparser_t *parser, char ch, int *ch_processed);
cparser_state_func cparser_state_func_tbl[CPARSER_MAX_STATES][3] = {
    { cparser_ws_erase,  cparser_ws_space,  cparser_ws_char }, 
    { cparser_tok_erase, cparser_tok_space, cparser_tok_char }, 
    { cparser_err_erase, cparser_err_space, cparser_err_char } };

cparser_result_t 
cparser_fsm_input (cparser_t *parser, char ch)
{
    int input_type, ch_processed, n, m;

    assert((uint32_t)parser->state < CPARSER_MAX_STATES);
    /*
     * We classify the input into one of 3 classes: backspace (BS),
     * whitespace (SPC), regular characters (CHAR). We also check
     * if the buffer is empty when erasing or the buffer is full 
     * when inserting. A more systematic approach is to check
     * this in 9 state-input functions. But checking here reduces
     * the amount of code by a good amount.
     */
    if ((parser->cfg.ch_erase == ch) || (parser->cfg.ch_del == ch)) {
        /* 
         * Line buffer code already checks that there is character 
         * in token stack
         */
        assert(0 < parser->current_pos);
	input_type = 0;
    } else {
	if (parser->current_pos >= (CPARSER_MAX_LINE_SIZE-1)) {
	    parser->cfg.printc(parser, '\a');
	    return CPARSER_OK;
	}
	if (' ' == ch) {
	    input_type = 1;
	} else {
	    input_type = 2;
	}
    }
    ch_processed = 0;
    parser->state = 
	cparser_state_func_tbl[parser->state][input_type](parser, ch, 
                                                          &ch_processed);

    if (ch_processed) {
        if (0 != input_type) {
            parser->current_pos++;
        }
        if (CPARSER_STATE_ERROR != parser->state) {
            parser->last_good = parser->current_pos - 1;
        }
    }

    if (parser->cfg.flags & CPARSER_FLAGS_DEBUG) {
        parser->cfg.printc(parser, '\n');

        /* Print out the state */
        switch (parser->state) {
            case CPARSER_STATE_WHITESPACE:
            {
                parser->cfg.prints(parser, "State: WHITESPACE\n");
                break;
            }
            case CPARSER_STATE_TOKEN:
            {
                parser->cfg.prints(parser, "State: TOKEN\n");
                break;
            }
            case CPARSER_STATE_ERROR:
            {
                parser->cfg.prints(parser, "State: ERROR\n");
                break;
            }
            default:
                parser->cfg.prints(parser, "State: UNKNOWN\n");
        }

        /* Print out the parser internal buffer and token stack */
        for (n = 0; n < parser->current_pos; n++) {
            parser->cfg.printc(parser, cparser_line_char(parser, n));
        }
        parser->cfg.printc(parser, '\n');

        for (n = 0; n <= parser->token_tos; n++) {
            parser->cfg.printc(parser, '[');
            for (m = 0; m <= parser->tokens[n].token_len; m++) {
                parser->cfg.printc(parser, parser->tokens[n].buf[m]);
            }
            parser->cfg.printc(parser, ']');
            parser->cfg.printc(parser, '\n');
        }

        /* Print the line buffer again */
        cparser_line_print(parser, 1, 1);
    }
    return CPARSER_OK;
}

void
cparser_fsm_reset (cparser_t *parser)
{
    assert(VALID_PARSER(parser));

    cparser_token_stack_reset(parser);
    parser->cur_node = parser->root[parser->root_level];
    parser->state = CPARSER_STATE_WHITESPACE;
}

