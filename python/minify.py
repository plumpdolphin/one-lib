import re

def css(str):
    # Remove newlines
    str = str.replace('\n', '')
    
    # Remove spacing around special characters
    str = re.sub(r'\s*([{},:;^|$~>()/\[\]])\s*', r'\1', str)

    # Remove comments
    str = re.sub(r'/\*.*?\*/', '', str)

    # Remove trailing semicolons
    str = str.replace(';}', '}')

    # Shorten relative URIs
    str = re.sub('[^\\.]\\./', '', str)

    # Add spacing around media ands
    str = re.sub(r'(\)?)\s*and\s*(\(?)', r'\1 and \2', str)

    return str