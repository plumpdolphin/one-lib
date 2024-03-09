#include <iostream>

#include "shapes.h"
#include "graph.h"

int main(void) 
{
    /*
    */
    
    // Create a circle using floating-point units.
    Circle<float> c(5, 0, 0);

    // Create a rectangle using integer units.
    // Construction can also be deferred like this, or using a standard constructor method. 
    Rectangle<int> r;
    r.size = {4, 8}; 
    r.position = {5, 5};

    r.rotate(45);

    r.rotateFrom(30, 10, 10);

    std::cout << r.str() << std::endl;

    NGon<float> n(5, 5);
    std::cout << n.inradius() << std::endl;
    std::cout << n.str() << std::endl;

    auto vertices = n.vertices();
    for (auto &v: vertices)
        std::cout << v.str() << std::endl;


    c.rotate(-90);
    vertices = c.vertices(7);
    for (auto &v: vertices)
        std::cout << v.str() << std::endl;

    /*
    ListGraph<int, true> g(5);

    g.verts = {69, 420, 69, 420, 69};

    g.connect(0, 1);
    g.connect(0, 2);
    g.connect(0, 3);
    g.connect(0, 4);
    g.connect(4, 0);
    g.connect(4, 1);
    g.connect(4, 2);
    g.connect(4, 3);

    g.print();

    g.remove(0, 1);
    //g.removeVertex(3);

    g.print();
    */

    return 0;
}