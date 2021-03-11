# Fetch information from the dev.json file

import json
import sys

if len(sys.argv) < 2:
    print('[!] Please provide argument in build.py')
    exit(1)


with open('dev.json') as f:
    dev = json.load(f)

if sys.argv[1] == 'build':
    dev['build'] += 1
    print(f"#define SVM_VERSION \"svm {dev['major']}.{dev['minor']} build {dev['build']}\"")

with open('dev.json', 'w') as f:
    json.dump(dev, f, indent=4)
