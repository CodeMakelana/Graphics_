template<int n>
Vector<n>::Vector() {
    arr = new float[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = 0.0;
    }
}

template<int n>
Vector<n>::Vector(std::initializer_list<float> list) {
    arr = new float[n];
    int i = 0;
    for (float val : list) {
        if (i >= n) break; // n is the size of the vector(the array), so we should not exceed it
        arr[i++] = val;
    }
    for (; i < n; i++) {
        arr[i] = 0.0;
    }
}

template<int n>
Vector<n>::Vector(float *arr1) {
    arr = arr1;
}

template<int n>
Vector<n>::~Vector() {
    delete[] arr;
}

template<int n>
Vector<n>::Vector(const Vector<n> &v) {
    arr = new float[n];
    for (int i = 0; i < n; i++) {
        arr[i] = v[i];
    }
}

template<int n>
Vector<n>::Vector(const Matrix<n,1>& mat) {
    arr = new float[n];
    for (int i = 0; i < n; i++) {
        arr[i] = mat[i][0];
    }
}

template<int n>
Vector<n>::operator Matrix<n,1>() const {
    Matrix<n,1> mat;
    for (int i = 0; i < n; i++) {
        mat[i][0] = arr[i];
    }
    return mat;
}

template<int n>
Vector<n>& Vector<n>::operator=(const Vector<n>& v) {
    if (this == &v) {
        return *this;
    }
    delete[] arr;
    arr = new float[n];
    for (int i = 0; i < n; i++) {
        arr[i] = v[i];
    }
    return *this;
}

template<int n>
Vector<n> Vector<n>::operator+(const Vector<n> v) const {
    Vector<n> res;
    for (int i = 0; i < n; i++) {
        res[i] = arr[i] + v[i];
    }
    return res;
}

template<int n>
Vector<n> Vector<n>::operator-(const Vector<n> v) const {
    Vector<n> res;
    for (int i = 0; i < n; i++) {
        res[i] = arr[i] - v[i];
    }
    return res;
}


template<int n>
Vector<n> Vector<n>::operator*(const float x) const {
    Vector<n> res;
    for (int i = 0; i < n; i++) {
        res[i] = (arr[i]) * x;
    }

    return res;
}

//dot product of two vectors
template<int n>
float Vector<n>::operator*(const Vector<n> v) const {
    float res = 0.0;
    for (int i = 0; i < n; i++) {
        res += arr[i] * v[i];
    }
    return res;
}

template<int n> 
float Vector<n>::magnitude() const {
    float mag = 0.0;
    for (int i = 0; i < n; i++) {
        mag += arr[i]*arr[i];
    }
    return sqrt(mag);
}

template<int n>
Vector<3> Vector<n>::crossProduct(const Vector<3> v) const {
    Vector<3> CrossRes;
// arr = A and v = B
// A x B = ( a2b3 - a3b2)i - (a1b3 - a3b1)j + (a1b2 - a2b1)k

    CrossRes[0] = ((arr[1] * v[2]) - (arr[2] * v[1]));
    CrossRes[1] = ((arr[2] * v[0]) - (arr[0] * v[2]));
    CrossRes[2] = ((arr[0] * v[1]) - (arr[1] * v[0]));
    
    return CrossRes;
}

template<int n>
Vector<n> Vector<n>::unitVector() const {
    //get norm
    float norm = magnitude();
    // if (norm == 0) {
    //     return;
    // }
    float inverse = 1 / norm;
    return (*this) * inverse;
}

template<int n>
int Vector<n>::getN() const {
    return n;
}

