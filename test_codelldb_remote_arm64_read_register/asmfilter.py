#!/usr/bin/env python
#coding: utf-8

"""
用来过滤 GCC/Clang 编译阶段生成的 .s 文件， 产生 human readable 的汇编。
也就是微型的 Compiler Explorer （godbolt.org） 仿制品。


最初用于 《Reverse Engineering for Beginners》这本书的汇编结果比对
"""

import sys
import fileinput

def cat():
    for line in fileinput.input():
        line = line.rstrip()
        print(line)


def is_label_line(line):
    thin_line = line.strip()
    if ( len(thin_line) == len(line) ):
        return True
    return False


def is_effective_line(line):
    thin_line = line.strip()
    if (thin_line.startswith('.string')):
        return True
    if (thin_line.startswith('.')):
        return False
    if (thin_line == 'endbr64'):
        return False
    if (thin_line == 'endbr32'):
        return False
    return True


def asmfilter():
    """
    mock of godbolt parseAsm
    https://stackoverflow.com/a/69122522/2999096
    https://github.com/compiler-explorer/compiler-explorer/blob/b744d6ad9e2aecc305df02b3e4c942ee434141e2/lib/asm-parser.js#L260
    https://stackoverflow.com/a/63015987/2999096
    """
    raw_content = []
    total = 0
    for line in fileinput.input():
        line = line.rstrip()
        raw_content.append(line)
        total += 1
    raw_content.append('useless:')

    final_content = []

    i = 0
    label = None
    label_follows = []
    for i in range(total):
        line = raw_content[i]
        if (is_label_line(line)):
            if ( (label is not None) and (len(label_follows) > 0) ):
                final_content.append(label)
                for item in label_follows:
                    final_content.append(item)

            label = line
            label_follows = []
            continue

        if (is_effective_line(line)):
            label_follows.append(line)

    for line in final_content:
        print(line)

def help():
    print("Usage:")
    print("    gcc -S -fno-asynchronous-unwind-tables -fno-dwarf2-cfi-asm -masm=intel f2.c")
    print("    asmfilter.py f2.s")


if __name__ == '__main__':
    if ( len(sys.argv) != 2):
        help()
        exit(-1)
    asmfilter()


