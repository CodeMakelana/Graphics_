template <int n, int m>
Matrix<n,m>::Matrix() {
    arr = new float*[n]; 

    // Combine allocation and initialization for efficiency and correctness
    for (int i = 0; i < n; i++) {
        arr[i] = new float[m];
        
        // Initialize values for this row
        for (int j = 0; j < m; j++) {
            arr[i][j] = 0.0f;
        }
    }
}

template <int n, int m>
Matrix<n,m>::Matrix(float ** mat1) {
    arr = mat1;
}

template<int n,int m>
Matrix<n,m>::Matrix(const Matrix<n,m> &mat) {
    arr = new float*[n];

    //creating the new array
    for (int i = 0; i < n; i++) {
        arr[i] = new float[m];
    }


    //Deep copying the array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = mat[i][j];
        }
    }
}

template<int n, int m>
Matrix<n,m>::~Matrix() {
    for (int i = 0; i < n; i++) {
        delete[] arr [i];
    }

    delete[] arr;
    arr = nullptr;
}

template<int n, int m>
Matrix<n,m>& Matrix<n,m>::operator=(const Matrix<n,m>& mat) {
    if (this == &mat) {
        return *this;
    }

    // Deallocate existing memory
    for (int i = 0; i < n; i++) {
        delete[] arr[i];
    }
    delete[] arr;

    // Allocate new memory
    arr = new float*[n];
    for (int i = 0; i < n; i++) {
        arr[i] = new float[m];
    }

    // Deep copy the values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = mat[i][j];
        }
    }
    return *this;
}

template<int n, int m>
template<int a>
Matrix<n,a> Matrix<n,m>::operator*(const Matrix<m,a> mat) const{
    //Create new matrix to store result in
    Matrix<n,a> res;
    //Matrix multiplication
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < a; j ++) {
            for (int k = 0; k < m; k++) {
                res[i][j] += arr[i][k] * mat[k][j];
            }
        }
    }
    return res;            
}

template<int n, int m>
Matrix<n,m> Matrix<n,m>::operator*(const float s) const {
    Matrix<n,m> res;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res[i][j] = arr[i][j] * s;
        }
    }
    return res;
}

template<int n, int m>
Matrix<n,m> Matrix<n,m>::operator+(const Matrix<n,m> mat) const {
    Matrix<n,m> res;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res[i][j] = arr[i][j] + mat[i][j];
        }
    }
    return res;
}

template<int n, int m>
Matrix<m,n> Matrix<n,m>::operator~() const {
    Matrix<m,n> res;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res[j][i] = arr[i][j];
        }
    }
    return res;
}

template<int n, int m>
int Matrix<n,m>::getM() const {
    return m;
}

template<int n, int m>
int Matrix<n,m>::getN() const {
    return n;
}

template<int n, int m>
Matrix<n-1, m-1> createSubMatrix(const Matrix<n,m>& mat, int excludeRow, int excludeCol) {
    Matrix<n-1, m-1> res;
    int r = 0;
    for (int i = 0; i < n; i++) {
        if (i == excludeRow) continue;
        int c = 0;
        for (int j = 0; j < m; j++) {
            if (j == excludeCol) continue;
            res[r][c] = mat[i][j];
            c++;
        }
        r++;
    }
    return res;
}


// Specialization for 1x1 base case
template<>
float Matrix<1,1>::determinant() const {
    return arr[0][0];
}

// General recursive case
template<int n, int m>
float Matrix<n,m>::determinant() const {
    // Check if matrix is square
    if (n != m) {
        throw "Matrix is not square";
    }
    
    // Cofactor expansion along the first row
    float det = 0.0f;
    
    for (int j = 0; j < n; j++) {
        // Create submatrix by removing row 0 and column j
        Matrix<n-1, n-1> submatrix = createSubMatrix(*this, 0, j);
        
        // Calculate cofactor sign: alternates +, -, +, -, ...
        float sign = (j % 2 == 0) ? 1.0f : -1.0f;
        
        // Add contribution: sign × element × determinant of submatrix
        det += sign * arr[0][j] * submatrix.determinant();
    }
    
    return det;
}