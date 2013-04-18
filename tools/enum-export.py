#!/usr/bin/python
# Let's not waste our life copying-pasting-editing
# to export enums to Boost::Python, right?
#
# Takes the enum declaration as input and
# outputs the corresponding code.
# You need to pass it the prefix to strip
# (i.e. "STATUS" for the Status enum) as
# the first argument.
import re, sys

head =  '  // ENUM: %(name)s\n'+\
        '  enum_<openni::%(name)s>("%(name)s")'
value = '    .value("%(name)s", openni::%(id)s)'
foot =  '  ;'

def err(msg):
  raise Exception(msg)

# Extract the arguments
pref = sys.argv[1]+'_'

# Extract the head & contents
m = re.match(r'^\s*enum\s+(\w+)\s*\{(.+)\}\s*$', sys.stdin.read(), re.S)
if not m: err('Invalid enum!')
print('\n')

# Output head
print(head % {'name': m.group(1)})

# Process values
for e in m.group(2).split(','):
  n = e.split('=')[0].strip()
  if not n.startswith(pref): err('Invalid prefix!')
  print(value % {'name': n[len(pref):], 'id': n})

# Output foot
print(foot)
