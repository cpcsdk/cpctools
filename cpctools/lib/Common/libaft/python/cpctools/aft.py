#!/bin/env python
# -*- coding: utf-8 -*-
from libaft import *

def main():
  port = '/dev/ttyUSB0'
  aksft = AksFileTransfert(port)

  if not aksft.IsOpen():
    print 'Unable to open port %s' % port
    exit(-1)

  while True:
    aksft.Run()

if __name__ == "__main__":
  main()


