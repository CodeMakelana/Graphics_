#include "Matrix.h"
#include "Vector.h"
#include "Shape.h"
#include "Triangle.h"
#include "Square.h"

#include <iostream>

#include <sstream>

// void testVector();
void testMatrix();
// void testSquare();
// void testTriangle();

int main(int argc, char const *argv[])
{

    // testVector();
    testMatrix();
    // testSquare();
    // testTriangle();
    
    return 0;

}

void testVector() {
     std::cout << "=====================Testing Vector=====================" << std::endl;

    // Default constructor
    Vector<3> vec1 = Vector<3>();
    std::cout << "Vec1 created:" << std::endl;
    vec1.print();
    std::cout << "Size of vec1 is " << vec1.getN() << std::endl;

    Vector<3> vec2 = Vector<3>();
    std::cout << "Vec2 has been created" << std::endl;
    vec2.print();
    std::cout << "Size of vec2 is " << vec2.getN() << std::endl;

    //Initializer list constructor
    Vector<3> vec3 = Vector<3>{1.0, 2.0, 3.0};
    std::cout << "Vec3 has been created using initializer list" << std::endl;
    vec3.print();
    

    //Vector mulitpilication
    float x = 2.0;
    Vector<3> vec4 = vec3 * x;
    std::cout << "Vec4 after scaling up by " << x << std::endl;
    vec4.print();

     // Copy constructor
    Vector<3> vec5 = Vector<3>(vec3);
    std::cout << "Vec5 (copy of vec3):" << std::endl;
    vec5.print();

    // Assignment operator
    vec1 = vec3;
    std::cout << "Vec1 after assignment from vec3:" << std::endl;
    vec1.print();

    // Addition
    Vector<3> vec6 = vec3 + vec4;
    std::cout << "Vec6 (vec3 + vec4):" << std::endl;
    vec6.print();

    // Subtraction
    Vector<3> vec7 = vec4 - vec3;
    std::cout << "Vec7 (vec4 - vec3):" << std::endl;
    vec7.print();

    // Dot product
    float dotProduct = vec3 * vec4;
    std::cout << "Dot product of vec3 and vec4: " << dotProduct << std::endl;

    // Magnitude
    float magnitudeVec3 = vec3.magnitude();
    std::cout << "Magnitude of vec3: " << magnitudeVec3 << std::endl;

    // Cross product
    Vector<3> crossProduct = vec3.crossProduct(vec4);
    std::cout << "Cross product of vec3 and vec4:" << std::endl;
    crossProduct.print();

    // Unit vector
    Vector<3> unitVec3 = vec3.unitVector();
    std::cout << "Unit vector of vec3:" << std::endl;
    unitVec3.print();

    // Cross product
    Vector<3> a = Vector<3>{1.0, 0.0, 0.0};
    Vector<3> b = Vector<3>{0.0, 1.0, 0.0};
    Vector<3> cross = a.crossProduct(b);
    std::cout << "Cross product (1,0,0) x (0,1,0) [expect 0,0,1]:" << std::endl;
    cross.print();

    std::cout << "=====================Vector testing complete=====================" << std::endl;
    std::cout << std::endl;
}

void testMatrix() {
    std::cout << "=====================Testing Matrix=====================" << std::endl;
    // Default constructor
    Matrix<2, 3> mat1 = Matrix<2, 3>();
    std::cout << "Mat1 created:" << std::endl;
    mat1.print();
    std::cout << "Size of mat1 is " << mat1.getN() << "x" << mat1.getM() << std::endl;

    //testing parameterized constructor
    float **arr = new float*[2];
    for (int i = 0; i < 2; i++) {
        arr[i] = new float[3];
        for (int j = 0; j < 3; j++) {
            arr[i][j] = i + j; // Fill with some values
        }
    }

    Matrix<2, 3> mat2 = Matrix<2, 3>(arr);
    std::cout << "Mat2 created using parameterized constructor:" << std::endl;
    mat2.print();
    std::cout << "Size of mat2 is " << mat2.getN() << "x" << mat2.getM() << std::endl;

    //testing copy constructor
    Matrix<2, 3> mat3 = Matrix<2, 3>(mat2);
    std::cout << "Mat3 (copy of mat2):" << std::endl;
    mat3.print();

    //testing assignment operator
    mat1 = mat2;
    std::cout << "Mat1 after assignment from mat2:" << std::endl;
    mat1.print();

    //testing the matrix multiplication
    Matrix<2,2> mat4;
    std::cout << "Mat4 is result of mat2 times mat3. Matrix multiplication." << std::endl;
    mat4 = mat2 * (~mat3);
    mat4.print();

    //testing the determinant
     std::cout << "Determinant of matrix 4 is " << mat4.determinant() << std::endl;

     std::cout << "=====================Matrix testing complete=====================" << std::endl;
     std::cout << std::endl;
}

void testSquare() {
    std::cout << "=====================Testing Square=====================" << std::endl;

    Vector<3> center = Vector<3>{0.0, 0.0, 0.0};
    float height = 2.0;
    float width = 2.0;

    Square<3> square1 = Square<3>(center, height, width);
    std::cout << "Square1 created using center, height, and width:" << std::endl;
    square1.print();

    Vector<3> tl = Vector<3>{-1.0, 1.0, 0.0};
    Vector<3> tr = Vector<3>{1.0, 1.0, 0.0};
    Vector<3> br = Vector<3>{1.0, -1.0, 0.0};
    Vector<3> bl = Vector<3>{-1.0, -1.0, 0.0};

    Square<3> square2 = Square<3>(tl, tr, br, bl);
    std::cout << "Square2 created using corner points:" << std::endl;
    square2.print();

    Square<3> square3 = Square<3>(square1);
    std::cout << "Square3 (copy of square1):" << std::endl;
    square3.print();

    //testing the "Square<n>& operator*=(const Matrix<n,n>&)" function
    Matrix<3,3> transformation;
    transformation[0][0] = 1.0; transformation[0][1] = 0.0; transformation[0][2] = 1.0; // Translation
    transformation[1][0] = 0.0; transformation[1][1] = 1.0; transformation[1][2] = 1.0; // Translation
    transformation[2][0] = 0.0; transformation[2][1] = 0.0; transformation[2][2] = 1.0; // Homogeneous coordinate

    square1 *= transformation;
    std::cout << "Square1 after *= transformation:" << std::endl;
    square1.print();

    //testing the "Square<n>* operator*(const Matrix<n,n>&) const" function
    Square<3>* square4 = square2 * transformation;
    std::cout << "Square4 (result of square2 * transformation):" << std::endl;
    square4->print();

    //testing the "float* getPoints() const" function
    float* points = square2.getPoints();
    std::cout << "Points of square2:" << std::endl;
    for (int i = 0; i < square2.getNumPoints(); i++) {
        std::cout << points[i] << " ";
    }
    std::cout << std::endl;

     std::cout << "=====================Square testing complete=====================" << std::endl;
     std::cout << std::endl;
}

void testTriangle() {
    std::cout << "=====================Testing Triangle=====================" << std::endl;

    Vector<3> p1 = Vector<3>{0.0, 0.0, 0.0};
    Vector<3> p2 = Vector<3>{1.0, 0.0, 0.0};
    Vector<3> p3 = Vector<3>{0.5, 1.0, 0.0};

    Triangle<3> triangle1 = Triangle<3>(p1, p2, p3);
    std::cout << "Triangle1 created using corner points:" << std::endl;
    triangle1.print();

    Triangle<3> triangle2 = Triangle<3>(triangle1);
    std::cout << "Triangle2 (copy of triangle1):" << std::endl;
    triangle2.print();

    //testing the "Triangle<n>& operator*=(const Matrix<n,n>&)" function
    Matrix<3,3> transformation;
    transformation[0][0] = 1.0; transformation[0][1] = 0.0; transformation[0][2] = 1.0; // Translation
    transformation[1][0] = 0.0; transformation[1][1] = 1.0; transformation[1][2] = 1.0; // Translation
    transformation[2][0] = 0.0; transformation[2][1] = 0.0; transformation[2][2] = 1.0; // Homogeneous coordinate

    triangle1 *= transformation;
    std::cout << "Triangle1 after *= transformation:" << std::endl;
    triangle1.print();

    //testing the triangle getPoints() function
    float* points = triangle2.getPoints();
    std::cout << "Points of triangle2:" << std::endl;
    for (int i = 0; i < triangle2.getNumPoints(); i++) {
        std::cout << points[i] << " ";
    }
    std::cout << std::endl;

    //testing the triangle grtNumPoints() function
    std::cout << "Number of points in triangle2: " << triangle2.getNumPoints() << std::endl;


     std::cout << "=====================Triangle testing complete=====================" << std::endl;
     std::cout << std::endl;
}