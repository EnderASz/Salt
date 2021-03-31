# Fetch information from the version.conf file.
# It is guaranteed to find the major, minor and build
# fields in the configuration.
import sys

with open(sys.argv[1]) as f:
    file = f.readlines()

info = {}

for i in file:
    if i.strip() and not i.startswith('#'):
        strings = [s.strip() for s in i.split('=')]
        info[strings[0]] = strings[1]

print(f"{info['major']}.{info['minor']}\\ build\\ {info['build']}")

# Bump build
info['build'] = str(int(info['build']) + 1)

# Write the result back to the file with the bumped build version
with open(sys.argv[1], 'w') as f:
    f.write('# The version file for SVM builds. Used in the Makefile\n\n')
    for k, v in info.items():
        f.write(f'{k}={v}\n')


