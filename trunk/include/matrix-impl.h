/*****************************************************************************
 *                               matrix-impl.h
 *
 * Implementation for Matrix class.
 *
 * Zhang Ming, 2010-01, Xi'an Jiaotong University.
 *****************************************************************************/


/**
 * initialize
 */
template <typename Type>
void Matrix<Type>::init( int rows, int columns )
{
	nRow = rows;
	nColumn = columns;
	nTotal = rows * columns;

	pv0 = new Type[nTotal];
	prow0 = new Type*[rows];
	prow1 = new Type*[rows];

	assert( pv0 != NULL );
	assert( prow0 != NULL );
	assert( prow1 != NULL );

	Type *p = pv0;
	pv1 = pv0 - 1;
	for( int i=0; i<nRow; ++i )
	{
		prow0[i] = p;
		prow1[i] = p-1;
		p += nColumn;
	}

	prow1--;
}


/**
 * copy matrix from normal array
 */
template <typename Type>
inline void Matrix<Type>::copyFromArray( const Type *v )
{
	for( long i=0; i<nTotal; ++i )
		pv0[i] = v[i];
}


/**
 * set matrix by a scalar
 */
template <typename Type>
inline void Matrix<Type>::setByScalar( const Type &x )
{
	for( long i=0; i<nTotal; ++i )
		pv0[i] = x;
}


/**
 * destroy the matrix
 */
template <typename Type>
void Matrix<Type>::destroy()
{
	if( pv0 == NULL )
		return ;
	else
		delete []pv0;

	if( prow0 != NULL )
		delete []prow0;

	prow1++;
	if( prow1 != NULL )
		delete []prow1;
}


/**
 * constructors and destructor
 */
template <typename Type>
Matrix<Type>::Matrix()
: pv0(0), pv1(0), prow0(0), prow1(0), nRow(0), nColumn(0), nTotal(0)
{
}

template <typename Type>
Matrix<Type>::Matrix( const Matrix<Type> &A )
{
	init( A.nRow, A.nColumn );
	copyFromArray( A.pv0 );
}

template <typename Type>
Matrix<Type>::Matrix( int rows, int columns, const Type &x )
{
	init( rows,columns );
	setByScalar(x);
}

template <typename Type>
Matrix<Type>::Matrix( int rows, int columns, const Type *arrays )
{
	init( rows,columns );
	copyFromArray( arrays );
}

template <typename Type>
Matrix<Type>::~Matrix()
{
	destroy();
}


/**
 * overload evaluate operator = from matrix to matrix
 */
template <typename Type>
Matrix<Type>& Matrix<Type>::operator=( const Matrix<Type> &A )
{
	if( pv0 == A.pv0 )
		return *this;

	if( nRow == A.nRow && nColumn == A.nColumn )
		copyFromArray( A.pv0 );
	else
	{
		destroy();
		init( A.nRow, A.nColumn );
		copyFromArray( A.pv0 );
	}

	return *this;
}


/**
 * overload evaluate operator = from scalar to matrix
 */
template <typename Type>
inline Matrix<Type>& Matrix<Type>::operator=( const Type &x )
{
	setByScalar( x );

	return *this;
}


/**
 * overload operator [] for 0-offset access
 */
template <typename Type>
inline Type* Matrix<Type>::operator[]( int i )
{
#ifdef BOUNDS_CHECK
	assert( 0 <= i );
	assert( i < nRow );
#endif

	return prow0[i];
}

template <typename Type>
inline const Type* Matrix<Type>::operator[]( int i ) const
{
#ifdef BOUNDS_CHECK
	assert( 0 <= i );
	assert( i < nRow );
#endif

	return prow0[i];
}


/**
 * overload operator () for 1-offset access
 */
template <typename Type>
inline Type& Matrix<Type>::operator()( int row, int column )
{
#ifdef BOUNDS_CHECK
	assert( 1 <= row );
	assert( row <= nRow ) ;
	assert( 1 <= column);
	assert( column <= nColumn );
#endif

	return  prow1[row][column];
}

template <typename Type>
inline const Type& Matrix<Type>::operator()( int row, int column ) const
{
#ifdef BOUNDS_CHECK
	assert( 1 <= row );
	assert( row <= nRow ) ;
	assert( 1 <= column);
	assert( column <= nColumn );
#endif

	return  prow1[row][column];
}


/**
 * type conversion functions
 */
template <typename Type>
inline Matrix<Type>::operator Type**()
{
	return  prow0;
}

template <typename Type>
inline Matrix<Type>::operator const Type**() const
{
	return prow0;
}


/**
 * get the matrix's size
 */
template <typename Type>
inline long Matrix<Type>::size() const
{
	return nTotal;
}


/**
 * get the matrix's dimension
 */
template <typename Type>
int Matrix<Type>::dim( int dimension ) const
{
#ifdef BOUNDS_CHECK
	assert( dimension >= 1);
	assert( dimension <= 2);
#endif

	if( dimension == 1 )
		return nRow;
	else if( dimension == 2 )
		return nColumn;
	else
		return 0;
}

template <typename Type>
inline int Matrix<Type>::rows() const
{
    return nRow;
}

template <typename Type>
inline int Matrix<Type>::cols() const
{
    return nColumn;
}


/**
 * reallocate matrix's size
 */
template <typename Type>
Matrix<Type>& Matrix<Type>::resize( int rows, int columns )
{
	if(  rows == nRow && columns == nColumn )
		return *this;

	destroy();
	init( rows, columns );

	return *this;
}


/**
 * get the matrix's row vector
 */
template <typename Type>
Vector<Type> Matrix<Type>::getRow( int row ) const
{
#ifdef BOUNDS_CHECK
	assert( row > 0 );
	assert( row <= nRow );
#endif

	Vector<Type> tmp( nColumn );
	for( int j=1; j<=nColumn; ++j )
		tmp(j) = prow1[row][j];

	return tmp;
}


/**
 * get the matrix's column vector
 */
template <typename Type>
Vector<Type> Matrix<Type>::getColumn( int column ) const
{
#ifdef BOUNDS_CHECK
	assert( column > 0 );
	assert( column <= nColumn );
#endif

	Vector<Type> tmp( nRow );
	for( int i=1; i<=nRow; ++i )
		tmp(i) = prow1[i][column];

	return tmp;
}


/**
 * set the matrix's row vector
 */
template <typename Type>
void Matrix<Type>::setRow( const Vector<Type> &v, int row )
{
#ifdef BOUNDS_CHECK
	assert( row > 0 );
	assert( row <= nRow );
	assert( v.dim() == nColumn );
#endif

	for( int j=1; j<=nColumn; ++j )
		prow1[row][j] = v(j);
}


/**
 * set the matrix's column vector
 */
template <typename Type>
void Matrix<Type>::setColumn(const Vector<Type> &v, int column )
{
#ifdef BOUNDS_CHECK
	assert( column > 0 );
	assert( column <= nColumn );
	assert( v.dim() == nRow );
#endif

	for( int i=1; i<=nRow; ++i )
		prow1[i][column] = v(i);
}


/**
 * get negative matrix
 */
template<typename Type>
Matrix<Type> operator-( const Matrix<Type> &A )
{
	int rows = A.rows();
	int columns = A.cols();

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			tmp[i][j] = -A[i][j];

	return tmp;
}


/**
 * matrix-scalar addition
 */
template<typename Type>
Matrix<Type> operator+( const Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int columns = A.cols();

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			tmp[i][j] = A[i][j] + x;

	return tmp;
}

template<typename Type>
inline Matrix<Type> operator+( Type x, const Matrix<Type> &A )
{
	return ( A + x );
}

template<typename Type>
Matrix<Type> operator+=( Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int columns = A.cols();

	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			A[i][j] += x;

	return A;
}


/**
 * matrix-scalar subtraction
 */
template<typename Type>
Matrix<Type> operator-( const Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int columns = A.cols();

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			tmp[i][j] = A[i][j] - x;

	return tmp;
}

template<typename Type>
Matrix<Type> operator-( Type x, const Matrix<Type> &A )
{
	int rows = A.rows();
	int columns = A.cols();

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			tmp[i][j] =  x - A[i][j];

	return tmp;
}

template<typename Type>
Matrix<Type> operator-=( Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int columns = A.cols();

	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			A[i][j] -= x;

	return A;
}


/**
 * matrix-scaling multiplication
 */
template <typename Type>
Matrix<Type> operator*( const Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int clumns = A.cols();

	Matrix<Type> tmp( rows,clumns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			tmp[i][j] = A[i][j] * x;

	return tmp;
}

template <typename Type>
inline Matrix<Type> operator*( Type x, const Matrix<Type> &A )
{
	return ( A * x );
}

template <typename Type>
Matrix<Type> operator*=( Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int clumns = A.cols();

	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			A[i][j] *= x;

	return A;
}


/**
 * matrix-scalar division
 */
template <typename Type>
Matrix<Type> operator/( const Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int clumns = A.cols();

	Matrix<Type> tmp( rows,clumns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			tmp[i][j] = A[i][j] / x;

	return tmp;
}

template <typename Type>
Matrix<Type> operator/( Type x, const Matrix<Type> &A )
{
	int rows = A.rows();
	int clumns = A.cols();

	Matrix<Type> tmp( rows,clumns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			tmp[i][j] = x / A[i][j];

	return tmp;
}

template <typename Type>
Matrix<Type> operator/=( Matrix<Type> &A, Type x )
{
	int rows = A.rows();
	int clumns = A.cols();

	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			A[i][j] /= x;

	return A;
}


/**
 * matrix-matrix addition
 */
template<typename Type>
Matrix<Type> operator+( const Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int columns = A1.cols();

	assert( rows == A2.rows() );
	assert( columns == A2.cols() );

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			tmp[i][j] = A1[i][j] + A2[i][j];

	return tmp;
}

template<typename Type>
Matrix<Type> operator+=( Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int columns = A1.cols();

	assert( rows == A2.rows() );
	assert( columns == A2.cols() );

	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			A1[i][j] += A2[i][j];

	return A1;
}


/**
 * matrix-matrix subtraction
 */
template<typename Type>
Matrix<Type> operator-( const Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int columns = A1.cols();

	assert( rows == A2.rows() );
	assert( columns == A2.cols() );

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			tmp[i][j] = A1[i][j] - A2[i][j];

	return tmp;
}

template<typename Type>
Matrix<Type> operator-=( Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int columns = A1.cols();

	assert( rows == A2.rows() );
	assert( columns == A2.cols() );

	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			A1[i][j] -= A2[i][j];

	return A1;
}


/**
 * matrix-matrix elementwise multiplication
 */
template <typename Type>
Matrix<Type> operator*( const Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int clumns = A1.cols();

	assert( rows == A2.rows() );
	assert( clumns == A2.cols() );

	Matrix<Type> tmp( rows, clumns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			tmp[i][j] = A1[i][j] * A2[i][j];

	return tmp;
}

template <typename Type>
Matrix<Type> operator*=( Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int clumns = A1.cols();

	assert( rows == A2.rows() );
	assert( clumns == A2.cols() );

	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			A1[i][j] *= A2[i][j];

	return A1;
}


/**
 * matrix-matrix elementwise division
 */
template <typename Type>
Matrix<Type> operator/( const Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int clumns = A1.cols();

	assert( rows == A2.rows() );
	assert( clumns == A2.cols() );

	Matrix<Type> tmp( rows, clumns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			tmp[i][j] = A1[i][j] / A2[i][j];

	return tmp;
}

template <typename Type>
Matrix<Type> operator/=( Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	int rows = A1.rows();
	int clumns = A1.cols();

	assert( rows == A2.rows() );
	assert( clumns == A2.cols() );

	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			A1[i][j] /= A2[i][j];

	return A1;
}


/**
 * This is an optimized version of matrix multiplication,
 * where the destination matrix has already been allocated.
 */
template <class Type>
Matrix<Type>& prod( const Matrix<Type> &A, const Matrix<Type> &B,
                    Matrix<Type> &C )
{
    int M = A.rows();
    int N = B.cols();
    int K = A.cols();

    assert( B.rows() == K );

    C.resize( M, N );
    Type        sum;
    const Type  *pRow,
                *pCol;

    for( int i=0; i<M; i++ )
        for( int j=0; j<N; ++j )
        {
            pRow  = &A[i][0];
            pCol  = &B[0][j];
            sum = 0;

            for( int k=0; k<K; ++k )
            {
                sum += (*pRow) * (*pCol);
                pRow++;
                pCol += N;
            }
            C[i][j] = sum;
        }
    return C;
}


/**
 * matrix-matrix multiplication
 */
template <typename Type>
Matrix<Type> prod(const Matrix<Type> &A1, const Matrix<Type> &A2)
{
	assert( A1.cols() == A2.rows() );

	int rows = A1.rows();
	int columns = A2.cols();
//	int K = A1.cols();

	Matrix<Type> tmp( rows, columns );
//	for( int i=0; i<rows; ++i )
//		for( int j=0; j<columns; ++j )
//		{
//            tmp[i][j] = 0;
//			for( int k=0; k<K; ++k )
//			    tmp[i][j] += A1[i][k] * A2[k][j];
//		}

    prod( A1, A2, tmp );

	return tmp;
}


/**
 * This is an optimized version of matrix and vector multiplication,
 * where the destination vector has already been allocated.
 */
template <class Type>
Vector<Type>& prod( const Matrix<Type> &A, const Vector<Type> &b,
                    Vector<Type> &c )
{
    int M = A.rows();
    int N = A.cols();

    assert( b.size() == N );

    c.resize( M );
    Type        sum;
    const Type  *pRow,
                *pCol;

    for( int i=0; i<M; i++ )
    {
        pRow  = &A[i][0];
        pCol  = &b[0];
        sum = 0;

        for( int j=0; j<N; ++j )
        {
            sum += (*pRow) * (*pCol);
            pRow++;
            pCol++;
        }
        c[i] = sum;
    }
    return c;
}


/**
 * matrix-vector multiplication
 */
template <typename Type>
Vector<Type> prod( const Matrix<Type> &A, const Vector<Type> &b )
{
	assert( A.cols() == b.dim() );

	int rows = A.rows();
//	int columns = A.cols();

	Vector<Type> tmp(rows);
//	for( int i=0; i<rows; ++i )
//	{
//		Type sum = 0;
//		for( int j=0; j<columns; ++j )
//			sum += A[i][j] * v[j];
//		tmp[i] = sum;
//	}

    prod( A, b, tmp );

	return tmp;
}


/**
 * matrix-matrix tranpose multiplication: tranpose(A)*B.
 */
template <typename Type>
Matrix<Type> tranProd( const Matrix<Type> &A1, const Matrix<Type> &A2 )
{
	assert( A1.rows() == A2.rows() );

	int rows = A1.cols();
	int columns = A2.cols();
	int K = A1.rows();

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
		{
			Type sum = 0;
			for( int k=0; k<K; k++ )
			   sum += A1[k][i] * A2[k][j];
			tmp[i][j] = sum;
		}

	return tmp;
}


/**
 * matrix-vector tranpose multiplication: tranpose(A)*b.
 */
template <typename Type>
Vector<Type> tranProd( const Matrix<Type> &A, const Vector<Type> &v )
{
	assert( A.rows() == v.dim() );

	int rows = A.rows();
	int columns = A.cols();

	Vector<Type> tmp( columns );
	for( int i=0; i<columns; ++i )
	{
		Type sum = 0;
		for( int j=0; j<rows; ++j )
			sum += A[j][i] * v[j];
		tmp[i] = sum;
	}

	return tmp;
}


/**
 * vector-vector tranpose multiplication: tranpose(a)*b.
 */
template <typename Type>
Matrix<Type> tranProd( const Vector<Type> &a, const Vector<Type> &b )
{
	int rows = a.dim();
	int columns = b.dim();

	Matrix<Type> tmp( rows, columns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			tmp[i][j] = a[i]*b[j];

	return tmp;
}


/**
 * matrix tranpose
 */
template <typename Type>
Matrix<Type> transpose( const Matrix<Type> &A )
{
	int rows = A.cols();
	int clumns = A.rows();

	Matrix<Type> tmp( rows, clumns );
	for( int i=0; i<rows; ++i )
		for( int j=0; j<clumns; ++j )
			tmp[i][j] = A[j][i];

	return tmp;
}


/**
 * Get the diagonal entries of matrix.
 */
template <typename Type>
Vector<Type> diag( const Matrix<Type> &A )
{
	int nColumn = A.rows();
	if( nColumn > A.cols() )
		nColumn = A.cols();

	Vector<Type> tmp( nColumn );
	for( int i=0; i<nColumn; ++i )
		tmp[i] = A[i][i];

	return tmp;
}


/**
 * Generate the identity matrix.
 */
template <typename Type>
Matrix<Type> eye( int N, Type x )
{
    Matrix<Type> tmp( N, N );
	for( int i=0; i<N; ++i )
		tmp[i][i] = x;

	return tmp;
}


/**
 * matrix inverse: The order of matrix A should be small, or
 * this subroutine is inefficiency.
 */
template <typename Type>
Matrix<Type> inverse( const Matrix<Type> &A )
{
	int rows = A.rows();
	int clumns = A.cols();

	assert( rows == clumns );

	Matrix<Type> invA(A);
	Vector<int> index( rows, 0 );
	int i, j, k;
	Type tmp = 0;

	for( k=0; k<rows; ++k )
	{
		//Findint pivot and exchange if necessary.
		index[k] = k;
		Type max = invA[k][k];
		for( i=k+1; i<rows; ++i )
		{
			tmp = abs(invA[i][k]);
			if( tmp > max )
			{
				max = tmp;
				index[k] = i;
			}
		}
		if( abs(max) < EPS )
		{
			cerr << "\n" << "A is a singular matrix." << "\n";
			return Matrix<Type>(0,0);
		}

		if( index[k] != k )
		{
			tmp = 0;
			for( j=0; j<rows; ++j )
			{
				tmp = invA[k][j];
				invA[k][j] = invA[index[k]][j];
				invA[index[k]][j] = tmp;
			}
		}

		// Calculating the kth column.
		invA[k][k] = 1/invA[k][k];
		for( i=0; i<rows; ++i )
			if( i != k )
				invA[i][k] = - invA[k][k]*invA[i][k];

		// Calculating all elements excptint the kth row and column.
		for( i=0; i<rows; ++i )
			if( i != k )
				for( j=0; j<rows; ++j )
					if( j != k )
						invA[i][j] += invA[i][k] * invA[k][j];

		// Calculating the kth row.
		for( j=0; j<rows; ++j )
			if( j != k )
				invA[k][j] *= invA[k][k];
	}

	//Exchanging back.
	for( k=0; k<rows; ++k )
		if( index[k] != k )
		{
			tmp = 0;
			for( i=0; i<rows; ++i )
			{
				tmp = invA[i][k];
				invA[i][k] = invA[i][index[k]];
				invA[i][index[k]] = tmp;
			}
		}

	return invA;
}


/**
 * Compute Frobenius norm of matrix.
 */
template <class Type>
Type norm(const Matrix<Type> &A)
{
	int m = A.rows();
	int n = A.cols();

	Type sum = 0;
	for( int i=1; i<=m; ++i )
		for( int j=1; j<=n; ++j )
            sum += A(i,j) * A(i,j);

	return sqrt(sum);
}


/**
 * Overload the output stream function.
 */
template <typename Type>
std::ostream& operator<<( std::ostream &out, const Matrix<Type> &A )
{
	int rows = A.rows();
	int columns = A.cols();

	out << "size: " << rows << " by " << columns << "\n";
	for( int i=0; i<rows; ++i )
	{
		for( int j=0; j<columns; ++j )
			out << A[i][j] << " ";
		out << "\n";
	}

	return out;
}


/**
 * Overload the intput stream function.
 */
template <typename Type>
std::istream& operator>>( std::istream &in, Matrix<Type> &A )
{
	int rows, columns;
	in >> rows >> columns;

	if( !( rows == A.rows() && columns == A.cols() ) )
		A.resize( rows, columns );

	for( int i=0; i<rows; ++i )
		for( int j=0; j<columns; ++j )
			in >> A[i][j];

	return in;
}
