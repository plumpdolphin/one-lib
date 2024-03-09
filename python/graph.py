'''
Author: PlumpDolphin
Date: March 3, 2024

Description: 
    Provides implementations of some basic graph structures.

License:
    The code in this file is licensed under the
    Revised 3-Clause BSD License.
    For details, see https://opensource.org/licenses/BSD-3-Clause
'''



class Graph:

    def __init__(self, size, directed = True): 
        self.directed = directed

        # Initialize lists for easy writes
        self.vertexes = [None for _ in range(size)]
        self.edges    = [[] for _ in range(size)]

    def add_edge(self, a, b):
        if b not in self.edges[a]:
            self.edges[a].append(b)

        if not self.directed:
            if a not in self.edges[b]:
                self.edges[b].append(a)