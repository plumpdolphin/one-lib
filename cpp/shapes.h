/// Author: PlumpDolphin
/// Date: March 3, 2024
/// 
/// Description: 
///     Provides a basic shape primitive classes, built off of the Vector library implementation.
///     These classes offer basic transform functions to rotate, translate, and scale 
///     the shapes in both global and local space, relative to a specified origin point.
/// 
/// License:
///     The code in this file is licensed under the
///     Revised 3-Clause BSD License.
///     For details, see https://opensource.org/licenses/BSD-3-Clause


#pragma once

#define _USE_MATH_DEFINES // This allows access of the Pi definition, M_PI, from <cmath>

#include <iostream>
#include <sstream>
#include <vector> // Include vector lists
#include <cmath>

#include "vectorx.h" // Includes definition for Vector2<T> required for the shapes.





// Example usage showing manipulations on some basic shapes.
/*
    // Create a circle using floating-point units.
    Circle<float> c(5, -1, -7);

    // Create a rectangle using integer units.
    // Construction can also be deferred like this, or using a standard constructor method. 
    Rectangle<int> r;
    r.size = {4, 8}; 
    r.position = {0, 0};

    // Scale up the circle by a factor of 10.
    c.scale(10);

    // Save the position of the circle.
    auto oldPos = c.position;

    // Move circle to position (6, 7)
    c.moveTo(6, 7);

    // Scale by a factor of 2 relative to the circle's previous position
    c.scaleFrom(2, oldPos);

    // Rotate around the rectangle's position in global space by 180 degrees.
    r.rotateFrom(180, r.position); 

    // Print out the circle as a string
    std::cout << c.str() << std::endl;

    // Print out the rectangle as json
    std::cout << r.json() << std::endl;

    // Create NGon with 5 points and circumradius of 5
    NGon<float> n(5, 5);

    // Print out features of n-gon
    std::cout << n.inradius() << std::endl;
    std::cout << n.str() << std::endl;
*/



// This specifies the type used for rotation values.
// This can be changed here to your desired type such as a double for more precise calculation.
typedef float Angle;





// Utility functions
template <typename T> constexpr
T to_radians(T degrees) {
    return degrees * (M_PI / 180);
}

template <typename T> constexpr
T to_degrees(T radians) {
    return (radians * 180) / M_PI;
}

template <typename T> constexpr
Vector2<T> rotate_point(Vector2<T> position, Vector2<T> origin, Angle degrees) {
    // Check easy return case
    if (degrees == 0)
        return position;

    // Get X and Y difference of position and the rotation origin.
    Vector2<T> diff = position - origin;

    // Calculate sin and cosine of angle
    Angle radians = to_radians(degrees);
    auto cd = cos(radians);
    auto sd = sin(radians);

    // Perform the rotation on the difference vector
    // Because it is offset from the rotation origin, this is considered in local space.
    auto x = (diff.x * cd) - (diff.y * sd);
    auto y = (diff.x * sd) + (diff.y * cd);

    // Offset by rotation origin again to get global position.
    return Vector2<T>(x, y) + origin;
}




template <typename T>
class Shape2D {
public:
    // Data
    Vector2<T> position;
    Angle rotation; // Stored in degrees to take advantage of floating-point accuracy.



    // Default constructor
    Shape2D()
        : rotation(0) {}

    // Position constructor
    Shape2D(T x, T y) 
        : rotation(0), position(x, y) {}
    Shape2D(Vector2<T> position) 
        : rotation(0), position(position) {}

    // Position and rotation constructor
    Shape2D(T x, T y, T r) 
        : rotation(0), position(x, y) {}
    Shape2D(Vector2<T> position, Angle rotation) 
        : rotation(rotation), position(position) {}



    // Utility functions
    virtual T area() = 0;
    virtual T perimeter() = 0;
    
    virtual std::vector<Vector2<T>> vertices() = 0;


    // Transformative functions
    // Any function with two implementations will call the other by default.

    // This way, only one function must be implemented by the inheriting class.
    virtual void rotate(Angle degrees) { rotation += degrees; }

    virtual void rotateFrom(Angle degrees, T x, T y) { rotateFrom(degrees, Vector2<T>(x, y)); }
    virtual void rotateFrom(Angle degrees, Vector2<T> origin) {
        // Call rotate to correct object rotation angle
        rotate(degrees);
        
        // Rotate position around origin point
        position = rotate_point(position, origin, degrees);
    }

    // Moves the shape from it's current position by some offset.
    virtual void move(T x, T y)          { move(Vector2<T>(x, y)); }
    virtual void move(Vector2<T> offset) { position += offset; }

    // Moves the shape to a specific position relative to global origin.
    virtual void moveTo(T x, T y)                { moveTo(Vector2<T>(x, y)); }
    virtual void moveTo(Vector2<T> destination)  { position = destination; }

    // Scales the shape to some standard factor.
    virtual void scale(T scalar) {}

    // Scale the shape 
    virtual void scaleFrom(T scalar, T x, T y) { scaleFrom(scalar, Vector2<T>(x, y)); }
    virtual void scaleFrom(T scalar, Vector2<T> origin) { 
        // Scale shape's dimensions
        scale(scalar);
        // Find the distance between position and orgin,
        // multiply the distance by scalar, and reposition to origin
        position = ((position - origin) * scalar) + origin;
    }



    // Partial serializers
    std::string str() {
        std::stringstream ss;
        ss << "position: " << position.str() << ", rotation: " << rotation << "°";
        return ss.str();
    }

    std::string json() {
        std::stringstream ss;
        ss << "\"position\":" << position.json() << ",\"rotation\":" << rotation;
        return ss.str();
    }
};





template <typename T>
class Circle : public Shape2D<T> {
public:
    // Data
    T radius;



    // Default constructor
    Circle()
        : radius(0) {}

    // Size  constructor
    Circle(T radius)
        : radius(radius) {}
    
    // Size and position constructor
    Circle(T radius, T positionX, T positionY)
        : radius(radius), Shape2D<T>(positionX, positionY) {}
    Circle(T radius, Vector2<T> position)
        : radius(radius), Shape2D<T>(position) {}

    // Size, position, and rotation constructor
    Circle(T radius, T positionX, T positionY, Angle rotation)
        : radius(radius), Shape2D<T>(positionX, positionY, rotation) {}
    Circle(T radius, Vector2<T> position, Angle rotation)
        : radius(radius), Shape2D<T>(position, rotation) {}



    // Utility functions
    constexpr T area() override { return M_PI * radius * radius; }
    constexpr T perimeter() override { return 2 * M_PI * radius; }

    // Call to render the circle to a default, fixed number of verticies
    constexpr std::vector<Vector2<T>> vertices() override { return vertices(64); }

    // Renders the circle to a list of vertices of a specified count
    constexpr std::vector<Vector2<T>> vertices(size_t resolution) {
        // Get central angle for given resolution
        Angle angle_central = -360 / static_cast<Angle>(resolution);

        // Defines the container for resulting verticies
        std::vector<Vector2<T>> vertex_list(resolution);

        // Iterate through each point and add to vertex list.
        for (size_t i = 0; i < resolution; i++) {
            // Calculate degrees of rotation for given vertex
            Angle v_rotation = angle_central * i;

            // Rotate point around circle's center
            vertex_list[i] = rotate_point(Vector2<T>(0, radius), Shape2D<T>::position, v_rotation);
        }

        // Rotate vertices by shapes's rotation value
        if (rotation != 0)
            for (auto &v: vertex_list) 
                v = rotate_point(v, Shape2D<T>::position, Shape2D<T>::rotation);

        // Return rotated vertices list
        return vertex_list;
    }


    // Transformative functions
    // Scales the circle from it's own origin point
    void scale(T scalar) override { radius *= scalar; }



    // Serializers
    std::string str() {
        std::stringstream ss;
        ss << "Circle { radius: " << radius << ", " << Shape2D<T>::str() << " }";
        return ss.str();
    }

    std::string json() {
        std::stringstream ss;
        ss << "{\"radius\":" << radius << "," << Shape2D<T>::json() << "}";
        return ss.str();
    }
};





template <typename T>
class Rectangle : public Shape2D<T> {
public:
    // Data
    Vector2<T> size;



    // Default constructor
    Rectangle()
        : size(0, 0) {}

    // Size constructor
    Rectangle(T width, T height)
        : size(width, height) {}
    
    // Size and position constructor
    Rectangle(T width, T height, T positionX, T positionY)
        : size(width, height), Shape2D<T>(positionX, positionY) {}
    Rectangle(T width, T height, Vector2<T> position)
        : size(width, height), Shape2D<T>(position) {}
    Rectangle(Vector2<T> size, T positionX, T positionY)
        : size(size), Shape2D<T>(positionX, positionY) {}
    Rectangle(Vector2<T> size, Vector2<T> position)
        : size(size), Shape2D<T>(position) {}

    // Size, position, and rotation constructor
    Rectangle(T width, T height, T positionX, T positionY, Angle rotation)
        : size(width, height), Shape2D<T>(positionX, positionY, rotation) {}
    Rectangle(T width, T height, Vector2<T> position, Angle rotation)
        : size(width, height), Shape2D<T>(position, rotation) {}
    Rectangle(Vector2<T> size, T positionX, T positionY, Angle rotation)
        : size(size), Shape2D<T>(positionX, positionY, rotation) {}
    Rectangle(Vector2<T> size, Vector2<T> position, Angle rotation)
        : size(size), Shape2D<T>(position, rotation) {}



    // Utility functions
    constexpr T area() override { return size.x * size.y; }
    constexpr T perimeter() override { return (size.x * 2) + (size.y * 2); }

    constexpr std::vector<Vector2<T>> vertices() override {
        // Calculate half of size for offsets
        Vector2<T> hs = size / 2;

        // Provider simpler aliases for base class variables
        auto position = Shape2D<T>::position;
        auto rotation = Shape2D<T>::rotation;

        // Define vertices
        std::vector<Vector2<T>> vertex_list = {
            position - hs,                      // Top left
            Vector2<T>(hs.x, -hs.y) + position, // Top right
            hs + position,                      // Bottom right
            Vector2<T>(-hs.x, hs.y) + position, // Bottom left
        };

        // Rotate all vertices by shape's rotation value
        if (rotation != 0)
            for (auto &v: vertex_list) 
                v = rotate_point(v, position, rotation);

        // Return rotated vertices list
        return vertex_list;
    }


    // Transformative functions
    void scale(T scalar) override { size *= scalar; }
    void scale(T x, T y) { 
        size.x *= x; 
        size.y *= y; 
    }
    void scale(Vector2<T> scale) { size *= scale; }



    // Serializers
    std::string str() {
        std::stringstream ss;
        ss << "Rectangle { size: " << size.str() << ", " << Shape2D<T>::str() << " }";
        return ss.str();
    }

    std::string json() {
        std::stringstream ss;
        ss << "{\"size\":" << size.json() << "," << Shape2D<T>::json() << "}";
        return ss.str();
    }
};





// This class represents regular N-Gons only.
// For asymmetrical N-gons, use the Polygon class. (Coming soon)
template <typename T>
class NGon : public Shape2D<T> {
public:
    // Data
    size_t N; // Number of vertices
    T radius; // This represents the N-Gon's circumradius.



    // Default constructor
    NGon()
        : N(0), radius(0) {}

    // Count constructor
    NGon(size_t count)
        : N(count), radius(0) {}

    // Count and size constructor
    NGon(size_t count, T radius)
        : N(count), radius(radius) {}

    // Size and position constructor
    NGon(size_t count, T radius, T positionX, T positionY)
        : N(count), radius(radius), Shape2D<T>(positionX, positionY) {}
    NGon(size_t count, T radius, Vector2<T> position)
        : N(count), radius(radius), Shape2D<T>(position) {}

    // Count, size, position, and rotation constructor
    NGon(size_t count, T radius, T positionX, T positionY, Angle rotation)
        : N(count), radius(radius), Shape2D<T>(positionX, positionY, rotation) {}
    NGon(size_t count, T radius, Vector2<T> position, Angle rotation)
        : N(count), radius(radius), Shape2D<T>(position, rotation) {}



    // Utility functions
    constexpr Angle centralAngle() { return 360 / N; }
    constexpr Angle innerAngle()   { return 180 - centralAngle(); }
    
    constexpr T edge() { return 2 * sin(M_PI / N) * radius; }
    constexpr T circumradius() { return radius; }
    constexpr T inradius() { return radius * cos(M_PI / N); }

    constexpr T perimeter() override { return edge() * N; }
    constexpr T area() override { 
        T e = edge();
        return (N * e * e) / (4 * tan(M_PI / N)); 
    }

    constexpr std::vector<Vector2<T>> vertices() {
        // Store value of central angle
        auto angle_central = -centralAngle();

        // Defines the container for resulting verticies
        std::vector<Vector2<T>> vertex_list(N);

        // Iterate through each point and add to vertex list.
        for (size_t i = 0; i < N; i++) {
            // Calculate degrees of rotation for given vertex
            Angle v_rotation = angle_central * i;

            // Rotate point around circle's center
            vertex_list[i] = rotate_point(Vector2<T>(0, radius), Shape2D<T>::position, v_rotation);
        }

        // Rotate vertices by shapes's rotation value
        if (rotation != 0)
            for (auto &v: vertex_list) 
                v = rotate_point(v, Shape2D<T>::position, Shape2D<T>::rotation);

        // Return rotated vertices list
        return vertex_list;
    }



    // Transformative functions
    void scale(T scalar) override { radius *= scalar; }



    // Serializers
    std::string str() {
        std::stringstream ss;
        ss << "NGon { N: " << N << ", radius: " << radius << ", " << Shape2D<T>::str() << " }";
        return ss.str();
    }

    std::string json() {
        std::stringstream ss;
        ss << "{\"N\":" << N << ",\"radius\":" << radius << "," << Shape2D<T>::json() << "}";
        return ss.str();
    }
};