/**
 * \file     test_parser.c
 * \brief    Test program for parser library. 
 * \details  This is a test program with a simple CLI that serves as a demo 
 *           as well.
 * \version  \verbatim $Id: test_parser.c 33 2009-01-22 06:45:33Z henry $ \endverbatim
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
#include "cparser_token.h"

int interactive = 0;
#define PRINTF(args...)                                 \
    if (interactive) {                                  \
        printf(args);                                   \
    } else {                                            \
        output_ptr += sprintf(output_ptr, args);        \
    }

#define MAX_NAME        (128)
#define MAX_EMPLOYEES   (100)
#define MAX_TITLE       (32)

char *output_ptr;
char output[2000]; /* buffer for sprintf */

/**
 * Employee record.
 */
typedef struct employee_ {
    char         name[MAX_NAME];   /**< Employee name */
    uint32_t     height;           /**< Height in units of inches */
    uint32_t     weight;           /**< Weight in units of lbs */
    double       bonus_factor;     /**< Multiplier for bonus */
    uint32_t     pc_ip_addr;       /**< PC IP address */
    uint8_t      mac_ip_addr[6];   /**< PC NIC MAC address */
    uint32_t     id;               /**< Employee id */
    struct dob_ {
        uint8_t  month;            /**< Month (1-12) */
        uint8_t  day;              /**< Day (1-31) */
        uint16_t year;             /**< Year */
    } dob;                         /**< Date of birth */
    char         title[MAX_TITLE]; /**< Job title */
} employee_t;

employee_t roster[MAX_EMPLOYEES];

/**
 * Add a new employee record to the roster.
 *
 * \param    roster  Pointer to the array of employee records.
 * \param    id      New employee id.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_OUT_OF_RES if there is
 *           no more available slot.
 */ 
static cparser_result_t
test_employee_add (employee_t *roster, const uint32_t id)
{
    int n;

    assert(roster && id);

    /* Look for an unused slot */
    for (n = 0; n < MAX_EMPLOYEES; n++) {
        if (!roster[n].id) {
            /* Found one. Use it */
            memset(&roster[n], 0, sizeof(roster[n]));
            roster[n].id = id;
            return CPARSER_OK;
        }
    }

    return CPARSER_ERR_OUT_OF_RES;
}

/**
 * Remove an existing employee record from the roster.
 *
 * \param    roster  Pointer to the array of employee records.
 * \param    id      Employee id to be deleted.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_NOT_EXIST if there is
 *           no employee with that id.
 */
static cparser_result_t
test_employee_remove (employee_t *roster, const uint32_t id)
{
    int n;

    assert(roster && id);
    
    /* Look for a match of the name */
    for (n = 0; n < MAX_EMPLOYEES; n++) {
        if (id == roster[n].id) {
            memset(&roster[n], 0, sizeof(*roster));
            return CPARSER_OK;
        }
    }

    return CPARSER_ERR_NOT_EXIST;
}

/**
 * Search for an existing employee record.
 *
 * \param    roster  Pointer to the array of employee records.
 * \param    id      Employee id to search.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_NOT_EXIST if there is
 *           no employee with that id.
 */
static employee_t *
test_employee_search (employee_t *roster, uint32_t id)
{
    int n;

    assert(roster && id);
    for (n = 0; n < MAX_EMPLOYEES; n++) {
        if (id == roster[n].id) {
            return &roster[n];
        }
    }
    return NULL;
}

/**
 * Initialize a roster of employee records.
 *
 * \param    roster Pointer to the array of employee records.
 */
void
test_employee_init (employee_t *roster)
{
    int n;

    assert(roster);
    for (n = 0; n < MAX_EMPLOYEES; n++) {
        memset(&roster[n], 0, sizeof(*roster));
    }
}

/**
 * Handle "show employees".
 */
cparser_result_t
cparser_cmd_show_employees (cparser_context_t *context)
{
    int n, num_shown = 0;

    assert(context);

    for (n = 0; n < MAX_EMPLOYEES; n++) {
        if (!roster[n].id) {
            continue;
        }
        PRINTF("0x%08X %s\n", roster[n].id, roster[n].name);
        num_shown++;
    }
    if (!num_shown) {
        PRINTF("No employee in the roster.\n");
    }
    return CPARSER_OK;
}

/**
 * Handle "show employees all".
 */
cparser_result_t
cparser_cmd_show_employees_all (cparser_context_t *context)
{
    int n, num_shown = 0;

    assert(context);

    for (n = 0; n < MAX_EMPLOYEES; n++) {
        if (!roster[n].id) {
            continue;
        }
        PRINTF("%s\n", roster[n].name);
        PRINTF("   ID: 0x%08X\n", roster[n].id);
        PRINTF("   Height: %3d\"   Weight: %3d lbs.\n",
               (int)roster[n].height, (int)roster[n].weight);
        num_shown++;
    }
    if (!num_shown) {
        PRINTF("No employee in the roster.\n");
    }
    return CPARSER_OK;
}

/**
 * Handle "show employees-by-id <HEX:min> <HEX:max>".
 */
cparser_result_t
cparser_cmd_show_employees_by_id_min_max (cparser_context_t *context, 
                                          uint32_t *min, uint32_t *max)
{
    int n, num_shown = 0;
    uint32_t min_val, max_val;

    assert(context && min); /* don't assert on 'max' because it is optional */

    min_val = *min;
    max_val = (max ? *max : 0xffffffff);

    for (n = 0; n < MAX_EMPLOYEES; n++) {
        if (!roster[n].id || (min_val > roster[n].id) || 
            (max_val < roster[n].id)) {
            continue;
        }
        PRINTF("%s\n", roster[n].name);
        PRINTF("   ID: 0x%08X\n", roster[n].id);
        PRINTF("   Height: %3d\"   Weight: %3d lbs.\n",
               (int)roster[n].height, (int)roster[n].weight);
        num_shown++;
    }
    if (!num_shown) {
        PRINTF("No employee in the roster.\n");
    }
    return CPARSER_OK;
}

/**
 * Handle "emp -> name <STRING:name>".
 */
cparser_result_t
cparser_cmd_emp_name_name (cparser_context_t *context, char **name)
{
    employee_t *emp;

    assert(context && name && (*name));
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    strncpy(emp->name, *name, MAX_NAME);
    return CPARSER_OK;
}

/**
 * Handle "emp -> height <UINT:inches>".
 */
cparser_result_t
cparser_cmd_emp_height_inches (cparser_context_t *context, uint32_t *inches)
{
    employee_t *emp;

    assert(context && inches);
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    if (120 <= *inches) {
        PRINTF("Too tall!\n");
        return CPARSER_NOT_OK;
    }
    emp->height = *inches;
    return CPARSER_OK;
}

/**
 * Handle "emp -> weight <UINT:lbs>".
 */
cparser_result_t
cparser_cmd_emp_weight_lbs (cparser_context_t *context, uint32_t *lbs)
{
    employee_t *emp;

    assert(context && lbs);
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    emp->weight = *lbs;
    return CPARSER_OK;
}

/**
 * Handle "emp -> bonuns-factor <FLOAT:scale>".
 */
cparser_result_t
cparser_cmd_emp_bonus_factor_scale (cparser_context_t *context, double *scale)
{
    employee_t *emp;

    assert(context && scale);
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    emp->bonus_factor = *scale;
    return CPARSER_OK;
}

/**
 * Handle "emp -> pc-ip-address <IPV4ADDR:ipv4>".
 */
cparser_result_t
cparser_cmd_emp_pc_ip_address_ipv4 (cparser_context_t *context,
                                    uint32_t *ipv4)
{
    employee_t *emp;

    assert(context && ipv4);
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    emp->pc_ip_addr = *ipv4;
    return CPARSER_OK;
}


/**
 * Handle "emp -> pc-mac-address <MACADDR:addr>".
 */
cparser_result_t
cparser_cmd_emp_pc_mac_address_addr (cparser_context_t *context,
                                     cparser_macaddr_t *addr)
{
    assert(context && addr);
    return CPARSER_OK;
}

/**
 * Handle "emp -> employee-id <HEX:id>".
 */
cparser_result_t
cparser_cmd_emp_employee_id_id (cparser_context_t *context, uint32_t *id)
{
    employee_t *emp;

    assert(context && id);
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    emp->id = *id;
    return CPARSER_OK;
}

/**
 * Handle "emp -> date-of-birth <INT:month> <INT:day> <INT:year>".
 */
cparser_result_t
cparser_cmd_emp_date_of_birth_month_day_year (cparser_context_t *context, 
                                              int32_t *month, int32_t *day, 
                                              int32_t *year)
{
    employee_t *emp;

    assert(context && month && day && year);
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    if (!(*month) || (12 < (*month))) {
        PRINTF("Month must be from 1-12.\n");
        return CPARSER_NOT_OK;
    }
    /* This is not totally correct but you get the point */
    if (!(*day) || (31 < (*day))) {
        PRINTF("Day must be from 1-31.\n");
    }
    emp->dob.month = *month;
    emp->dob.day = *day;
    emp->dob.year = *year;
    return CPARSER_OK;
}

/**
 * Handle "emp -> title <STRING:title>".
 */
cparser_result_t
cparser_cmd_emp_title_title (cparser_context_t *context, char **title)
{
    employee_t *emp;

    assert(context && title);
    emp = (employee_t *)context->cookie[1];
    assert(emp);
    strncpy(emp->name, *title, MAX_NAME);
    emp->name[MAX_NAME-1] = '\0';
    return CPARSER_OK;
}

/**
 * Handle "emp -> exit".
 */
cparser_result_t
cparser_cmd_emp_exit (cparser_context_t *context)
{
    assert(context && context->parser);
    return cparser_submode_exit(context->parser);
}

/**
 * Handle "employee <UINT:id>".
 */
cparser_result_t
cparser_cmd_employee_id (cparser_context_t *context, uint32_t *id)
{
    employee_t *new_emp;
    char prompt[CPARSER_MAX_PROMPT];

    assert(context && id && (*id));

    /* Check that there is no existing employee by that name */
    new_emp = test_employee_search(roster, *id);
    if (!new_emp) {
        /* Add a new record with that name */
        if (CPARSER_OK != test_employee_add(roster, *id)) {
            PRINTF("Fail to add employee.\n");
            return CPARSER_NOT_OK;
        }
        new_emp = test_employee_search(roster, *id);
    }

    /* Enter the submode */
    assert(new_emp);
    snprintf(prompt, CPARSER_MAX_PROMPT, "0x%08X: ", *id);
    return cparser_submode_enter(context->parser, new_emp, prompt);
}

/**
 * Handle "no employee <UINT:id>".
 */
cparser_result_t
cparser_cmd_no_employee_id (cparser_context_t *context, uint32_t *id)
{
    employee_t *emp;

    assert(context && id && *id);

    /* Check that the name exists */
    emp = test_employee_search(roster, *id);
    if (!emp) {
        PRINTF("Employee 0x%08X does not exist in the database.\n", *id);
        return CPARSER_ERR_NOT_EXIST;
    }

    /* Delete it */
    return test_employee_remove(roster, *id);
}

/**
 * Load a roster file into memory.
 */
cparser_result_t
cparser_cmd_load_roster_filename (cparser_context_t *context, char **filename)
{
    assert(context && filename && (*filename));
    return cparser_load_cmd(context->parser, *filename);
}

/**
 * Save the roster in memory into a file.
 */
cparser_result_t
cparser_cmd_save_roster_filename (cparser_context_t *context, char **filename)
{
    FILE *fp;
    int n;

    assert(context && filename);
    fp = fopen(*filename, "w");
    if (!fp) {
        PRINTF("Fail to open %s.\n", *filename);
        return CPARSER_NOT_OK;
    }

    for (n = 0; n < MAX_EMPLOYEES; n++) {
        if (!roster[n].id) {
            continue;
        }
        fprintf(fp, "employee 0x%08X\n", roster[n].id);
        if (strlen(roster[n].name)) {
            fprintf(fp, " name %s\n", roster[n].name);
        }
        fprintf(fp, " height %u\n", roster[n].height);
        fprintf(fp, " weight %u\n", roster[n].weight);
        if (roster[n].bonus_factor) {
            fprintf(fp, " bonus-factor %f\n", roster[n].bonus_factor);
        }
        if (roster[n].pc_ip_addr) {
            fprintf(fp, " pc-ip-address %d.%d.%d.%d\n", 
                    (int)((roster[n].pc_ip_addr >> 24) & 0xff),
                    (int)((roster[n].pc_ip_addr >> 16) & 0xff),
                    (int)((roster[n].pc_ip_addr >> 8) & 0xff),
                    (int)((roster[n].pc_ip_addr >> 0) & 0xff));
        }
        if (roster[n].dob.month && roster[n].dob.day && roster[n].dob.year) {
            fprintf(fp, " date-of-birth %d %d %d\n", roster[n].dob.month,
                    roster[n].dob.day, roster[n].dob.year);
        }
        if (strlen(roster[n].title)) {
            fprintf(fp, " title %s\n", roster[n].title);
        }
    }

    fclose(fp);
    return CPARSER_OK;
}

/**
 * List all available commands
 */
cparser_result_t
cparser_cmd_help_filter (cparser_context_t *context, char **filter)
{
    assert(context);
    return cparser_help_cmd(context->parser, filter ? *filter : NULL);
}

cparser_result_t
cparser_cmd_emp_help_filter (cparser_context_t *context, char **filter)
{
    assert(context);
    return cparser_help_cmd(context->parser, filter ? *filter : NULL);
}

/**
 * Exit the parser test program.
 */
cparser_result_t
cparser_cmd_quit (cparser_context_t *context)
{
    assert(context);
    return cparser_quit(context->parser);
}

