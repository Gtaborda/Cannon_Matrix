#include "Matrix.h"
#include "Exception.h"

// para i=1 retorna o número de linhas
// para i=2 retorna o número de colunas
// senão retorna 0
int Size(const Matrix& a, const int i)
{
	return a.Size(i);
}

// retorna a matriz com tamanho colunas x linhas com valores 1
Matrix Ones(const int rows, const int cols)
{
	Matrix res = Matrix(rows, cols);

	for (int r = 1; r <= rows; r++)
	{
		for (int c = 1; c <= cols; c++)
		{
			res(r, c) = 1;
		}
	}
	return res;
}

// retorna a matriz com tamanho colunas x linhas com valores 0
Matrix Zeros(const int rows, const int cols)
{
	return Matrix(rows, cols);
}

// retorna a diagonal principal da matriz com valores 1´s
// @param  v vector
// @return uma matriz diagonal com 1´s na diagonal (matriz identidade)
Matrix Diag(const int n)
{
	Matrix res = Matrix(n, n);
	for (int i = 1; i <= n; i++)
	{
		res(i, i) = 1;
	}
	return res;
}

// retorna uma matriz diagonal
// @param  v vector
// @return uma matriz diagonal com o dado vetor v na diagonal
Matrix Diag(const Matrix& v)
{
	Matrix res;
	if (v.GetCols() == 1)
	{
		// a matriz é um vetor vector n x 1
		int rows = v.GetRows();
		res = Matrix(rows, rows);

		// copia os valores do vetor para a matriz
		for (int r = 1; r <= rows; r++)
		{
			res(r, r) = v.get(r, 1);
		}
	}
	else if (v.GetRows() == 1)
	{
		// a matriz é um vetor de 1 x n
		int cols = v.GetCols();
		res = Matrix(cols, cols);

		// copia os valores do vetor para a matriz
		for (int c = 1; c <= cols; c++)
		{
			res(c, c) = v.get(1, c);
		}
	}
	else
	{
		throw Exception("ERRO: parametro para diagonal precisa ser um vetor");
	}
	return res;
}

// retorna o determinante de uma matriz a
float Det(const Matrix& a)
{
	float d = 0;    // valor do determinante
	int rows = a.GetRows();
	int cols = a.GetRows();

	if (rows == cols)
	{
		// matriz quadrada
		if (rows == 1)
		{
			// matriz 1 x 1
			d = a.get(1, 1);
		}
		else if (rows == 2)
		{
			// matriz 2 x 2
			// o determinante de [a11,a12;a21,a22]  é = a11*a22-a21*a12
			d = a.get(1, 1) * a.get(2, 2) - a.get(2, 1) * a.get(1, 2);
		}
		else
		{
			// matriz 3 x 3 ou maior
			for (int c = 1; c <= cols; c++)
			{
				Matrix M = a.Minor(1, c);
				//d += pow(-1, 1+c) * a(1, c) * Det(M);
				d += (c % 2 + c % 2 - 1) * a.get(1, c) * Det(M); // mais rápido do que com a função pow()
			}
		}
	}
	else
	{
		throw Exception("ERRO: matriz precisa ser quadrada");
	}
	return d;
}

// troca dois valores
void Swap(float& a, float& b)
{
	float temp = a;
	a = b;
	b = temp;
}

// retorna a inversa de uma matriz a
Matrix Inv(const Matrix& a)
{
	Matrix res;
	float d = 0;    // valor do determinante
	int rows = a.GetRows();
	int cols = a.GetRows();

	d = Det(a);
	if (rows == cols && d != 0)
	{
		// matriz quadrada
		if (rows == 1)
		{
			// matriz 1 x 1
			res = Matrix(rows, cols);
			res(1, 1) = 1 / a.get(1, 1);
		}
		else if (rows == 2)
		{
			// matriz 2 x 2
			res = Matrix(rows, cols);
			res(1, 1) = a.get(2, 2);
			res(1, 2) = -a.get(1, 2);
			res(2, 1) = -a.get(2, 1);
			res(2, 2) = a.get(1, 1);
			res = (1 / d) * res;
		}
		else
		{
			// matriz 3 x 3 ou maior
			// calcula a inversa usando o método de eliminação gauss-jordan
			// http://mathworld.wolfram.com/MatrixInverse.html
			// http://math.uww.edu/~mcfarlat/inverse.htm
			res = Diag(rows);   // a matriz diagonal com 1´s na diagonal (identidade)
			Matrix ai = a;      // copia a matriz a

			for (int c = 1; c <= cols; c++)
			{
				// elemento (c, c) não pode ser zero, se for troca o conteudo com as linhas inferiores
				int r;
				for (r = c; r <= rows && ai(r, c) == 0; r++)
				{
				}
				if (r != c)
				{
					// troca linhas
					for (int s = 1; s <= cols; s++)
					{
						Swap(ai(c, s), ai(r, s));
						Swap(res(c, s), res(r, s));
					}
				}

				// elimina valores não zero nas outras linhas da coluna c
				for (int r = 1; r <= rows; r++)
				{
					if (r != c)
					{
						// elimina valor na coluna c e linha r
						if (ai(r, c) != 0)
						{
							float f = -ai(r, c) / ai(c, c);

							// adiciona (f * coluna c) para a linha r para eliminar o valor na coluna c
							for (int s = 1; s <= cols; s++)
							{
								ai(r, s) += f * ai(c, s);
								res(r, s) += f * res(c, s);
							}
						}
					}
					else
					{
						// faz valor em (c, c) ser 1
						// divide cada valor na linha r com o valor de ai(c,c)
						float f = ai(c, c);
						for (int s = 1; s <= cols; s++)
						{
							ai(r, s) /= f;
							res(r, s) /= f;
						}
					}
				}
			}
		}
	}
	else
	{
		if (rows == cols)
		{
			throw Exception("ERRO: matriz precisa ser quadrada");
		}
		else
		{
			throw Exception("ERRO: determinante da matriz e zero");
		}
	}
	return res;
}

// retorna a transposta de uma matriz a
Matrix Transposed(const Matrix& a)
{
	Matrix res;
	int rows = a.GetRows();
	int cols = a.GetCols();

	res = Matrix(cols, rows);

	for (int r = 1; r <= rows; r++)
		for (int c = 1; c <= cols; c++)
			res(c,r) = a.get(r,c);

	return res;
}

// retorna a matriz de translação 3x3
Matrix Translate3X3(const Matrix& a, const float X, const float Y)
{
	Matrix res;
	res = Matrix(3, 3);

	Matrix T = a;
	T(1, 3) = X;
	T(2, 3) = Y;

	return T * res;
}

// retorna a matriz de escala 3x3
Matrix Scale3X3(const Matrix& a, const float SX, const float SY)
{
	Matrix res;
	res = Matrix(3, 3);

	Matrix T = a;
	T(1, 1) = SX;
	T(2, 2) = SY;

	return T * res;
}

Matrix Rotate3X3(const Matrix& a, const float degrees)
{
	Matrix res;
	res = Matrix(3, 3);

	Matrix T = a;
	T(1, 1) = cos(degrees);
	T(1, 2) = -sin(degrees);
	T(2, 1) = sin(degrees);
	T(2, 2) = cos(degrees);

	return T * res;
}

Matrix Transform(const Matrix& a, const float X, const float Y)
{
	Matrix res;
	res = Matrix(3, 1);

	res(1, 1) = X;
	res(2, 1) = Y;
	res(3, 1) = 1;

	Matrix T = a;
	return T * res;
}
