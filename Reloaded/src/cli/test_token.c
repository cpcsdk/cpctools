/**
 * \file     test_token.c
 * \brief    Test program for verifying token parsing routines.
 * \version  \verbatim $Id: test_token.c 105 2009-03-26 23:22:58Z henry $ \endverbatim
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
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"

/** Return the number of elements of an array */
#define NELEM(a) (sizeof(a)/sizeof(a[0]))

#define SET_TOKEN(t, s)                 \
    strcpy((t).buf, (s));               \
    (t).token_len = strlen(s)

/**
 * \brief    Entry point of the program.
 *
 * \param    argc Number of arguments.
 * \param    argv Array of argument strings.
 *
 * \return   Return 0.
 */
int main (int argc, char *argv[])
{
    struct {
        char               *name;
        char               *str;
        cparser_node_type_t type;
        char               *param;
        cparser_result_t    result;
        int                is_complete;
    } match_testcases[] = {
        /* Keyword */
        { "KW01", "s", CPARSER_NODE_KEYWORD, "show", CPARSER_OK, 0 },
        { "KW02", "sh", CPARSER_NODE_KEYWORD, "show", CPARSER_OK, 0 },
        { "KW03", "sho", CPARSER_NODE_KEYWORD, "show", CPARSER_OK, 0 },
        { "KW04", "show", CPARSER_NODE_KEYWORD, "show", CPARSER_OK, 1 },
        { "KW05", "shows", CPARSER_NODE_KEYWORD, "show", CPARSER_NOT_OK, 0 },
        { "KW06", "how", CPARSER_NODE_KEYWORD, "show", CPARSER_NOT_OK, 0 },
        /* String */
        { "STR01", "abc", CPARSER_NODE_STRING, "s", CPARSER_OK, 1 },
        { "STR02", "1234", CPARSER_NODE_STRING, "s", CPARSER_OK, 1 },
        /* Unsigned integer */
        { "UINT01", "1xabc", CPARSER_NODE_UINT, "a", CPARSER_NOT_OK, 0 },
        { "UINT02", "0xabc", CPARSER_NODE_UINT, "a", CPARSER_OK, 1 },
        { "UINT03", "01234", CPARSER_NODE_UINT, "a", CPARSER_OK, 1 },
        { "UINT04", "012abc", CPARSER_NODE_UINT, "a", CPARSER_NOT_OK, 0 },
        { "UINT05", "0x", CPARSER_NODE_UINT, "a", CPARSER_OK, 0 },
        { "UINT06", "1x", CPARSER_NODE_UINT, "a", CPARSER_NOT_OK, 0 },
        /* Signed integer */
        { "INT01", "123", CPARSER_NODE_INT, "a", CPARSER_OK, 1 },
        { "INT02", "9876543210", CPARSER_NODE_INT, "a", CPARSER_OK, 1},
        { "INT03", "abc", CPARSER_NODE_INT, "a", CPARSER_NOT_OK, 0 },
        { "INT04", "-980", CPARSER_NODE_INT, "a", CPARSER_OK, 1 },
        { "INT05", "-", CPARSER_NODE_INT, "a", CPARSER_OK, 0 },
        { "INT06", "+", CPARSER_NODE_INT, "a", CPARSER_OK, 0 },
        { "INT07", "+5678", CPARSER_NODE_INT, "a", CPARSER_OK, 1 },
        { "INT08", "1324-80", CPARSER_NODE_INT, "a", CPARSER_NOT_OK, 0 },
        /* Hexadecimal */
        { "HEX01", "0x78abcdef", CPARSER_NODE_HEX, "a", CPARSER_OK, 1 },
        { "HEX02", "0x0123456", CPARSER_NODE_HEX, "a", CPARSER_OK, 1 },
        { "HEX03", "1x1234", CPARSER_NODE_HEX, "a", CPARSER_NOT_OK, 0 },
        { "HEX04", "0x", CPARSER_NODE_HEX, "a", CPARSER_OK, 0 },
        { "HEX05", "0xyz", CPARSER_NODE_HEX, "a", CPARSER_NOT_OK, 0 },
        /* Float */
        /* MAC address */
        { "MAC01", "0", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC02", "00", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC03", "00:", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC04", "00:1", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC05", "00:11", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC06", "00:11:", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC07", "00:11:9", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC08", "00:11:99", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC09", "00:11:99:", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC10", "00:11:99:a", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC11", "00:11:99:aa", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC12", "00:11:99:aa:", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC13", "00:11:99:aa:C", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC14", "00:11:99:aa:CC", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC15", "00:11:99:aa:CC:", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 0 },
        { "MAC16", "00:11:99:aa:CC:F", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 1 },
        { "MAC17", "00:11:99:aa:CC:Ff", CPARSER_NODE_MACADDR, "macaddr", CPARSER_OK, 1 },
        { "MAC18", "xx:yy:zz:pp:qq:rr", CPARSER_NODE_MACADDR, "macaddr", CPARSER_NOT_OK, 0 },
        /* IP address */
        { "IP4_01", "1", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_OK, 0 },
        { "IP4_02", "10.", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_OK, 0 },
        { "IP4_03", "10.123", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_OK, 0 },
        { "IP4_04", "10.123.0.", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_OK, 0 },
        { "IP4_05", "10.123.0.1", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_OK, 1 },
        { "IP4_06", "10.123.0.13", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_OK, 1 },
        { "IP4_07", "1234.1.1.1", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_NOT_OK, 0 },
        { "IP4_08", "1.2.3.3.4.5", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_NOT_OK, 0 },
        { "IP4_09", "a.b.c.d", CPARSER_NODE_IPV4ADDR, "ipv4", CPARSER_NOT_OK, 0 },
        /* File path */
        { "FILE01", "test.txt", CPARSER_NODE_FILE, "fname", CPARSER_OK, 1 },
        { "FILE02", "/usr/include/stdio.h", CPARSER_NODE_FILE, "fname", CPARSER_OK, 1 },
    };
    int n, is_complete, num_tests = 0, num_passes = 0;
    int total_tests = 0, total_passes = 0;
    cparser_result_t result;
    cparser_node_t node;
    cparser_token_t token;

    for (n = 0; n < NELEM(match_testcases); n++) {
        node.type = match_testcases[n].type;
        node.param = match_testcases[n].param;
        is_complete = 0xff;
        assert(cparser_match_fn_tbl[node.type]);
        result = cparser_match_fn_tbl[node.type](match_testcases[n].str, 
                                                strlen(match_testcases[n].str), 
                                                &node, &is_complete);
        num_tests++;

        if ((result == match_testcases[n].result) && 
            (is_complete == match_testcases[n].is_complete)) {
            printf("PASS: %s: ", match_testcases[n].name);
            num_passes++;
        } else {
            printf("FAIL: %s: ", match_testcases[n].name);
        }
        printf("'%s' -> (%d, %s) -> (%s, %s)\n", match_testcases[n].str,
               match_testcases[n].type, match_testcases[n].param, 
               (CPARSER_OK == result ? "OK" : "NOT OK"),
               (is_complete ? "COMPLETE" : "INCOMPLETE"));
    }
    printf("Match function tests: %d total. %d passed.\n\n\n", 
           num_tests, num_passes);
    total_tests = num_tests;
    total_passes = num_passes;

    /* 
     * Test the get functions. Due to the output argument of each 
     * function being different. There is no table to simplify this.
     */
    num_tests = num_passes = 0;

    /* String */
    char *str = "hello, world", *str_val;
    SET_TOKEN(token, str);
    result = cparser_get_string(&token, &str_val);
    num_tests++;
    if ((CPARSER_OK != result) || (str_val != token.buf)) {
        printf("FAIL: STR01: %s -> %s\n", str, str_val);
    } else {
        printf("PASS: STR01: %s -> %s\n", str, str_val);
        num_passes++;
    }

    /* Unsigned integer */
    struct {
        char             *name;
        char             *str;
        cparser_result_t  result;
        uint32_t         val;
    } get_uint_testcases[] = {
        { "UINT01", "0x1234abCD", CPARSER_OK, 0x1234abcd },
        { "UINT02", "12345678", CPARSER_OK, 12345678 },
        { "UINT03", "0xabcdef123", CPARSER_NOT_OK, 0 },
        { "UINT04", "9876543210", CPARSER_NOT_OK, 0 },
        { "UINT05", "0x0000001234", CPARSER_OK, 0x1234 },
        { "UINT06", "00000000000001234", CPARSER_OK, 1234 }
    };
    uint32_t uint_val;
    for (n = 0; n < NELEM(get_uint_testcases); n++) {
        SET_TOKEN(token, get_uint_testcases[n].str);
        result = cparser_get_uint(&token, &uint_val);
        num_tests++;
        if ((result != get_uint_testcases[n].result) || 
            (uint_val != get_uint_testcases[n].val)) {
            printf("FAIL: %s: ", get_uint_testcases[n].name);
        } else {
            printf("PASS: %s: ", get_uint_testcases[n].name);
            num_passes++;
        }
        printf("%s -> %lu (%lX)\n", get_uint_testcases[n].str, 
               (unsigned long)uint_val, (unsigned long)uint_val);
    }

    /* Integer */
    struct {
        char             *name;
        char             *str;
        cparser_result_t  result;
        int32_t          val;
    } get_int_testcases[] = {
        { "INT01", "123", CPARSER_OK, 123 },
        { "INT02", "12345678", CPARSER_OK, 12345678 },
        { "INT03", "12345678900", CPARSER_NOT_OK, 0 },
        { "INT04", "9876543210", CPARSER_NOT_OK, 0 },
        { "INT05", "00000000000001234", CPARSER_OK, 1234 },
        { "INT06", "+000777777", CPARSER_OK, 777777 },
        { "INT07", "-000777777", CPARSER_OK, -777777 },
        { "INT08", "+2147483647", CPARSER_OK, 2147483647L },
        { "INT09", "+2147483648", CPARSER_NOT_OK, 0 },
        { "INT10", "-2147483648", CPARSER_OK, -2147483648LL },
        { "INT11", "-2147483649", CPARSER_NOT_OK, 0 },
    };
    int32_t int_val;
    for (n = 0; n < NELEM(get_int_testcases); n++) {
        SET_TOKEN(token, get_int_testcases[n].str);
        result = cparser_get_int(&token, &int_val);
        num_tests++;
        if ((result != get_int_testcases[n].result) || 
            (int_val != get_int_testcases[n].val)) {
            printf("FAIL: %s: ", get_int_testcases[n].name);
        } else {
            printf("PASS: %s: ", get_int_testcases[n].name);
            num_passes++;
        }
        printf("%s -> %ld\n", get_int_testcases[n].str, (long int)int_val);
    }

    /* Hexadecimal */
    struct {
        char             *name;
        char             *str;
        cparser_result_t  result;
        uint32_t         val;
    } get_hex_testcases[] = {
        { "HEX01", "0x1234abCD", CPARSER_OK, 0x1234abcd },
        { "HEX02", "0xabcdef123", CPARSER_NOT_OK, 0 },
        { "HEX03", "0x0000001234", CPARSER_OK, 0x1234 },
    };
    for (n = 0; n < NELEM(get_hex_testcases); n++) {
        SET_TOKEN(token, get_hex_testcases[n].str);
        result = cparser_get_hex(&token, &uint_val);
        num_tests++;
        if ((result != get_hex_testcases[n].result) || 
            (uint_val != get_hex_testcases[n].val)) {
            printf("FAIL: %s: ", get_hex_testcases[n].name);
        } else {
            printf("PASS: %s: ", get_hex_testcases[n].name);
            num_passes++;
        }
        printf("%s -> %lX\n", get_hex_testcases[n].str, 
               (unsigned long)uint_val);
    }

    /* Float */

    /* MAC address */
    struct {
        char             *name;
        char             *str;
        cparser_result_t  result;
        uint8_t          macaddr[6];
    } get_macaddr_testcases[] = {
        { "MAC01", "12:45:89:ab:CD:Ef", CPARSER_OK, 
          { 0x12, 0x45, 0x89, 0xab, 0xcd, 0xef } },
        { "MAC02", "1:4:9:a:C:f", CPARSER_OK, 
          { 0x01, 0x04, 0x09, 0x0a, 0x0c, 0x0f } },
    };
    cparser_macaddr_t macaddr;
    for (n = 0; n < NELEM(get_macaddr_testcases); n++) {
        SET_TOKEN(token, get_macaddr_testcases[n].str);
        result = cparser_get_macaddr(&token, &macaddr);
        num_tests++;
        if ((result != get_macaddr_testcases[n].result) || 
            (memcmp(&macaddr, get_macaddr_testcases[n].macaddr, 6))) {
            printf("FAIL: %s: ", get_macaddr_testcases[n].name);
        } else {
            printf("PASS: %s: ", get_macaddr_testcases[n].name);
            num_passes++;
        }
        printf("%s -> %02X:%02X:%02X:%02X:%02X:%02X\n", 
               get_macaddr_testcases[n].str, macaddr.octet[0], 
               macaddr.octet[1], macaddr.octet[2], macaddr.octet[3], 
               macaddr.octet[4], macaddr.octet[5]);
    }

    /* IPv4 address */
    struct {
        char             *name;
        char             *str;
        cparser_result_t  result;
        uint32_t         ipv4addr;
    } get_ipv4addr_testcases[] = {
        { "IP4_01", "192.168.1.1", CPARSER_OK, 0xc0a80101 },
        { "IP4_02", "333.1.1.1", CPARSER_NOT_OK, 0x0 },
    };
    uint32_t ipv4addr;
    for (n = 0; n < NELEM(get_ipv4addr_testcases); n++) {
        SET_TOKEN(token, get_ipv4addr_testcases[n].str);
        result = cparser_get_ipv4addr(&token, &ipv4addr);
        num_tests++;
        if ((result != get_ipv4addr_testcases[n].result) || 
            (memcmp(&ipv4addr, &get_ipv4addr_testcases[n].ipv4addr, 4))) {
            printf("FAIL: %s: ", get_ipv4addr_testcases[n].name);
        } else {
            printf("PASS: %s: ", get_ipv4addr_testcases[n].name);
            num_passes++;
        }
        printf("%s -> %d.%d.%d.%d\n", get_ipv4addr_testcases[n].str, 
               (int)(ipv4addr >> 24) & 0xff, (int)(ipv4addr >> 16) & 0xff, 
               (int)(ipv4addr >> 8) & 0xff, (int)ipv4addr & 0xff);
    }

    /* File path */
    struct {
        char             *name;
        char             *str;
        cparser_result_t  result;
    } get_file_testcases[] = {
        { "FILE01", "/usr/include/stdlib.h", CPARSER_OK },
        { "FILE02", "/tmp/xyz.parser", CPARSER_NOT_OK },
        { "FILE03", "/usr", CPARSER_NOT_OK }
    };
    char *file;
    for (n = 0; n < NELEM(get_file_testcases); n++) {
        SET_TOKEN(token, get_file_testcases[n].str);
        result = cparser_get_file(&token, &file);
        num_tests++;
        if ((result != get_file_testcases[n].result) || 
            ((CPARSER_OK == result) && 
             strcmp(file, get_file_testcases[n].str))) {
            printf("FAIL: %s: ", get_file_testcases[n].name);
        } else {
            printf("PASS: %s: ", get_file_testcases[n].name);
            num_passes++;
        }
        printf("%s -> %s\n", get_file_testcases[n].str, 
               (CPARSER_OK == result ? "OK" : "NOT OK"));
    }
    printf("Get function tests: %d total. %d passed.\n\n", 
           num_tests, num_passes);

    total_tests += num_tests;
    total_passes += num_passes;
    printf("Total=%d  Passed=%d  Failed=%d\n", total_tests,
           total_passes, total_tests - total_passes);
    return 0;
}
