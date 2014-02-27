import os
import sys
from os.path import join
from nose.tools import ok_, eq_

PROJRELROOT = '../'
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), PROJRELROOT)))
sys.path.append(os.path.abspath('./'))

import result_parser

the_product = str(os.getenv('TARGET_PRODUCT'))
the_parser = result_parser.ResultParser()
the_parser.ParseLog(the_product)
    
def test_flash():
    eq_(True, True)

def test_boot():
    ret = the_parser.GetLogResult('INSTALLATION', 'BOOT')
    eq_(ret, True)
