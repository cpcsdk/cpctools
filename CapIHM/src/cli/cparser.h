/**
 * \file     cparser.h
 * \brief    Parser library definitions and function prototypes.
 * \version  \verbatim $Id: cparser.h 130 2009-04-03 21:37:09Z henry $ \endverbatim
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
/**
 * \mainpage CLI Parser
 *
 * \section intro 1. INTRODUCTION
 *
 * Many embedded system require a command-line interface (CLI). CLI 
 * commands are useful for debugging and testing. This library provides 
 * a simply yet feature-rich CLI infrastructure.
 *
 * The goal of CLI parser is to centralize the complexity of CLI features
 * such that developers do not need to spend a lot of time coding in order
 * to get the convenience of a modern, Cisco-like CLI.
 *
 * \section overview 2. OVERVIEW
 *
 * CLI parser simplifies command creation by introducing a .cli file 
 * syntax. Users create their commands by writing .cli file. A python 
 * script (provided by CLI parser) converts these .cli files into a 
 * list of C structures forming a parse tree. The parse tree is
 * then used by CLI parser to provide CLI in run-time.
 *
 * Users are responsible for providing the following:
 *
 * - .cli files - These files define CLI commands and are compiled by 
 *   mk_parser.py into a parse tree used by libparser.
 *
 * - Action functions - They are callback functions when a CLI command
 *   is accepted by CLI parser.
 *
 * - I/O functions - Prototypes of these functions are listed in cparser_io.h.
 *   These functions connect CLI parser to the system I/O. For Linux/UNIX,
 *   these functions are provided. For other platforms, you will have to provide
 *   your own.
 *
 * CLI parser provides the following features:
 *
 * - Command completion
 * - Command recall
 * - Context sensitive help
 * - Automatic help generation
 *
 * The rest of the document are divided into the following sections:
 * - \ref cli - Describe the syntax of CLI files.
 * - \ref action - Describe how to write action functions.
 * - \ref app - Describe how to build an application using CLI parser.
 * - \ref port - Describe how to port CLI parser to non-UNIX platforms.
 *
 * \section install 3. INSTALLATION
 *
 * To install CLI parser, obtain a .tar.gz file for CLI Parser. 
 * Unzip/untar it into a directory of UNIX machine. Go to the top-level 
 * directory of CLI Parser and issue: <tt>make -s unix</tt>. This
 * will create a build/unix directory which contains a library in
 * lib/libcparser.a and some test and demo programs in bin/. (The -s
 * flag is only used to suppress the actual issued command.)
 *
 * \section license 4. LICENSING
 *
 * I have no intention of making a dime out of this work. I am more 
 * interested in having some tool that I like available to me regardless
 * of whatever job I take in the future. If you like what you see, feel
 * free to use. I delibrately use a modified BSD license because I don't
 * want people to worry about opening their own source and other legal
 * issue. If you make millions using this, more power to you! Regardless,
 * if you have any kind of feedbacks, drop me a note.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * \page cli CLI Files
 *
 * \section cli_intro 1. INTRODUCTION
 *
 * CLI files have suffix of .cli. Each CLI contains four types lines:
 * - Empty line
 * - Comment - Any line preceeded by '//' is a line of comment. '//'
 *   must be 1st character of the line.
 * - Directive - C style \#ifdef, \#ifndef, \#include, \#submode,
 *   \#endsubmode and \#end are available. The label is defined via 
 *   command-line arguments to mk_parser.py.
 * - Command definition - A command definition contains a sequence of 
 *   tokens separated by spaces.
 *
 * \section cli_empty 2. Empty Lines
 *
 * Empty lines can be inserted anywhere to make CLI files more readable.
 * However, it does have an implication on generating help displays.
 * See next section for details.
 *
 * \section cli_comment 3. Comments
 *
 * Each line of comment must begin with '//'. One cannot append
 * comments to a line of command or directive. A line of comment
 * immediately preceeding a line of command definition is automatically
 * included in the help display (generated by calling cparser_help_cmd().)
 * cparser_help_cmd() walks the parse tree and generating a summary of all
 * available commands. The comment immediately preceeding the command 
 * is saved as the description string of that command.
 *
 * For example, suppose we have the following command.
 *
 * <pre>
 * // Clear all interface counters
 * clear interface stats
 * </pre>
 *
 * cparser_help() will generate the following:
 *
 * <pre>
 * Clear all interface counters
 *   clear interface stats
 * </pre>
 *
 * Note that if an empty line is inserted between the comment and the
 * command definition, the command will have an empty description string.
 *
 * \section cli_directive 4. Directive
 *
 * \subsection cli_directive_ifdef 4.1 Conditional Inclusion
 *
 * Directive are used to provide conditional inclusion of commands. 
 * For example, suppose we have an embedded system that runs on
 * both the actual hardware platform and on a Linux PC. Some of the
 * hardware diagnostic commands should not be included in the Linux
 * PC simulation.
 *
 * We can use \#ifdef/\#endif to conditionally define a set of commands.
 *
 * <pre>
 * \#ifdef HW_PLATFORM
 * register read <HEX:base_addr> { <HEX:num_words> }
 * register write <HEX:base_addr> <HEX:val>
 * \#endif
 * </pre>
 *
 * These two commands of peeking and poking H/W registers will only be
 * available if HW_PLATFORM is provided to mk_parser.py. To define
 * this label, pass "-D HW_PLATFORM" to mk_parser.py.
 *
 * \subsection cli_directive_include 4.2 CLI Files Inclusion
 *
 * When the number of commands becomes large, it is inconvenient to
 * hold all commands in a single .cli file. A \#include directive is
 * provided to allow file inclusion much like C header inclusion.
 *
 * For example, we can put the two H/W register command in a file 
 * called hw_reg.cli and rewrite the previous block as:
 *
 * <pre>
 * \#ifdef HW_PLATFORM
 * \#include "hw_reg.cli"
 * \#endif
 * </pre>
 * 
 * \subsection cli_directive_submode 4.3 Submode
 *
 * Submode allows the parser to switch to a different parse tree.
 * Submode is a useful feature for creating hierarchy in CLI.
 * Sometimes, a "flat" CLI structure can make it difficult to read
 * configuration. Suppose we have a network router with interfaces. 
 * We can configure it using a flat hierarchy.
 *
 * <pre>
 * interface eth0 ip-address 20.1.1.1 255.0.0.0
 * interface eth0 acl acl1
 * interface eth0 qos qos-profile1
 * </pre>
 *
 * or it can use submode to wrap all interface configurations.
 *
 * <pre>
 * interface eth0  <- enter interface submode.
 *  ip-adress 20.1.1.1 255.0.0.0
 *  acl acl1
 *  qos qos-profile1
 * </pre>
 *
 * By grouping all interface configuration commands inside interface
 * submode, it reduces the number of top-level commands. This has
 * an implication on the effectiveness of context sensitive help. If
 * a first-time user wants to see what top-level commands are 
 * available, he/she can be overwhelmed if all commands belong to
 * the top-level.
 *
 * To define commands in a submode, wrap those commands between
 * \#submode and \#endsubmode. For example, the interface submode
 * above looks like this:
 *
 * <pre>
 * interface <STRING:if_name>
 * \#submode "if"
 * ip-address <IPV4ADDR:addr> <IPV4ADDR:mask>
 * acl <STRING:acl_name>
 * qos <STRING:qos_name>
 * \#endsubmode
 * </pre>
 *
 * A \#submode directive requires a submode name. All submode action
 * functions are prefixed by this name after "cparser_cmd_". For example, 
 * \#submode "if" means all its action functions are prefixed by 
 * "cparser_cmd_if_". It must also follow a command. The command
 * that a \#submode directive follows is the command that enters the.
 * submode.
 *
 * Submodes can be nested. The total number of submode plus the top-
 * level is equal to CPARSER_MAX_NESTED_LEVELS.
 *
 * \section cli_command 5. Command Definitions
 *
 * The following are a list of currently supported tokens. There are
 * two classes of tokens - keywords and parameters. Keywords are
 * strings that form CLI commands. For example, the command
 * "show roster" consists of two keyword tokens ("show" and "roster"). 
 * Each keyword should be a string consists of alphabet, digits (0-9), 
 * '_' and '-'. Parameters are values entered by users. They have 
 * the form of <[type]:[var]> where [type] is the token type and [var] 
 * is a legal C variable name.
 *
 * CLI parser provides the following parameter token types:
 *
 * - STRING - A string without ' '.
 * - UINT - 32-bit unsigned integer in decimal or 
 *   hexadecimal. E.g. 0x1234, 312476.
 * - INT - 32-bit signed integer. E.g., -1234, 13469.
 * - HEX - 32-bit unsigned integer in form of hexadecimal 
 *   only. E.g., 0xabc1234.
 * - FLOAT - 64-bit floating point value. E.g., 0.134,
 *   -123.406.
 * - MACADDR - 48-bit IEEE 802 MAC address. E.g., 00:11:AA:EE:FF.
 * - IPV4ADDR - IPv4 address. E.g., 10.1.1.1 or 192.168.1.1.
 * - FILE - A string that represents the path of a file. It
 *   is a specialized version of STRING that provides file 
 *   completion.
 * 
 * \subsection cli_extending 5.1 Extending CLI parser
 *
 * (<i>In the future, user can extend CLI parser by adding new token 
 * types.</i>)
 *
 * \subsection cli_optional 5.2 Optional parameters
 *
 * It is possible to include optional parameters in CLI commands. To
 * make a parameter optional, wrap it inside a pair of { }. For example,
 * suppose we define a command that adds 2 or 3 numbers:
 *
 * add <INT:a> <INT:b> { <INT:c> }
 *
 * The following are valid inputs: 
 *
 * <pre>
 * add 1 2 3
 * add 1 2
 * </pre>
 *
 * It is possible to have multiple optional parameters in a command but
 * they must be nested. For example, suppose we have a command that adds
 * two to four numbers:
 *
 * <pre>
 * LEGAL: add <INT:a> <INT:b> { <INT:c> { <INT:d> } }
 * ILLEGAL: add <INT:a> <INT:b> { <INT:c> } { <INT:d> }
 * </pre>
 *
 * There are couple reasons for imposing such restrictions. First,
 * a command with N optional parameters has only N possible acceptable
 * forms with the nesting constraint but 2^N forms without it. Second,
 * there are unresolvable ambiguities without the constraint.
 * 
 * Suppose user inputs "add 1 2 3". Is it c=3 and d is omitted or
 * c is omitted and d=3? With the nesting constraint, c must be 3
 * and d is omitted.
 * 
 * \section cli_example 6. EXAMPLE
 *
 * The following is an example of a CLI for a sample employee 
 * database program.
 *
 * <pre>
 * // This is the CLI syntax file of a test/sample program that handles
 * // an employee roster of a company.
 *
 *
 * // This line tests if comments are handled correctly
 * 
 * // List a summary of employees.
 * show employees
 * 
 * // List detail records of all employees.
 * show employees all
 *
 * // List all employees within a certain range of employee ids
 * show employees-by-id <UINT:min> { <UINT:max> }
 * 
 * // Add a new employee or enter the record of an existing employee
 * employee <HEX:id>
 *
 * \#ifdef TEST_LABEL1
 * // This next line tests if file inclusion is handled correctly.
 * \#include "test_included.cli"
 * \#endif
 *
 * \#ifdef TEST_LABEL2
 * // This next line tests if not existing label is handled correctly.
 * \#include "test_not_included.cli"
 * \#endif
 *
 * no employee <HEX:id>
 *
 * // Save the current roster to a file
 * save roster <STRING:filename>
 *
 * // Load roster file
 * load roster <FILE:filename>
 *
 * // List all available commands with a substring 'filter' in it.
 * help { <STRING:filter> }
 *
 * // Leave the database
 * quit
 *
 * </pre>
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * \page action Action Functions
 *
 * \section action_intro 1. INTRODUCTION
 *
 * An action function is the function called when a command is accepted
 * by CLI parser. This section describes how to write action functions
 * and what you can do with them.
 *
 * \section action_name 2. FUNCTION NAMING
 *
 * This function name is automatically generated from the command itself 
 * by: 
 * -# Concatenating all keywords and parameter variable names with '_'
 *    between them. This includes variable names of optional parameters.
 * -# Converting all '-' to '_'.
 * -# Prepending 'cparser_cmd_' to it.
 *
 * For example, the command <tt>"save roster <STRING:filename>"</tt> expects an 
 * action function of <tt>"cparser_cmd_save_roster_filename"</tt>.
 *
 * \section action_params 3. FUNCTION PARAMETERS
 *
 * The first argument of an action function is cparser_context_t which 
 * provides some information about the parser state. (More on this later). 
 * Each parameter token type corresponds to the following C data type:
 *
 * - STRING -> char *
 * - UINT -> uint32_t 
 * - INT -> int32_t 
 * - HEX -> uint32_t 
 * - FLOAT -> double
 * - MACADDR -> cparser_macaddr_t
 * - IPV4ADDR -> uint32_t 
 * - FILE -> char *
 *
 * All parameter tokens of a command are converted into pointers of their
 * corresponding C argument of the action function in order. For example,
 * we have a command <pre>"save roster { <STRING:filename> }"</pre> that
 * saves all employee record into a file. If 'filename' is not provided,
 * "default.dat" is used. The function prototype of its corresponding 
 * action function is:
 *
 * <pre>
 * cparser_result_t 
 * cparser_cmd_save_roster(cparser_context_t *context, char **filename);
 * </pre>
 *
 * The extra pointer is used to indicate if the parameter is present.
 * For optional parameter that is omitted, the pointer will be NULL.
 * For parameters that exist (mandatory or optional), they will point to
 * valid data. You should check against NULL for each optional parameter
 * in your action functions. In our example, it may look something like 
 * this:
 *
 * <pre>
 * cparser_result_t 
 * cparser_cmd_save_roster(cparser_context_t *context, char **filename);
 * {
 *     char *fname = "default.dat";
 *     if (filename) {
 *         fname = *filename;
 *     }
 *     // Save the employee roster using 'fname'
 *     ...
 *     return CPARSER_OK;
 * }
 * </pre>
 *
 * \section action_retval 4. RETURNED VALUES
 *
 * Each action function must return a cparser_result_t. If there is no
 * error, return CPARSER_OK. Otherwise, return CPARSER_NOT_OK. The
 * processing of an action function is considered failed if the
 * requested action cannot be fullfilled. This can happen even though
 * CLI Parser does parameter checking because application specific
 * constraints cannot be checked. For example, if user enters 14 for 
 * a parameter token <UINT:month> which is supposed to be a valid
 * month from 1 to 12, the action function should reject the command
 * and return an error. (Returning an error does not alter CLI Parser
 * processing in anyways currently. In the future, it will increment
 * a counter.)
 *
 * \section action_submode 5. ENTERING AND LEAVING SUBMODE
 *
 * If submode is used, there must be a command for each submode to
 * enter it. Action function of that command should call 
 * cparser_submode_enter(). An opaque pointer 'cookie' is provide to allow
 * some context of the submode. For example, going back to our interface
 * submode example, 'cookie' can point to the interface structure.
 * The command prompt can also change to indicate what submode you
 * are in; simply provide a string in 'prompt'.
 *
 * 'cookie' can be retrieved by the 'cookie' field of a cparser_context_t
 * (passed in every action function). The first submode uses index 1.
 * The second (nested) submode uses index 2 and etc.
 *
 * When user enters a command to exit the submode, call 
 * cparser_submode_exit().
 *
 * \section cli_api 6. OTHER USEFUL API
 *
 * \subsection cli_load 6.1 Loading A Script
 *
 * cparser_load_cmd() feeds a text file into the parser. It automatically
 * exits submode by examining indentation. Submodes are indented from
 * its parent mode. If a line indentation is less than the previous line,
 * it automatically exits the submode. This behavior is identical to
 * Cisco CLI.
 *
 * \subsection cli_help 6.2 Display A Help Summary
 *
 * cparser_help_cmd() generates a summary of all available commands in
 * the parse tree. The final output is similar to the original .cli
 * file (or files). This function provides a more convenient alternative
 * to creating a separate command that displays all available commands.
 * Since this function walks the compiled parse tree, it will never fail 
 * to list an existing command. If a separate help command is created,
 * developer may forget to update the help command when new commands are
 * introduced.
 *
 * It also have an optional filter string. If 'str' is non-NULL, it is *
 * used to filter out commands. Any command with a keyword that has 'str'
 * as a substring is displayed. All other commands are omitted. This
 * feature is useful when the number of commands in the system is very
 * large and user just want to find a small set of commands (possibly
 * with the same keywords or prefixes).
 *
 * \subsection cli_walk 6.3 Walk The Parse Tree
 *
 * cparser_walk() walks through the entire parse tree and invoke 
 * up to two callbacks for each node. One application of this function
 * is cparser_help_cmd(). Another example of this function application is
 * to save configuration. One can walk through the tree and locate all
 * commands that generate states. Each command can then regenerate CLI
 * commands that will recreate the state.
 *
 * \subsection cli_quit 6.4 Quit The Parser
 *
 * cparser_quit() causes CLI parser to exit from cparser_run().
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * \page app Building Your Application
 *
 * \section app_intro 1. INTRODUCTION
 *
 * After defining your CLI commands and coding up action functions
 * for these commands, you need to incorporate these commands
 * (and their action functions) into your application.
 *
 * To do that,
 * -# Run mk_parser.py to compile all .cli files into the parse tree.
 * -# Call cparser_init() to initialize a parser instance in your 
 *    application.
 * -# Call cparser_run() or cparser_input() to run the parser.
 * -# Compile your application with both the action functions and
 *    the parse tree linked in.
 *
 * \section app_mk_parser 2. COMPILING CLI FILES
 *
 * Use mk_parser.py to compile .cli files in a parse tree. mk_parser.py
 * takes in a set of -D flags (could be an empty set) and a set of 
 * .cli files and outputs a cparser_tree.c, a cparser_tree.h.
 *
 * cparser_tree.c contains a set of C structures that form the parse tree.
 *
 * \section app_calls 3. ADDING CLI PARSER CALLS
 *
 * You can have multiple CLI parser sessions in your application. Each
 * parser instance is initialized by calling cparser_init(). One needs 
 * to fill out all fields in cparser_cfg_t. The 'root' field should be 
 * <pre>&cparser_root</pre> defined in cparser_tree.c. 'ch_complete', 
 * 'ch_erase', 'ch_del', and 'ch_help' are commonly '\\t', \<BS\> ('\\b'),
 * \<DEL\> (127), and '?' respectively. 'prompt' must be a NULL-terminated 
 * string.
 * 
 * Run the parser instance when you are ready. There are two interfaces 
 * available - cparser_input() and cparser_run(). cparser_input() is a 
 * low-level interface. You are responsible for getting all characters (from 
 * whatever source) and feed them into the parser one character at a time. 
 * cparser_run() is a more automated interface. The user provides a set 
 * of parser I/O functions (see cparser_io.h) and cparser_run() will 
 * call them to get characters. It returns when the CLI session terminates.
 *
 * \subsection app_compile 4. COMPILING YOUR APPLICATION
 *
 * To build your application, include a rule for generating cparser_tree.c
 * and another rule for compiling cparser_tree.c and your .c file 
 * containing action functions. Link your application against libcparser.a.
 *
 * \subsection app_example 5. EXAMPLE
 *
 * A complete example is provided in build/unix/bin/test_parser. If run
 * withont any parameter, it is an automated unit test. To run it 
 * interactively, issue: 
 *
 * <pre>
 * test_parser -i
 * </pre>
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * \page port Porting
 * 
 * \section port_intro 1. INTRODUCTION
 *
 * CLI parser is mostly platform-independent. However, there are couple
 * areas that needs to be rewritten/modified when porting to different
 * platforms - some basic data types and I/O subsystem
 *
 * \section port_types 2. BASIC DATA TYPES
 *
 * CLI Parser assumes the following types are defined - int32_t, uint32_t,
 * int16_t, uint16_t, uint8_t, int8_t. In Linux / MAC OS X, stdint.h defines
 * these types. If your platform does not have stdint.h, you will
 * need to define these types in cparser.h.
 *
 * \section port_io 2. I/O SUBSYSTEM
 * 
 * I/O subsystem refers to a set of functions that receives keystrokes
 * from users and sends output to user's screen. The default I/O assumes 
 * input comes from a UNIX terminate as stdin and output goes to stdout. 
 * If these are not true, you need to provide a set of functions to 
 * initialize, cleanup I/O subsystem, to read a character and to write 
 * a character and a string. cparser_io.h provides the function prototype
 * of all I/O functions that needs to be rewritten when porting to a
 * different platforms.
 */

#ifndef __CPARSER_H__
#define __CPARSER_H__

#include "cparser_options.h"

/*
 * CLI Parser assumes the following types are defined: uint8_t, int8_t,
 * uint16_t, int16_t, uint32_t, int32_t. In many platforms (including
 * all platforms with GNU toolchain), stdint.h defines these types.
 * Thus, including this file is sufficient. If your platform does not 
 * have these types defined, you need to define them here.
 */
#include <stdint.h>

/*
 * This is to match Cisco CLI behavior. For example, if there is a
 * command "show crypto interfaces", one can just enter "sh cry int"
 * if no other command has the same prefix form.
 */
#define SHORTEST_UNIQUE_KEYWORD

/**
 * \brief    Parser API result code.
 */
typedef enum cparser_result_ {
    CPARSER_OK = 0,
    CPARSER_NOT_OK,
    CPARSER_ERR_INVALID_PARAMS, /**< Invalid input parameters to a call */
    CPARSER_ERR_NOT_EXIST,      /**< Requested object does not exist    */
    CPARSER_ERR_OUT_OF_RES,     /**< Requested resource is exhausted    */
    CPARSER_MAX_RESULTS
} cparser_result_t;

/**
 * \struct   cparser_t
 * \brief    CLI parser structure.
 * \details  See cparser_ for detais.
 */
typedef struct cparser_ cparser_t;
typedef struct cparser_node_ cparser_node_t;

#include "cparser_line.h"
#include "cparser_io.h"

/**
 * \struct   cparser_context_t
 * \brief    A structure passed back in action function.
 * \details  It contains the parser where the command is accepted 
 *           and some cookie that can be set the caller.
 */
typedef struct {
    /** Pointer to the parser instance */
    cparser_t *parser;
    /** An array of opaque pointers for nested levels */
    void      *cookie[CPARSER_MAX_NESTED_LEVELS];
} cparser_context_t;

#define CPARSER_FLAGS_DEBUG        (1 << 0)

/**
 * \struct   cparser_cfg_
 * \brief    Contains all configurable parameters of a parser.
 */
typedef struct cparser_cfg_ {
    cparser_node_t  *root;
    char            ch_complete;
    char            ch_erase;
    char            ch_del;
    char            ch_help;
    char            prompt[CPARSER_MAX_PROMPT];
    int             fd;
    unsigned long   flags;

    /* I/O API */
    cparser_io_init_fn     io_init;
    cparser_io_cleanup_fn  io_cleanup;
    cparser_getch_fn       getch;
    cparser_printc_fn      printc;
    cparser_prints_fn      prints;
} cparser_cfg_t;

/**
 * \struct   cparser_token_t
 * \brief    A parsed token.
 */
typedef struct cparser_token_ {
    /** Index (in the line) of the beginning of the token */
    short          begin_ptr;
    short          token_len;  /**< Number of character in the token */
    char           buf[CPARSER_MAX_TOKEN_SIZE]; /**< Local copy of the token */
    /** 
     * Pointer to the parent node whose one of its children matches
     * this token.
     */
    cparser_node_t *parent;
} cparser_token_t;

/**
 * \brief    Parser FSM states.
 * \details  There are 3 possible states in parser FSM.
 */
typedef enum cparser_state_ {
    CPARSER_STATE_WHITESPACE = 0,
    CPARSER_STATE_TOKEN,
    CPARSER_STATE_ERROR,
    CPARSER_MAX_STATES
} cparser_state_t;

/**
 * \brief    CLI parser structrure.
 * \details  This structure contains all configuration and running states.
 *           of a parser instance.
 */
struct cparser_ {
    /** Parser configuration structure */
    cparser_cfg_t     cfg;
    /** Current nested level */
    int               root_level;
    /** Parse tree root node at different nested levels */
    cparser_node_t    *root[CPARSER_MAX_NESTED_LEVELS];
    /** Parser prompt at different nested levels */
    char              prompt[CPARSER_MAX_NESTED_LEVELS][CPARSER_MAX_PROMPT];
    /** Current node */
    cparser_node_t    *cur_node;

    /********** FSM states **********/
    cparser_state_t   state;       /**< Current state */
    short             token_tos;   /**< Token stack top pointer */
    short             current_pos; /**< Current cursor in the line */
    /** Last cursor position that is in a non-error state */
    short             last_good;
    /** Token stack */
    cparser_token_t   tokens[CPARSER_MAX_NUM_TOKENS]; /* parsed tokens */

    /********** Line buffering states **********/
    short             max_line;
    short             cur_line;
    cparser_line_t    lines[CPARSER_MAX_LINES];

    /** Flag indicating if the parser should continue to except input */
    int               done;   
    /** Context passed back to action function */ 
    cparser_context_t context;
};

typedef cparser_result_t (*cparser_glue_fn)(cparser_t *parser);
typedef cparser_result_t (*cparser_token_fn)(char *token, int token_len,
                                             int *is_complete);

/**
 * \typedef  cparser_walker_fn
 * \brief    Walker function prototype used in cparser_walk().
 *
 * \param    parser Pointer to the parser structure.
 * \param    node   Pointer to the current node being walked.
 * \param    cookie An opaque pointer passed from cparser_walk().
 *
 * \return   CPARSER_OK to continue the walk; CPARSER_NOT_OK to abort.
 */
typedef cparser_result_t (*cparser_walker_fn)(cparser_t *parser, 
                                              cparser_node_t *node, void *cookie);

/**
 * \brief    Initialize a parser.
 *
 * \param    cfg Pointer to the parser configuration structure.
 *
 * \retval   parser Pointer to the initialized parser.
 * \return   CPARSER_OK if succeeded; CPARSER_NOT_OK if failed.
 */
cparser_result_t cparser_init(cparser_cfg_t *cfg, cparser_t *parser);

/**
 * \brief    Input a character to the parser.
 *
 * \param    parser  Pointer to the parser structure.
 * \param    ch      Character to be input.
 * \param    ch_type Character type.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_NOT_OK if failed.
 */
cparser_result_t cparser_input(cparser_t *parser, char ch, cparser_char_t ch_type);

/**
 * \brief    Run the parser. 
 * \details  This function is a wrapper around cparser_input(). It first 
 *           calls cparser_io_init(). Then, it calls cparser_getch() and 
 *           feeds character into the parser until it quits.
 *
 * \param    parser Pointer to the parser structure.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_NOT_OK if failed.
 */
cparser_result_t cparser_run(cparser_t *parser);

/**
 * \brief    Walk the parse tree in the parser.
 *
 * \param    parser  Pointer to the parser structure.
 * \param    pre_fn  Walker function that called before tranverse its children.
 * \param    post_fn Walker function that called after transvere its children.
 * \param    cookie  An opaque pointer that is passed back to the caller via
 *                   callback functions 'pre_fn' and 'post_fn'.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_NOT_OK if failed.
 */
cparser_result_t cparser_walk(cparser_t *parser, cparser_walker_fn pre_fn,
                              cparser_walker_fn post_fn, void *cookie);

/**
 * \brief    Walk the parser tree and generate a list of all available commands.
 *
 * \param    parser Pointer to the parser structure.
 * \param    str    Pointer to a filter string. If it is NULL, all
 *                  commands in the parse tree are displayed. Otherwise,
 *                  only commands with keywords that contain 'str' as
 *                  a substring are displayed.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_INVALID_PARAMS if 
 *           the parser structure is invalid.
 */
cparser_result_t cparser_help_cmd(cparser_t *parser, char *str);

/**
 * \brief    Load a command/config file to the parser. 
 * \details  A command/config file is just a text file with CLI commands. 
 *           (One command per line.) The only difference is that submode 
 *           is automatically exited if the indentation changes. This 
 *           behavior is the same as Cisco CLI.
 *
 * \param    parser   Pointer to the parser structure.
 * \param    filename Pointer to the filename.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_INVALID_PARAMS
 *           if the input parameters are NULL; CPARSER_NOT_OK if the file
 *           cannot be opened.
 */
cparser_result_t cparser_load_cmd(cparser_t *parser, char *filename);

/**
 * \brief    Exit a parser session.
 * \details  This call causes the parser to exit and returns from 
 *           cparser_run().
 *
 * \param    parser - Pointer to the parser structure.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_INVALID_PARAMS if failed.
 */
cparser_result_t cparser_quit(cparser_t *parser);

/**
 * \brief    Enter a submode.
 *
 * \param    parser Pointer to the parser structure.
 * \param    cookie A parameter that is passed back to each submode command
 *                  via cparser_context_t.
 * \param    prompt The new submode prompt.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_INVALID_PARAMS if the input 
 *           parameters are invalid; CPARSER_NOT_OK if there too many levels 
 *           of submode already.
 */
cparser_result_t cparser_submode_enter(cparser_t *parser, void *cookie, 
                                       char *prompt);

/**
 * \brief    Leave a submode. 
 * \details  The previous mode context and prompt are automatically restored.
 *
 * \param    parser Pointer to the parser structure.
 *
 * \return   CPARSER_OK if succeeded; CPARSER_ERR_INVALID_PARAMS if the input 
 *           parameters are invalid; CPARSER_NOT_OK if the parser has not 
 *           entered any submode.
 */
cparser_result_t cparser_submode_exit(cparser_t *parser);

#endif /* __CPARSER_H__ */
