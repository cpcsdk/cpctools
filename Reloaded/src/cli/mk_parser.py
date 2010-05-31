#!/usr/bin/python
# $Id: mk_parser.py 110 2009-03-27 00:57:00Z henry $

# Copyright (c) 2008-2009, Henry Kwok
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the project nor the names of its contributors 
#       may be used to endorse or promote products derived from this software 
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY HENRY KWOK ''AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL HENRY KWOK BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import re, sys, glob

print_tree = True
debug = False
end_node = None

##
# \brief     Debug printf
# \details   Print out a object preceeded by a string header if global
#            'debug' is True.
#
# \param     hdr A header string
# \param     s   Data to be displayed
def DBG(hdr, s):
    global debug
    if debug:
        sys.stdout.write(hdr)
        print(s)
        
## A class of parse tree node
class Node:
    TOKENS = [ 'ROOT', 'END', 'KEYWORD', 'STRING', 'UINT', 'INT',
               'HEX', 'FLOAT', 'MACADDR', 'IPV4ADDR', 'FILE' ]
    TYPES = { 'ROOT'       : None,
              'END'        : None,
              'KEYWORD'    : None,
              'STRING'     : 'char *',
              'UINT'       : 'uint32_t ',
              'INT'        : 'int32_t ',
              'HEX'        : 'uint32_t ',
              'FLOAT'      : 'double ',
              'MACADDR'    : 'cparser_macaddr_t ',
              'IPV4ADDR'   : 'uint32_t ',
              'FILE'       : 'char *'
              }
    
    ## Constructor.
    def __init__(self, node_type, param, desc, flags):
        '''
        Constructor.
        '''
        self.type = node_type
        self.param = param
        if desc:
            self.desc = '    "%s",\n' % desc
        else:
            self.desc = '    NULL,\n'
        self.flags = flags
        self.children = []

        # Cannot fill these out until we insert the node to the tree
        self.parent = None
        self.depth = 0
        self.path = ''
        self.next = None
        return

    ##
    # \brief     Add a child node to a tree node.
    #
    # \param     child A Node object that is added to this node as its child.
    def add_child(self, child):
        for c in self.children:
            if (c.type == child.type) and (c.param == child.param):
                # The node already exists. Re-use the existing node.
                return c
        # Fill out some information that are tree structure dependent.
        # These information are actually embedded in the tree already
        # However, we compute them and cache them to reduce the
        # processing time later. This approach increases the storage
        # requirement but decreases the processing time.
        child.parent = self
        child.depth = self.depth + 1
        if child.is_param() or child.is_keyword():
            child.path = self.path + '_' + child.param.replace('-','_')
        elif 'END' == child.type:
            child.path = self.path + '_eol'
        elif 'ROOT' == child.type:
            # If we are adding a child ROOT node, the parent ('self' here)
            # must be an END node. For END node, we insert a string
            # 'cparser_glue' to the front of self.path. So, we must
            # remove it first before creating the submode root path
            child.path = self.param.replace('cparser_glue', '') + '_root'
        if len(self.children) > 0:
            self.children[-1].next = child
        
        # Insert the node into the children list
        self.children.append(child)
        return child

    ## Return True if it is a parameter node; False otherwise.
    def is_param(self):
        if (('ROOT' == self.type) or ('END' == self.type) or ('KEYWORD' == self.type)):
            return False
        return True

    ## Return True if it is a keyword node; False otherwise.
    def is_keyword(self):
        return ('KEYWORD' == self.type)

    ## Return True if it is an optional keyword / parameter; False otherwise.
    def is_optional(self):
        return (0 < (self.flags.count('CPARSER_NODE_FLAGS_OPT_END') +
                     self.flags.count('CPARSER_NODE_FLAGS_OPT_PARTIAL')))
    
    ## Display a summary of the node.
    def display(self, fout=sys.stdout):
        if 'ROOT' == self.type:
            fout.write('<ROOT> ')
        elif 'KEYWORD' == self.type:
            fout.write('<KEYWORD:%s> ' % self.param)
        elif 'END' == self.type:
            fout.write('<END> ')
        else:
            fout.write('<%s:%s> ' % (self.type, self.param))

    ##
    # \brief     Walk the tree
    #
    # \param     fn     Function to be called per node.
    # \param     mode   'pre-order', 'func', 'post-order'.
    # \param     cookie An opaque object to be passed into 'fn'.
    #
    # \return    Return the number of callback made.
    def walk(self, fn, mode, cookie):
        last = len(self.children)
        count = 0
        if (mode == 'pre-order') or (mode == 'func'):
            if ((mode == 'pre-order') or
                ((self.type == 'END') and
                 (not self.flags.count('CPARSER_NODE_FLAGS_OPT_PARTIAL')))):
                fn(self, cookie)
                count += 1
            for n in range(last):
                count += self.children[n].walk(fn, mode, cookie)
        elif 'post-order' == mode:
            for n in range(last):
                count += self.children[last-1-n].walk(fn, mode, cookie)
            fn(self, cookie)
            count += 1
        else:
            raise ValueError, 'Unknown walk mode: %s' % mode
        return count
    
    ## Generate the C structure name.
    def c_struct(self):
        if self.parent == None:
            msg = 'cparser_node_t cparser_root = {\n'
        else:
            msg = 'cparser_node_t cparser_node%s = {\n' % self.path
        # type
        msg += '    CPARSER_NODE_%s,\n' % self.type
        # flags
        if len(self.flags) == 0:
            msg += '    0,\n'
        else:
            msg += '    ' + ' | '.join(self.flags) + ',\n'
        # param
        if 'ROOT' == self.type:  msg += '    NULL,\n'
        elif 'END' == self.type: msg += '    %s,\n' % self.param
        elif 'KEYWORD' == self.type: msg += '    "%s",\n' % self.param
        else: msg += '    "<%s:%s>",\n' % (self.type, self.param)
        # desc
        msg += self.desc
        # sibling
        if self.next:
            msg += '    &cparser_node%s,\n' % self.next.path
        else:
            msg += '    NULL,\n'
        # children
        if len(self.children) > 0:
            msg += '    &cparser_node%s\n' % self.children[0].path
        else:
            msg += '    NULL\n'
        msg += '};\n'
        return msg

    ## Return a list of Node objects that forms a path from root to this node.
    def walk_up_to_root(self):
        assert self.type == 'END'
        p = []
        cur_node = self
        while cur_node.parent:
            p.insert(0, cur_node)
            cur_node = cur_node.parent
            if cur_node.type == 'ROOT':
                break
        return p

    ## Generate the action function.
    def action_fn(self):
        # Build a list of parse nodes that forms the path from the root.
        # to this end node
        path = self.walk_up_to_root()

        # Declare the action function
        msg = ('cparser_result_t %s(cparser_context_t *context' %
               self.param.replace('cparser_glue', 'cparser_cmd'))

        # Declare the variable list
        for n in path:
            if not n.is_param(): continue
            msg += ',\n    %s*%s_ptr' % (Node.TYPES[n.type], n.param)
        msg += ');\n'
        return msg

    ## Generate the glue function.
    def glue_fn(self):
        # Build a list of parse nodes that forms the path from the root.
        # to this end node
        path = self.walk_up_to_root()

        # Build the glue function
        msg =  'cparser_result_t\n'
        msg += '%s (cparser_t *parser)\n' % self.param
        msg += '{\n'

        # Declare the variable list
        skip = ''
        for n in path:
            if not n.is_param(): continue
            skip = '\n'
            val_type = Node.TYPES[n.type]
            msg += '    %s%s_val;\n' % (val_type, n.param)
            msg += '    %s*%s_ptr = NULL;\n' % (val_type, n.param)
        if skip: msg += '    cparser_result_t rc;\n'
        msg += skip
        
        # Extract the parameters
        k = -1
        for n in path:
            k = k + 1
            if not n.is_param(): continue
            msg += ('    rc = cparser_get_%s(&parser->tokens[%d], &%s_val);\n' %
                    (n.type.lower(), k, n.param))
            if n.is_optional():
                msg += '    if (CPARSER_OK == rc) {\n'
                msg += '        %s_ptr = &%s_val;\n' % (n.param, n.param)
                msg += '    } else {\n'
                msg += '        assert(%d > parser->token_tos);\n' % (k+1)
                msg += '    }\n'
            else:
                msg += '    assert(CPARSER_OK == rc);\n'
                msg += '    %s_ptr = &%s_val;\n' % (n.param, n.param)

        # Call the user-provided action function
        msg += ('    %s(&parser->context' %
                self.param.replace('cparser_glue', 'cparser_cmd'))
        for n in path:
            if n.is_param(): msg += ',\n        %s_ptr' % n.param
        msg += ');\n'
        msg += '    return CPARSER_OK;\n'
        msg += '}\n\n'
        return msg

##
# \brief     Add one line of CLI to the parse tree.
#
# \param     root     Root node of the parse tree.
# \param     line     A line of command from a CLI file.
# \param     comment  The comment that goes with this line of command.
#
# \return    Return a new root node.
def add_cli(root, line, comment):
    global end_node
    nodes = []
    flags = []
    num_opt_start = 0

    # Convert a line into a token list
    line = line.replace('\n','')
    DBG('\nDBG:add_cli:1>> ', line)
    tokens = line.split(' ')

    # Delete all token that is ''
    cnt = tokens.count('')
    for k in range(0, cnt):
        tokens.remove('')
    if len(tokens) == 0:
        return root # this is a blank line. quit

    # Check each token to make sure that it is valid
    for t in tokens:
        m = re.search('\<(.+):(.+)\>', t)
        if m:
            # A parameter. Check type and variable name
            if not Node.TYPES.has_key(m.group(1)):
                raise ValueError, 'Unknown parameter type "%s".' % m.group(1)
            if not re.search('^[a-zA-Z]([a-zA-Z0-9_]*)$', m.group(2)):
                raise ValueError, 'Invalid parameter name "%s".' % m.group(2)
        elif (t == '{') or (t == '}'):
            continue
        else:
            if not re.search('^[a-zA-Z0-9_-]+$', t):
                raise ValueError, 'Invalid keyword "%s".' % t

    for t in tokens:
        # Parse each token
        # Look for '{'
        if '{' == t:
            flags.append('CPARSER_NODE_FLAGS_OPT_START')
            num_opt_start = num_opt_start + 1
            continue
        # Look for '}'
        if '}' == t:
            nodes[len(nodes)-1].flags.append('CPARSER_NODE_FLAGS_OPT_END')
            continue
        flags = []
        # Look for a parameter 
        m1 = re.search('\<(.+):(.+)\>', t)
        m2 = re.search('\<(.+):(.+):(.+)\>', t)
        if m1 or m2:
            if m2:
                node_type = m2.group(1)
                param = m2.group(2)
                desc = m2.group(3)
            else:
                desc = None
                node_type = m1.group(1)
                param = m1.group(2)
            nodes.append(Node(node_type, param, desc, flags))
            continue
        # Look for a keyword
        m = re.search('(.+)', t)
        if m:
            nodes.append(Node('KEYWORD', m.group(1), None, flags))

    # hack alert - Check that if there are optional parameters, the format is ok
    DBG('DBG:add_cli:2>> ', tokens)

    if debug:
        DBG('DBG:add_cli:3>> ', '')
        for nn in nodes:
            nn.display()
        sys.stdout.write('\n')

    # We need to create the glue function name. Since the path of the node
    # is set up when the node is inserted, we don't have it here. So,
    # we manually walk all the nodes and generate the glue function name
    glue_fn = 'cparser_glue' + root.param
    for n in nodes:
        glue_fn = glue_fn + '_' + n.param.replace('-','_')
    
    # Insert them into the parse tree
    for k in range(0, num_opt_start+1):
        DBG('DBG:add_cli:4>> ', k)
        num_braces = 0
        drop = False
        cur_node = root
        if num_opt_start == k:
            end_node = Node('END', glue_fn, comment, [])
        else:
            end_node = Node('END', glue_fn, None, ['CPARSER_NODE_FLAGS_OPT_PARTIAL'])
        for n in nodes:
            if drop:
                continue
            if (n.flags.count('CPARSER_NODE_FLAGS_OPT_START') +
                n.flags.count('CPARSER_NODE_FLAGS_OPT_END') > 0):
                if num_braces == k:
                    drop = True
                num_braces = num_braces + 1
            if debug:
                cur_node.display()
                sys.stdout.write('-> ')
                n.display()
                sys.stdout.write('\n')
            cur_node = cur_node.add_child(n)
        if debug:
            cur_node.display()
            sys.stdout.write('-> ')
            end_node.display()
            sys.stdout.write('\n')
        cur_node.add_child(end_node)
    return root

##
# \brief     Process one .cli file. This includes handling all
#            preprocessors directive.
#
# \param     filename Name of the .cli file.
# \param     root     Root Node object of the parse tree.
# \param     mode     'compile', 'preprocess' or 'mkdep'
# \param     labels   A dictionary containing all defined labels used in
#                     preprocessing.
#
# \return    Return the new root node.
def process_cli_file(filename, root, mode, labels):
    num_disable = 0
    label_stack = []
    deplist = []
    comment = None
    line = ''
    last_cli_root = None
    last_cli_end = None
    submode = False
    try:
        fin = open(filename, 'r')
    except:
        print 'Fail to open %s.' % filename
        sys.exit(-1)
        
    line_num = 0
    for line in fin:
        line_num = line_num + 1

        # Process the file line by line. The orderof processing
        # These different directives is extremely important. And the
        # order below is not arbitrary.
        #
        # We must process #endif not matter what. Otherwise, once
        # we start a #ifdef block that is omitted, we'll never be able
        # to terminate it. Then, we omit every other type of line as long
        # as there is at least one disable #ifdef left. Afterward, we
        # check for illegal directives. A normal command line is handled
        # last. Illegal tokens are checked inside add_cli().

        # #endif
        m = re.search('^#endif', line)
        if m:
            if len(label_stack) == 0:
                print('%s:%d: Unmatched #ifdef/#ifndef' % (filename, line_num))
                sys.exit(-1)
            num_disable = num_disable - label_stack.pop(0)[1]
            continue
        # Skip the rest of processing because some #ifdef/#ifndef is
        # keeping this line from being processed.
        if (num_disable > 0):
            continue
        # Check for illegal preprocessor directives
        if (re.search('^#', line) and
            (not re.search('^#ifdef(\S*\/\/.*)*', line) and
             not re.search('^#submode(\S*\/\/.*)*', line) and
             not re.search('^#endsubmode(\S*\/\/.*)*', line) and
             not re.search('^#include(\S*\/\/.*)*', line))):
            print('%s:%d: Unknown preprocessor directive.' % (filename, line_num))
            sys.exit(-1)
        # Comment
        m = re.search('^\s*\/\/\s*(.*)', line)
        if m:
            if 'compile' == mode:
                comment = m.group(1)
            if 'preprocess' == mode:
                sys.stdout.write(line)
            continue
        # #ifdef
        m = re.search('^#ifdef (.+)', line)
        if m:
            l = m.group(1)
            val = 0
            if not labels.has_key(l):
                val = 1
                num_disable = num_disable + 1
            label_stack.insert(0, [l, val])
            continue
        # #ifndef
        m = re.search('^#ifndef (.+)', line)
        if m:
            l = m.group(1)
            val = 0
            if labels.has_key(m.group(1)):
                val = 1
                num_disable = num_disable + 1
            label_stack.insert(0, [l, val])
            continue
        # #include
        m = re.search('^#include "(.+)"', line)
        if m:
            if len(glob.glob(m.group(1))) == 0:
                print('%s:%d: file %s does not exist.' %
                      (filename, line_num, m.group(1)))
                sys.exit(-1)
            if ('compile' == mode):
                process_cli_file(m.group(1), root, mode, labels)
            elif ('mkdep' == mode):
                deplist.append(m.group(1))
            elif ('preprocess' == mode):
                process_cli_file(m.group(1), root, mode, labels)
            else:
                print('%s:%d: unknown mode %s' % (filename, line_num, mode))
            continue
        # #submode
        m = re.search('^#submode "(.+)"', line)
        if m:
            if submode:
                print('%s:%d: nested submode is invalid.' % (filename, line_num))
            else:
                submode = True
                last_cli_root = Node('ROOT', '_' + m.group(1),
                                     'Root of submode %s' % m.group(1), [])
                last_cli_end.add_child(last_cli_root)
            continue
        # #endsubmode
        m = re.search('^#endsubmode', line)
        if m:
            if not submode:
                print('%s:%d: #endsubmode without a #submode.' %
                      (filename, line_num))
                sys.exit(-1)
            else:
                submode = False
                last_cli_root = None
                last_cli_end = None
            continue
        # What survive must be either an empty line or a command
        if ('compile' == mode):
            try:
                if not submode:
                    root = add_cli(root, line, comment)
                    last_cli_end = end_node
                else:
                    last_cli_root = add_cli(last_cli_root, line, comment)
            except ValueError, msg:
                print('%s:%d: %s' % (filename, line_num, msg))
                sys.exit(-1)
        elif ('preprocess' == mode):
            sys.stdout.write(line)
        comment = None
    if 'mkdep' == mode:
        sys.stdout.write('%s:' % filename)
        for d in deplist: sys.stdout.write(' %s' % d)
        sys.stdout.write('\n')
    return root

# walker_gen_dbg - Display each node
def walker_gen_dbg(node, fout):
    fout.write('  ' * node.depth)
    node.display()
    fout.write('\n')

## Process the command-line argument
def main():
    filelist = []
    labels = {}
    mode = 'compile'
    out_dir = '.'
    c_fname = 'cparser_tree.c'
    h_fname = 'cparser_tree.h'
    # Parse input arguments
    sys.argv.pop(0) # remove mk_parser.py itself
    while (len(sys.argv) > 0):
        item = sys.argv.pop(0)
        if '-MM' == item:
            mode = 'mkdep'
        elif '-P' == item:
            mode = 'preprocess'
        elif '-D' == item:
            l = sys.argv.pop(0)
            labels[l] = 0;
        elif '-o' == item:
            out_dir = sys.argv.pop(0)
        elif '-c' == item:
            c_fname = sys.argv.pop(0)
        elif '-i' == item:
            h_fname = sys.argv.pop(0)
        else:
            filelist.append(item)

    # Process each file
    root = Node('ROOT', '', 'Root node of the parser tree', [])
    for f in filelist:
        if ('mkdep' != mode):
            print('Processing %s...' % f)
        root = process_cli_file(f, root, mode, labels)
    if print_tree:
        root.walk(walker_gen_dbg, 'pre-order', sys.stdout)

    # Generate .c file that contains glue functions and parse tree
    c_fname = out_dir + '/' + c_fname
    try:
        fout = open(c_fname, 'w')
    except:
        print 'Fail to open %s.' % c_fname
        sys.exit(-1)
    fout.write('/*----------------------------------------------------------------------\n' +
               ' * This file is generated by mk_parser.py.\n' + 
               ' *----------------------------------------------------------------------*/\n' +
               '#include <assert.h>\n' +
               '#include <stdint.h>\n' +
               '#include <stdio.h>\n' +
               '#include "cparser.h"\n' +
               '#include "cparser_priv.h"\n' +
               '#include "cparser_token.h"\n' +
               '#include "cparser_tree.h"\n\n')    
    n_cmds = root.walk(lambda n,f: f.write(n.glue_fn()), 'func', fout)
    n_nodes = root.walk(lambda n,f: f.write(n.c_struct()), 'post-order', fout)
    fout.close()

    h_fname = out_dir + '/' + h_fname
    try:
        fout = open(h_fname, 'w')
    except:
        print 'Fail to open %s.' % h_fname
        sys.exit(-1)
    fout.write('/*----------------------------------------------------------------------\n' +
               ' * This file is generated by mk_parser.py.\n' +
               ' *----------------------------------------------------------------------*/\n' +
               '#ifndef __CPARSER_TREE_H__\n' +
               '#define __CPARSER_TREE_H__\n\n' +
               'extern cparser_node_t cparser_root;\n\n')
    root.walk(lambda n,f: f.write(n.action_fn()), 'func', fout)
    fout.write('\n#endif /* __CPARSER_TREE_H__ */\n')
    fout.close()

    # Print out a summary
    print '%d commands.' % n_cmds
    print '%d parse tree nodes (%d bytes).' % (n_nodes, n_nodes * 24)

    return

# Entry point of the script
if __name__ == '__main__':
    main()

