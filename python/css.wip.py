'''
Author: PlumpDolphin
Date: February 4, 2024

Description: 
    Provides functions for parsing, pretty printing, and minifying CSS3.

License:
    The code in this file is licensed under the
    Revised 3-Clause BSD License.
    For details, see https://opensource.org/licenses/BSD-3-Clause
'''



import re



class Rule:
    def __init__(self, name: str, attrs = {}):
        self.name = name
        self.attrs = attrs

class AtRule:
    def __init__(self, keyword: str, rules = {}):
        self.keyword = keyword
        self.rules = rules

class ConditionalRule:
    def __init__(self, conditions, rules = {}):
        self.conditions = conditions
        self.rules = rules

class NamedAtRule:
    def __init__(self, name: str, rules = {}):




class CSSDocument:
    @classmethod
    def from_str(cls, str):
        # Get all rules
        