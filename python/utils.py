# utility functions for SusyNt python access
#
# davide.gerbaudo@gmail.com
# June 2014

class Memoize :
    """A class to cache cpu-intensive functions.
    Arguments must be hashable.
    See for example
    http://stackoverflow.com/questions/1988804/what-is-memoization-and-how-can-i-use-it-in-python
    """
    def __init__(self, f) :
        self.f = f
        self.memo = {}
    def __call__(self, *args) :
        if not args in self.memo : self.memo[args] = self.f(*args)
        return self.memo[args]

def enum_from_header(filename, enumName) :
    """
    Given a c header file, extract the enum as a dict of key:values.
    From:
    https://mail.python.org/pipermail/python-list/2009-August/548422.html
    Modified to also get the enum name.
    """
    verbose = False
    file_data = open(filename).read()
    # Remove comments and preprocessor directives
    file_data = ' '.join(line.split('//')[0].split('#')[0] for line in file_data.splitlines())
    file_data = ' '.join(re.split(r'\/\*.*?\*\/', file_data))
    # Look for enums: In the first { } block after the keyword "enum"
    enums = [(text.split('{')[0].replace('enum','').strip(), text.split('{')[1].split('}')[0])
             for text in re.split(r'\benum\b', file_data)[1:]]
    enum = dict()
    for enum_name, enum_keyvals in enums:
        last_value = -1
        for key_name in enum_keyvals.split(','):
            if '=' in key_name:
                key_name, key_value = key_name.split('=')
                key_value = int(key_value, 0)
            else:
                key_value = last_value + 1
            last_value = key_value
            key_name = key_name.strip()
            if enum_name == enumName : enum[key_name] = key_value
            if verbose : print '%s = %d' % (key_name, key_value)
        if verbose : print
    return enum
