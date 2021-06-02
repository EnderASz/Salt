#!/usr/bin/python3.9
""" 
This is the SCC3 assembler used for compiling down a basic SCC text format
into the actual SCC3 bytecode. This is extremely simple and is only used
for manual construction of SCC code, and isn't the best idea to use in real
compilers (if you're not writing them in Python).

Built for Python 3.9 and above.
"""
import argparse
import struct 
import re

__author__  = 'bellrise'
__version__ = '0.5'

# This SCC magic value is placed right at the beginning of the header,
# which lets SVM know that this indeed a Salt compiled code file.
SCC_MAGIC = b'\x7f\x53\x43\x43\xff\xee\0\0'
SCC_VERSION = 3

instruction_map: dict[str, list] = {
    'CALLF': [str],
    'EXITE': [],
    'EXTLD': [str],
    'KILLX': [],
    'MLMAP': [],
    'OBJMK': [int, bool, int, None],
    'OBJDL': [int],
    'PRINT': [int],
    'RETRN': [int, int],
    'RPUSH': [int, int]
}

# The constant table makes using constant values easier. In order to specify
# an object of type int, all you need to do is pass the [int] const value, 
# without remembering the actual byte value being 0x01.
const_table: dict[str, bytes] = {
    'false': b'\x00',
    'true': b'\x01',
    'null': b'\x00',
    'int': b'\x01',
    'float': b'\x02',
    'bool': b'\x03',
    'string': b'\x04'
}


def fatal(err_msg: str):
    """ Print the error message and exit the program. """
    
    print('fatal:', err_msg)
    exit()


class Assembler:
    """ This is responsible for generating the SCC3 bytecode from the passed
    instructions. """

    __version: int = 3
    __instructions: list[bytes] = []
    __registers: int = 0
    __code: str = ''

    __debug: bool = False

    __signature = b'\x7f\x7a\x7b\x7c\x32\x31\x00\x0a'
    
    replace_map = [
        ('\\n', '\x11'),
        ('\\r', '\r'),
        ('\\0', '\0'),
        ('\\t', '\t'),
        ('\\"', '"'),
        ('\\b', '\b')
    ]


    def __init__(self, code: str, *, debug: bool = False):
        """ Assembler constructor, prepares values for use. """
        
        self.__debug = debug
        for i, line in enumerate(code.split('\n')):
            self.add_instruction(i, line)

    def render(self) -> bytes:
        """ This will return the whole rendered bytecode in a byte string 
        so it can be put in a file """

        header = SCC_MAGIC
        header += struct.pack('ixxxx', SCC_VERSION)
        header += struct.pack('ixxxx', len(self.__instructions))
        header += struct.pack('ixxxx', self.__registers)
        header += b'\x00' * 24
        header += self.__signature

        if len(header) != 64:
            fatal('size of header is not 64 bytes')
        
        header += b'\n'.join(self.__instructions)
        if len(self.__instructions) != 0:
            header += b'\n'

        return header

    def print(self, *args, end='\n'):
        """ This just calls the normal print function only if the --debug flag
        is passed as an argument. """

        if self.__debug:
            print(*args, end=end)

    def add_meta(self, index: int, line: str):
        """ Add a setting to the header. """ 

        data = line.split()
        if len(data) < 2:
            fatal(f'Meta value required a parameter (\'{line}\', on line {index})')
        
        key = data[0][2:]
        value = data[1]

        if key == 'reg':
            if not value.isdigit():
                fatal(f'Register amount has to be an int on line {index}')
            value = int(value)
            if value < -2147483647 or value > 2147483647:
                fatal(f'int out of bounds on line {index}')
            self.__registers = value
        else:
            fatal(f'Unknown meta value on line {index}')

    def split_args(self, line: str):
        """ Split the arguments into a list. This ignores spaces found in
        strings. """
        
        args = []

        splits = [0]
        in_str = False
        for i, c in enumerate(line):
            if c == '"' and in_str:
                in_str = False
            if c == '"' and not in_str:
                in_str = True
            if c == ' ' and not in_str:
                splits.append(i)
        
        for i, split in enumerate(splits):
            args.append(line[splits[i-1]:split].strip())
        args.append(line[splits[-1]:].strip())

        return [i for i in args if i]


    def add_instruction(self, index: int, line: str):
        """ This will add the single instruction to the instruction list, 
        parsing & validating it beforehand. """ 

        line = line.strip()

        if not line or line.startswith(';'):
            return

        if not line.isascii():
            fatal(f'All instructions have to be ASCII (line {index})')
 
        self.print('Parsing:', line)

        if line.startswith('%%'):
            self.add_meta(index, line)
            return

        if line.startswith('@'):
            self.add_label(index, line)
            return

        if not re.match(r'[A-Z]{5}.*', line):
            fatal(f'Invalid instruction \'{line}\' on line {index}')

        # If it's a single instruction, just push it 
        if len(line) == 5:
            self.__instructions.append(bytes(line, 'ascii'))
            return

        data = self.split_args(line)[1:]
        buffer = bytes(line[:5], 'ascii')
        for elm in data:

            # Add an int onto the buffer
            if re.match(r'^-{0,1}[0-9]+$', elm):
                value = int(elm)
                if value < -2147483647 or value > 2147483647:
                    fatal(f'int out of bounds on line {index}')
                buffer += struct.pack('i', value)

            # Add a float value
            elif re.match(r'^-{0,1}[0-9]+\.[0-9]+$', elm):
                value = float(elm)
                buffer += struct.pack('f', value)

            # Add a single byte
            elif re.match(r'\^[0-9]+', elm):
                value = int(elm[1:])
                if value < 0 or value > 255:
                    fatal(f'byte out of bounds on line {index}')
                buffer += struct.pack('B', value)

            # Add a const value
            elif re.match(r'\[[a-z]+\]', elm): 
                if elm[1:-1] not in const_table.keys():
                    fatal(f'unknown const value on line {index}')
                buffer += const_table[elm[1:-1]]

            # Raw string
            elif re.match(r'\(.+\)', elm):
                buffer += bytes(elm[1:-1], 'ascii')
                buffer += b'\0'

            # String
            elif re.match(r'".*"', elm):

                string = self.handle_string(elm)

                buffer += struct.pack('i', len(string) + 1)
                buffer += bytes(string, 'ascii') + b'\x00'

            else:
                fatal(f'Unknown parameter \'{elm}\' on line {index}')

        self.print('Buffer:', buffer)
        self.__instructions.append(buffer)


    def handle_string(self, string: str):
        """ This will return a SCC string representation from the SA one. """ 
        
        string = string[1:-1]
        for source, dest in self.replace_map:
            string = string.replace(source, dest)

        return string

    def add_label(self, index: int, line: str):
        """ This will add a label to the instruction list. """

        self.print(f'Adding label \'{line[1:]}\'')
           
        self.__instructions.append(bytes(line, 'ascii'))


def main():
    """ If not imported, this will execute parsing the arguments and calling
    the Assembler on it's own. """ 
    
    pr = argparse.ArgumentParser(description='The SC3 assembler will assemble '
            'text based SCC "assembly" and convert it into actual bytecode '
            'the Salt Virtual Machine can execute', usage='Usage: sc3.py '
            '[OPTION]... [FILE]')
    pr.add_argument('file', type=str, help='The file to assemble')
    pr.add_argument('-o', '--output', help='name of the output file, will ' 
            'be file.scc by default')
    pr.add_argument('-d', '--debug', action='store_true')
    args = pr.parse_args()

    code: str = ''
    try:
        with open(args.file) as f:
            code = f.read()
    except FileNotFoundError:
        fatal('file not found')
    except IsADirectoryError:
        fatal('cannot read directory')

    asm = Assembler(code, debug=args.debug)
    filename = 'out.scc'
    if args.output:
        filename = args.output
    with open(filename, 'wb') as f:
        f.write(asm.render())
    
    print('done')


if __name__ == '__main__':
    main()
