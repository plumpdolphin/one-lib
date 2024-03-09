/// Author: PlumpDolphin
/// Date: February 29, 2024
/// 
/// Description: 
/// TODO: Update description
///     Provides Vector2, Vector3, and Vector4 classes designed for operations on cartesian coordinate systems.
///     The provided macros make it simple to extend this functionality to other fixed Vector sizes with custom
///     dimension labels.
/// 
/// License:
///     The code in this file is licensed under the
///     Revised 3-Clause BSD License.
///     For details, see https://opensource.org/licenses/BSD-3-Clause


#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>

#include <vector>
#include <list>
#include <unordered_set>



template <typename T, bool directed>
class ListGraph {
public:
    // Data
    std::vector<T> verts;
    std::vector<std::vector<size_t>> edges;

    // Constructors
    ListGraph(size_t size) {
        // Initialize vetrtex and edge vectors
        verts.resize(size);
        edges.resize(size);
    }

    // Transformative functions
    void add(T v) {
        // Increase vertex count and add to vertex vector
        verts.push_back(v);

        // Add new empty set to edge lists
        edges.push_back(std::vector<size_t>());
    }

    // TODO: Redo using auto iterators
    void remove(size_t idx) {
        // Modify edge connections
        for (size_t i = 0; i < edges.size(); i++) {
            // Set values to check for removal for this edge list
            bool remove = false;
            size_t remove_idx = 0;

            // Iterate each edge
            for (size_t j = 0; j < edges[i].size(); j++) {
                // Store edge's connected vertex for re-use
                const T value = edges[i][j];

                // If connected vertex is the vertex to-be-removed, save index for removal
                if (value == idx) {
                    remove = true;
                    remove_idx = j;
                }

                // Decrease vertex index if it is greater than the removed index
                edges[i][j] -= (size_t)( value > idx );
            }

            if (remove) {
                // Remove edge
                edges[i].erase( edges[i].begin() + remove_idx );
            }
        }

        // Remove index
        verts.erase( verts.begin() + idx );
        edges.erase( edges.begin() + idx );
    }

    // TODO: Figure out why ranges are wierd.
    void remove(size_t start_idx, size_t end_idx) {
        // Remove unnecessary values and edge lists
        verts.erase( verts.begin() + start_idx, verts.begin() + end_idx );
        edges.erase( edges.begin() + start_idx, edges.begin() + end_idx );
        
        for (auto &edge_list : edges) {
            // Remove elements within the specified range
            edge_list.erase(
                std::remove_if(
                    edge_list.begin(),
                    edge_list.end(),
                    [start_idx, end_idx](const T& value) {
                        return value >= start_idx && value < end_idx;
                    }
                ),
                edge_list.end()
            );

            // Adjust the values of the remaining elements
            for (auto &value : edge_list) {
                if (value > end_idx) {
                    value -= (end_idx - start_idx + 1);
                }
            }
        }
    }

    void connect(size_t a, size_t b) {
        // Add b to a's edge list
        // Does not append if already in list
        if (std::find(edges[a].begin(), edges[a].end(), b) == edges[a].end()) 
            edges[a].push_back(b);

        // If graph type is specified as not directed,
        // Adds a to b's edge list as well 
        if constexpr(!directed) {
            if (std::find(edges[b].begin(), edges[b].end(), a) == edges[b].end()) 
                edges[b].push_back(a);
        }
    }

    void disconnect(size_t a, size_t b) {
        // Removes b from a's edge list if found
        auto idx = std::find(edges[a].begin(), edges[a].end(), b);
        if (idx == edges[a].end())
            edges[a].erase(idx);

        // If graph type is specified as not directed,
        // Removes a from b's edge list as well 
        if constexpr(!directed) {
            idx = std::find(edges[b].begin(), edges[b].end(), a);
            if (idx == edges[b].end())
                edges[b].erase(idx);
        }
    }

    // Serializers
    void print() {
        // Iterate over and print all edge connections
        for (size_t i = 0; i < edges.size(); i++) {
            std::cout << i;
            for (auto v: edges[i])
                std::cout << "->" << v;
            std::cout << std::endl;
        }
    }
};