/// Author: PlumpDolphin
/// Date: February 29, 2024
/// 
/// Description: 
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



// Example usage showing construction and arithmetic syntax.
/*
    // Dimension values default to 0
    Vector4<int> a; // (0, 0, 0, 0)

    auto b = Vector4<int>(0, 1, 2, 3);
    
    // Performing arithmetic across vectors
    a -= b;  // Subtract values of B from A 
    a <<= b; // Left-shift values of A by values of B places

    // Performing scalar arithmetic on vectors
    a *= 3; // Multiplies 3 times each value
    b -= 1; // Subtracts 1 from each value

    // Formatting vector to debug string format or standard JSON
    std::cout << a.str() << std::endl;
    std::cout << b.json() << std::endl;

    // Formatting evaluated expressions to debug string or to JSON
    std::cout << (a * b).json() << std::endl;
*/



// Generator Constants
#define DEFAULT_MEMBER_VALUE 0



// Defines which operators should be implemented by the Vector classes
// Note: the operators assignment-variant is implemented as well.
// For example, when you declare `+` here, the `+=` operator will also be created.
#define OPERATORS(func, ...) \
    func(+, __VA_ARGS__)     \
    func(-, __VA_ARGS__)     \
    func(*, __VA_ARGS__)     \
    func(/, __VA_ARGS__)     \
    func(%, __VA_ARGS__)     \
    func(&, __VA_ARGS__)     \
    func(|, __VA_ARGS__)     \
    func(^, __VA_ARGS__)     \
    func(<<, __VA_ARGS__)    \
    func(>>, __VA_ARGS__)



// Formatting functions
// This section is a bunch of utility functions.
// The ones ending in 0 or 1 are used as compile-time comparisons to treat the end of the
// dimensions list differently than prior elements. This is used for comma-delimination mostly.
#define _VARIABLE_DEF(str, is_end, ...) T str;

#define _PARAMETER_DEF0(str) T str,
#define _PARAMETER_DEF1(str) T str
#define _PARAMETER_DEF(str, is_end, ...) _PARAMETER_DEF##is_end(str)

#define _MEMBER_INITIALIZER_DEF0(str, value) str (value),
#define _MEMBER_INITIALIZER_DEF1(str, value) str (value)
#define _MEMBER_INITIALIZER_DEF(str, is_end, ...) _MEMBER_INITIALIZER_DEF##is_end(str, str)
#define _MEMBER_INITIALIZER_DEFAULT(str, is_end, ...) _MEMBER_INITIALIZER_DEF##is_end(str, DEFAULT_MEMBER_VALUE)

#define _INSERTION_STR0(str) str << ", " <<
#define _INSERTION_STR1(str) str << ")";
#define _INSERTION_STR(str, is_end, ...) _INSERTION_STR##is_end(str)

#define _INSERTION_JSON0(str) str << "," <<
#define _INSERTION_JSON1(str) str << "]";
#define _INSERTION_JSON(str, is_end, ...) _INSERTION_JSON##is_end(str)



// Operator helper functions
// Performs scaling on `this` vector
#define _SCALE_SCALAR_THIS(dim, is_end, op) dim op##= scalar;
#define _SCALE_OTHER_THIS(dim, is_end, op)  dim op##= other.dim;

// Performs scaling on temporary Vector
#define _SCALE_SCALAR_NEW(dim, is_end, op) v.dim = dim op scalar;
#define _SCALE_OTHER_NEW(dim, is_end, op)  v.dim = dim op other.dim;



/*** Defines template for creating operator implementations for scalar values and other Vector types. ***/
#define _VECTOR_OP(op, dim)                                     \
    /* Create new vector, scales all dimensions, and returns */ \
    Vector##dim operator op(const T& scalar) {                  \
        Vector##dim v;                                          \
        _VECTOR_DIMS_##dim( _SCALE_SCALAR_NEW, op )             \
        return v;                                               \
    }                                                           \
                                                                \
    /* Scales all dimensions of first vecotr and returns it */  \
    Vector##dim& operator op##=(const T& scalar) {              \
        _VECTOR_DIMS_##dim( _SCALE_SCALAR_THIS, op )            \
        return *this;                                           \
    }                                                           \
                                                                \
    /* Create new vector, scales all dimensions, and returns */ \
    Vector##dim operator op(const Vector##dim& other) {         \
        Vector##dim v;                                          \
        _VECTOR_DIMS_##dim( _SCALE_OTHER_NEW, op )              \
        return v;                                               \
    }                                                           \
                                                                \
    /* Scales all dimensions of first vecotr and returns it */  \
    Vector##dim& operator op##=(const Vector##dim& other) {     \
        _VECTOR_DIMS_##dim( _SCALE_OTHER_THIS, op )             \
        return *this;                                           \
    }



/*** Defines template for each Vector class implementation ***/
#define _VECTOR_DEF(dim)                                           \
    template<typename T>                                           \
    class Vector##dim {                                            \
    public:                                                        \
        /* Declares all of the dimension variables */              \
        _VECTOR_DIMS_##dim( _VARIABLE_DEF )                        \
                                                                   \
        /* Builds the Vector constructor functions */              \
        Vector##dim ()                                             \
            : _VECTOR_DIMS_##dim( _MEMBER_INITIALIZER_DEFAULT ) {} \
        Vector##dim ( _VECTOR_DIMS_##dim( _PARAMETER_DEF ) )       \
            : _VECTOR_DIMS_##dim( _MEMBER_INITIALIZER_DEF ) {}     \
                                                                   \
        /* Implements all of the supported operators */            \
        OPERATORS(_VECTOR_OP, dim)                                 \
                                                                   \
        /* Imlements string conversion for all dimensions */       \
        std::string str() {                                        \
            std::stringstream ss;                                  \
            ss << "(" << _VECTOR_DIMS_##dim(_INSERTION_STR);       \
            return ss.str();                                       \
        }                                                          \
                                                                   \
        /* Imlements JSON conversion for all dimensions */         \
        std::string json() {                                       \
            std::stringstream ss;                                  \
            ss << "[" << _VECTOR_DIMS_##dim(_INSERTION_JSON);      \
            return ss.str();                                       \
        }                                                          \
    };






// Define each Vector size's dimension names

// PARAMS:
// 0: Name of dimension  (x, y, z, etc)
// 1: Is last element    (0 = False, 1 = True)
// 2: Optional arguments (Set to __VA_ARGS__)
#define _VECTOR_DIMS_2(func, ...) \
    func(x, 0, __VA_ARGS__)       \
    func(y, 1, __VA_ARGS__)

#define _VECTOR_DIMS_3(func, ...) \
    func(x, 0, __VA_ARGS__)       \
    func(y, 0, __VA_ARGS__)       \
    func(z, 1, __VA_ARGS__)

#define _VECTOR_DIMS_4(func, ...) \
    func(x, 0, __VA_ARGS__)       \
    func(y, 0, __VA_ARGS__)       \
    func(z, 0, __VA_ARGS__)       \
    func(w, 1, __VA_ARGS__)

// Example: Adding your own vector is as simple as specifying the letters for each dimension.
//          Then, call the _VECTOR_DEF() macro with the same dimension number below.
/*
#define _VECTOR_DIMS_5(func, ...) \
    func(x, 0, __VA_ARGS__)       \
    func(y, 0, __VA_ARGS__)       \
    func(z, 0, __VA_ARGS__)       \
    func(w, 0, __VA_ARGS__)       \
    func(a, 1, __VA_ARGS__)
*/



// Build Vector class definitions
_VECTOR_DEF(2)
_VECTOR_DEF(3)
_VECTOR_DEF(4)

// Example: This would be the implementation for the above 5th-dimensional class.
/*
_VECTOR_DEF(5)
*/






// Clean up all definitions to prevent collisions or bugs with other broken headers.
// Not necessarily required, but good practice. I don't want that blood on my hands.
#undef DEFAULT_MEMBER_VALUE
#undef OPERATORS

#undef _VARIABLE_DEF

#undef _PARAMETER_DEF
#undef _PARAMETER_DEF0
#undef _PARAMETER_DEF1

#undef _MEMBER_INITIALIZER_DEF
#undef _MEMBER_INITIALIZER_DEF0
#undef _MEMBER_INITIALIZER_DEF1
#undef _MEMBER_INITIALIZER_DEFAULT

#undef _INSERTION_STR
#undef _INSERTION_STR0
#undef _INSERTION_STR1
#undef _INSERTION_JSON
#undef _INSERTION_JSON0
#undef _INSERTION_JSON1

#undef _SCALE_OTHER_NEW
#undef _SCALE_OTHER_THIS
#undef _SCALE_SCALAR_NEW
#undef _SCALE_SCALAR_THIS

#undef _VECTOR_OP
#undef _VECTOR_DEF