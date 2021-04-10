#!/usr/bin/python3.9
"""
This code is responsible for executing & checking all the test directories, 
loading the configuration file from test.json
"""
import subprocess
import argparse
import json
import sys
import os

__author__  = 'bellrise'
__version__ = '0.3'


class TestSuite:
    """ This represents a single test case. """

    __path   = ''
    __config = {}
    __output = False

    def __init__(self, directory: str, output: bool):
        """ Load the config file from the given directory creating a new
        test suite. """

        self.__output = output

        self.__path = directory
        if not self.__path.endswith('/'):
            self.__path += '/'

        self.__config = json.loads(self.fetch_file('test.json', 'Cannot load ' \
        'configuration file %s for \'%s\' test suite'))

        # Validate test.json
        fields = ['@version', 'title', 'source', 'result']
        for field in fields:
            if field not in self.__config:
                print(f'Missing {field} field in test.json config')
                exit()

    def fetch_file(self, filename: str, err_msg='Cannot open %s in %s') -> str:
        file = ''
        try:
            with open(self.__path + filename) as f:
                file = f.read()
        except FileNotFoundError:
            print(err_msg % (filename, self.__path))
            exit()
        return file

    def test(self) -> bool:
        """ Run the test. """

        print('-' * 80)
        print(f'* Running test suite \033[93m{self.__config["title"]}\033[0m')
        if 'about' in self.__config:
            print(f'\033[94m  {self.__config["about"]}\033[0m')
        else:
            print(f'mNo description provided.')

        args = ''
        if 'args' in self.__config:
            args = self.__config['args']

        print(f'  Running \'svm {args} {self.__path}{self.__config["source"]}\'')
        out = subprocess.check_output(f'svm {args} {self.__path}{self.__config["source"]}', shell=True)
        out = str(out, 'ascii')

        if self.__output:
            print(out)

        result = self.fetch_file(self.__config['result'])

        if out == result:
            print('& \033[92mPassed.\033[0m\n')
            return True
        
        print('& \033[91mFailed.\033[0m\n')
        return False


def main():
 
    # Check the version before running
    if sys.version_info[0] != 3 or sys.version_info[1] < 9:
        print('svm_test does not support Python 3.8 or less')
        exit()


    # Find svm executable
    found = False
    for bin_dir in os.get_exec_path():
        for file in os.listdir(bin_dir):
            if file == 'svm' or file == 'svm.exe':
                found = True
                break

    if not found:
        print('SVM not found')
        exit()


    pr = argparse.ArgumentParser(description='The testing software for the SVM')
    pr.add_argument('-f', '--fail', action='store_true', help='exit on first failed test')
    pr.add_argument('-o', '--only', help='run a single test')
    pr.add_argument('-d', '--debug', help='print test output', action='store_true')

    args = pr.parse_args()

    if args.only:
        if args.only in os.listdir():
            test = TestSuite(args.only)
            test.test()
        else:
            print('Unknown test')
        return

    org_dirs = os.listdir()
    to_remove = ['README.md', 'svm_test.py']
    tests = []
    for i in org_dirs:
        if not i.startswith('.') and i not in to_remove:
            tests.append(i)

    passed = 0
    for test_name in tests:
        test = TestSuite(test_name, args.debug)
        if test.test():
            passed += 1
        elif args.fail:
            print('Exiting on failure (\'-f\')')
            exit(1)

    print(f'Passed [{passed}/{len(tests)}] tests.')

if __name__ == '__main__':
    main()
