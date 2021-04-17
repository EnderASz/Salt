#!/usr/bin/python3.9
""" Assemble all .sa files in the tests """

import os

folders = os.listdir()
to_remove = ['svm_test.py', 'README.md']
for i in to_remove:
    folders.remove(i)
folders = [s for s in folders if not s.startswith('.')]

for folder in folders:
    files = os.listdir(folder)
    to_asm = [s.split('.')[0] for s in files if s.endswith('.sa')]
    to_asm = [folder + '/' + s for s in to_asm]

    for asm in to_asm:
        print('Assembling {asm}.sa')
        os.system(f'sasm {asm}.sa -o {asm}.scc')

