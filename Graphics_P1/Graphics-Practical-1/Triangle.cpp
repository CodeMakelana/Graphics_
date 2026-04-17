template <int n>
Triangle<n>::Triangle(const Vector<n>& p1, const Vector<n>& p2, const Vector<n>& p3) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}

template <int n>
Triangle<n>::Triangle(const Triangle<n>& other) {
    p1 = other.p1;
    p2 = other.p2;
    p3 = other.p3;
}

template <int n>
Triangle<n>& Triangle<n>::operator*=(const Matrix<n,n>& other) {
    Matrix<n,1> p1Mat = p1;  // Vector → Matrix
    Matrix<n,1> p2Mat = p2;
    Matrix<n,1> p3Mat = p3;

    p1Mat = other * p1Mat;  // Matrix multiplication
    p2Mat = other * p2Mat;
    p3Mat = other * p3Mat;

    p1 = p1Mat;  // Matrix → Vector
    p2 = p2Mat;
    p3 = p3Mat;
    return *this;
}

template <int n>
Triangle<n>* Triangle<n>::operator*(const Matrix<n,n>& other) const {
    Triangle<n>* res = new Triangle<n>(*this);
    (*res) *= other;  // Reuse the *= operator
    return res;
}

template <int n>
float* Triangle<n>::getPoints() const {
    int pointNum = 3; // A triangle has 3 points
    float* points = new float[pointNum * n]; // Each point has n dimensions
    
    int pos = 0;
    for (int i = 0; i < n; i++) {
        points[pos++] = p1[i];
    }
    for (int i = 0; i < n; i++) {
        points[pos++] = p2[i];
    }
    for (int i = 0; i < n; i++) {
        points[pos++] = p3[i];
    }

    return points;
}

template <int n>
int Triangle<n>::getNumPoints() const {
    return 3*n; // A triangle has 3 points
}