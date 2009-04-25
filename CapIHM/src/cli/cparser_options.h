/**
 * \file     cparser_options.h
 * \brief    Parser limits for various objects.
 * \version  \verbatim $Id: cparser_options.h 117 2009-03-28 21:30:12Z henry $ \endverbatim
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

#ifndef __CPARSER_OPTIONS_H__
#define __CPARSER_OPTIONS_H__

/**
 * Maximum number of character of a command prompt including the 
 * terminating NULL.
 */
#define CPARSER_MAX_PROMPT         (16)

/**
 * Maximum number of nested sub-mode levels.
 */
#define CPARSER_MAX_NESTED_LEVELS  (4)

/**
 * Maximum number of characters in one token.
 */
#define CPARSER_MAX_TOKEN_SIZE     (256)

/**
 * Maximum number of token per line.
 */
#define CPARSER_MAX_NUM_TOKENS     (32)

/**
 * Maximum number of lines.
 */
#define CPARSER_MAX_LINES          (10)

/**
 * Maximum number of character per line.
 */
#define CPARSER_MAX_LINE_SIZE      (383)

#endif /* __CPARSER_OPTIONS_H__ */
