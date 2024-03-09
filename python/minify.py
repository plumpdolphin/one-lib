'''
Author: PlumpDolphin
Date: January 30, 2024

Description: 
    Provides functions for minifying a variety of languages.

License:
    The code in this file is licensed under the
    Revised 3-Clause BSD License.
    For details, see https://opensource.org/licenses/BSD-3-Clause
'''



import re



def seek(str, pattern, start_idx):
    pl = len(pattern)
    for i in range(start_idx, len(str)):
        if str[i : pl + i] == pattern:
            return i
    return None


class CSSDocument:
    class Rule:
        def __init__(self):
            self.attrs = {}

    class Media:
        def __init__(self):
            self.rules = []

    def __init__(self):
        self.rules = []

    def from_str(cls, str):
        
        


def css(str):
    '''
    class attr = [key, value]
    class rule = [name, attrs{}]
    class media = [cond, rules[]]
    class frame = [duration, attrs{}]
    class keyframes = [name, frames[]]
    class document = [rules[], medias[], keyframes[]]
    '''

    # Parse document into standardized format
    doc = {
        'rules': {
            'body': {
                'color': 'white',
                'width': '100%'
            }
        },
        'queries': [
            {
                'conditions': {
                    'screen': 'any',
                    'max-width': '800px'
                },
                'rules': {
                    'body': {
                        'background': 'black'
                    }
                }
            }
        ],
        'animations': {
            'fade-in': {
                '0%': {

                }
            }
        }
    }

    # Perform document structure optimizations

    # Rebuild text from standardized document



def single_pass(str):
    '''
    {}:;()%
    'and'
    /*  */
    '''

    whitespace   = [' ', '\t', '\n']
    no_space = ['{','}',',',':',';','^','|','$','~','>','(',')','/','[',']']

    r = ''

    # Pass
    i = 0
    while i < len(str):
        # Handle comments
        if str[i : i+2] == '/*':
            while str[i : i+2] != '*/':
                i += 1
            i += 2
        
        # Handle quotes
        if str[i] == '"':
            j = i
            
            has_space = False

            while str[j] != '"':
                has_space |= (str[j] in whitespace)
                j += 1

            if j == len(str):
                i += 1 # Error unclosed quote, ignore.
                continue
            elif has_space:
                r += str[i : j] # Include entire quoted section
            else:
                r += str[i+1 : j-1] # Include quoted section without the quotations
            
            i = j
            continue

        # Handle whitespace
        if str[i] in whitespace:
            # Expand whitespace
            j = i
            while str[j] in whitespace:
                j += 1

            # Only before and after "and" needs space
            if (str[i - 3 : i-1] == 'and' or
                (str[j + 1 : j + 3] == 'and' and str[i + 4] in whitespace)):
                r += str[i]
            
            i += 1
            continue


        
        # Handle trailing semi-colons
        if str[i] == ';':
            for j in range(i, len(str)):
                if str[j] in whitespace:
                    continue

                if str[j] == '}':
                    r += '}'
                    i = j
                    break
                else:
                    i = j
                    break
            i += 1
            continue

                
        r += str[i]
        i += 1

    print(r)

    return r



def something(str):
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



if __name__ == '__main__':
    css('/*Hello World*/body { hello: korea; font-family: "Arial", "Comic Sans", helvetica;}')