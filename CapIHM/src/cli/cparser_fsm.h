/**
 * \file     cparser_fsm.h
 * \brief    Parser state machine definitions and prototype.
 * \version  \verbatim $Id: cparser_fsm.h 72 2009-03-19 07:30:35Z henry $ \endverbatim
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

#ifndef __CPARSER_FSM_H__
#define __CPARSER_FSM_H__

#include "cparser.h"

#define CUR_TOKEN(p) (&((p)->tokens[(p)->token_tos]))

/**
 * Reset all parser FSM states.
 *
 * \param    parser Pointer to the parser structure.
 *
 * \return   None.
 */
void cparser_fsm_reset(cparser_t *parser);

/**
 * Input a character to parser FSM.
 *
 * \param    parser Pointer to the parser structure.
 * \param    ch     Input character.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_NOT_OK otherwise.
 */
cparser_result_t cparser_fsm_input(cparser_t *parser, char ch);

/**
 * Walk through all children of a node. Return a match node if one is found.
 *
 * \param    token     Pointer to the beginning of the token.
 * \param    token_len Length of the token.
 * \param    parent    Pointer to the parent node.
 *
 * \retval   match       Pointer to a node that matches the token.
 *                       If there are multiple matches, the highest priority match
 *                       is returned.
 * \retval   is_complete 1 if the token completely matches 
 * \return   Number of matches.
 */
int cparser_match(const char *token, const int token_len, cparser_node_t *parent,
                  cparser_node_t **match, int *is_complete);

#endif /* __CPARSER_FSM_H__ */
