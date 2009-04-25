/**
 * \file     cparser_token.h
 * \brief    Parser token parsing and completion routine prototypes.
 * \version  \verbatim $Id: cparser_token.h 105 2009-03-26 23:22:58Z henry $ \endverbatim
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

#ifndef __CPARSER_TOKEN_H__
#define __CPARSER_TOKEN_H__

#define CPARSER_MAX_NODE_TYPES (16)

/**
 * Parser node type.
 */
typedef enum {
    CPARSER_NODE_ROOT = 0,
    CPARSER_NODE_END,
    CPARSER_NODE_KEYWORD,
    CPARSER_NODE_STRING,
    CPARSER_NODE_UINT,
    CPARSER_NODE_INT,
    CPARSER_NODE_HEX,
    CPARSER_NODE_FLOAT,
    CPARSER_NODE_MACADDR,
    CPARSER_NODE_IPV4ADDR,
    CPARSER_NODE_FILE,
    CPARSER_MAX_NODES
} cparser_node_type_t;

/**
 * 48-bit MAC address.
 */
typedef struct {
    /** Six octet holding the MAC address */
    uint8_t octet[6];
} cparser_macaddr_t;

/**
 * \brief    Match function pointer.
 * \details  This function pointer is the prototype of all
 *           CLI Parser match functions.
 *
 * \param    token     Pointer to the token.
 * \param    token_len Number of valid characters in the token.
 * \param    node      The parse tree node to be matched against.
 *
 * \retval   is_complete Return 1 from if the token matches the entire
 *                       token; 0 otherwise.
 * \return   CPARSER_OK if it is a complete or partial match; 
 *           CPARSER_NOT_OK if it does match the node.
 */
typedef cparser_result_t (*cparser_match_fn)(const char *token, 
                                             const int token_len, 
                                             cparser_node_t *node, 
                                             int *is_complete);

/**
 * \brief    Completion function pointer.
 * \details  This function pointer is the prototype of all
 *           CLI Parser match functions.
 *
 * \param    token     Pointer to the token.
 * \param    token_len Number of valid characters in the token.
 */
typedef cparser_result_t (*cparser_complete_fn)(const char *token,
                                                const int token_len);

/**
 * \brief    Get function pointer.
 * \details  This function pointer is the prototype of all CLI Parser 
 *           get functions.
 *
 * \param    token     Pointer to the token.
 *
 * \retval   val       Pointer to the returned parameter value.
 * \return   CPARSER_OK if succeeded; CPARSER_NOT_OK otherwise.
 */
typedef cparser_result_t (*cparser_get_fn)(const cparser_token_t *token,
                                           void *val);

extern cparser_match_fn    cparser_match_fn_tbl[CPARSER_MAX_NODE_TYPES];
extern cparser_complete_fn cparser_complete_fn_tbl[CPARSER_MAX_NODE_TYPES];
extern cparser_get_fn      cparser_get_fn_tbl[CPARSER_MAX_NODE_TYPES];

/********** Token match functions **********/
cparser_result_t cparser_match_root(const char *token, const int token_len,
                                    cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_end(const char *token, const int token_len,
                                   cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_keyword(const char *token, const int token_len,
                                       cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_string(const char *token, const int token_len,
                                      cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_uint(const char *token, const int token_len,
                                    cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_int(const char *token, const int token_len,
                                   cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_hex(const char *token, const int token_len,
                                   cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_float(const char *token, const int token_len,
                                     cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_macaddr(const char *token, const int token_len,
                                       cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_ipv4addr(const char *token, const int token_len,
                                        cparser_node_t *node, int *is_complete);
cparser_result_t cparser_match_file(const char *token, const int token_len,
                                    cparser_node_t *node, int *is_complete);

/********** Token complete functions **********/
cparser_result_t cparser_complete_file(const char *token, const int token_len);

/********** Token get functions **********/
cparser_result_t cparser_get_string(const cparser_token_t *token, void *value);
cparser_result_t cparser_get_uint(const cparser_token_t *token, void *value);
cparser_result_t cparser_get_int(const cparser_token_t *token, void *value);
cparser_result_t cparser_get_hex(const cparser_token_t *token, void *value);
cparser_result_t cparser_get_float(const cparser_token_t *token, void *value);
cparser_result_t cparser_get_macaddr(const cparser_token_t *token, void *value);
cparser_result_t cparser_get_ipv4addr(const cparser_token_t *token, void *value);
cparser_result_t cparser_get_file(const cparser_token_t *token, void *value);

#endif /* __CPARSER_MATCH_H__ */
