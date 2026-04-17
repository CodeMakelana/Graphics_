template<int n>
Square<n>::Square(const Vector<n>& center, float height, float width) {
    float halfWidth =  width / 2.0;
    float halfHeight = height / 2.0;
    
    //Corner calcs
    this->tl = Vector<n>();  
    this->tl[0] = center[0] - halfWidth;   // x: go LEFT from center
    this->tl[1] = center[1] + halfHeight;  // y: go UP from center
    
    // Copy all remaining dimensions from center
    for (int i = 2; i < n; i++) {
        this->tl[i] = center[i];
    }

    this->tr = Vector<n>();  
    this->tr[0] = center[0] + halfWidth;   // x: go RIGHT from center
    this->tr[1] = center[1] + halfHeight;  // y: go UP from center
    
    // Copy all remaining dimensions from center
    for (int i = 2; i < n; i++) {
        this->tr[i] = center[i];
    }

    this->br = Vector<n>();  
    this->br[0] = center[0] + halfWidth;   // x: go RIGHT from center
    this->br[1] = center[1] - halfHeight;  // y: go DOWN from center
    
    // Copy all remaining dimensions from center
    for (int i = 2; i < n; i++) {
        this->br[i] = center[i];
    }

    this->bl = Vector<n>();  
    this->bl[0] = center[0] - halfWidth;   // x: go LEFT from center
    this->bl[1] = center[1] - halfHeight;  // y: go DOWN from center
    
    // Copy all remaining dimensions from center
    for (int i = 2; i < n; i++) {
        this->bl[i] = center[i];
    }
}

template<int n>
Square<n>::Square(const Vector<n>& tl, const Vector<n>& tr, const Vector<n>& br, const Vector<n>& bl) {
    this->tl = tl;
    this->tr = tr;
    this->br = br;
    this->bl = bl;
}

template<int n>
Square<n>::Square(const Square<n>& other) {
    tl = other.tl;
    tr = other.tr;
    br = other.br;
    bl = other.bl;
}

template <int n>
Square<n>& Square<n>::operator*=(const Matrix<n,n>& other) {
    Matrix<n,1> tlMat = tl;  // Vector → Matrix
    Matrix<n,1> trMat = tr;
    Matrix<n,1> brMat = br;
    Matrix<n,1> blMat = bl;
    
    // Matrix multiplication
    tlMat = other * tlMat;
    trMat = other * trMat;
    brMat = other * brMat;
    blMat = other * blMat;
    
    // Convert back to vectors
    tl = Vector<n>(tlMat);  // Matrix → Vector
    tr = Vector<n>(trMat);
    br = Vector<n>(brMat);
    bl = Vector<n>(blMat);
    
    return *this;
}

template <int n>
Square<n>* Square<n>::operator*(const Matrix<n,n>& other) const {
    Square<n>* res = new Square<n>(*this);
    (*res) *= other;
    return res;
}

template <int n>
float* Square<n>::getPoints() const {
    int pointNum = 4; // A square has 4 points
    float* points = new float[pointNum * n]; // Each point has n dimensions
    // for (int i = 0; i < n; i++) {
    //     points[i] = tl[i];
    //     points[n + i] = tr[i];
    //     points[2*n + i] = br[i];
    //     points[3*n + i] = bl[i];
    // }

    int pos = 0;
    for (int i = 0; i < n; i++) { //top left point
        points[pos++] = tl[i];
    }
    for (int i = 0; i < n; i++) { //top right point
        points[pos++] = tr[i];
    }
    for (int i = 0; i < n; i++) { //bottom right point
        points[pos++] = br[i];
    }
    for (int i = 0; i < n; i++) { //bottom left point
        points[pos++] = bl[i];
    }

    return points;
}

template <int n>
int Square<n>::getNumPoints() const {
    return 4*n; // A square has 4 points
}
