import sys

# open filepath
filepath = sys.argv[1]
file = open(filepath, 'r')
str = file.read()
 
# convert the string to binary
bin_data = ''.join(format(ord(x), 'b') for x in str)

# display the string
print bin_data
