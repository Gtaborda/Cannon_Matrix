#pragma once
#include <cstdlib>
#include <cstdio>
#include <math.h>
#include <iostream>
#include "Exception.h"


using namespace std;

// Declarações
class Matrix;
float Det(const Matrix& a);
Matrix Diag(const int n);
Matrix Diag(const Matrix& v);
Matrix Inv(const Matrix& a);
Matrix Ones(const int rows, const int cols);
int Size(const Matrix& a, const int i);
Matrix Zeros(const int rows, const int cols);
Matrix Transposed(const Matrix& a);
Matrix Translate3X3(const Matrix& a, const float X, const float Y);
Matrix Scale3X3 (const Matrix& a, const float SX, const float SY);
Matrix Rotate3X3(const Matrix& a, const float degrees);
Matrix Transform(const Matrix&, const float X, const float Y);

class Matrix
{
public:
	// constructor
	Matrix()
	{
		// cria um objeto Matriz sem conteúdo
		p = NULL;
		rows = 0;
		cols = 0;
	}

	// constructor (por default criar uma matriz identidade)
	Matrix(const int row_count, const int column_count)
	{
		// cria um objeto Matriz com um número dado de linhas e colunas
		p = NULL;

		if (row_count > 0 && column_count > 0)
		{
			rows = row_count;
			cols = column_count;

			p = new float*[rows];
			for (int r = 0; r < rows; r++)
			{
				p[r] = new float[cols];

				// inicialmente preenche com zeros todos os campos da matriz
				for (int c = 0; c < cols; c++)
				{
					if (r == c)
						p[r][c] = 1;
					else
					    p[r][c] = 0;
				}
			}
		}
	}

	// operador de atribuição
	Matrix(const Matrix& a)
	{
		rows = a.rows;
		cols = a.cols;
		p = new float*[a.rows];
		for (int r = 0; r < a.rows; r++)
		{
			p[r] = new float[a.cols];

			// copia os valores da matriz a
			for (int c = 0; c < a.cols; c++)
			{
				p[r][c] = a.p[r][c];
			}
		}
	}

	// operador de índice. você pode usar a classe como minhaMatriz(linha, coluna)
	// os índices são baseados em 1 e não em zero
	float& operator()(const int r, const int c)
	{
		if (p != NULL && r > 0 && r <= rows && c > 0 && c <= cols)
		{
			return p[r - 1][c - 1];
		}
		else
		{
			throw Exception("Indice fora do intervalo");
		}
	}

	// operador de indice myMatrix.get(row, col)
	// os indices são baseados (iniciados) com 1 e não com zero
	// use essa função se quiser ler de uma matriz const
	float get(const int r, const int c) const
	{
		if (p != NULL && r > 0 && r <= rows && c > 0 && c <= cols)
		{
			return p[r - 1][c - 1];
		}
		else
		{
			throw Exception("Indice fora do intervalo");
		}
	}

	// operador de atribuição
	Matrix& operator= (const Matrix& a)
	{
		rows = a.rows;
		cols = a.cols;
		p = new float*[a.rows];
		for (int r = 0; r < a.rows; r++)
		{
			p[r] = new float[a.cols];

			// copy the values from the matrix a
			for (int c = 0; c < a.cols; c++)
			{
				p[r][c] = a.p[r][c];
			}
		}
		return *this;
	}

	// adiciona um valor float
	Matrix& Add(const float v)
	{
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				p[r][c] += v;
			}
		}
		return *this;
	}

	// subtrai um valor float
	Matrix& Subtract(const float v)
	{
		return Add(-v);
	}

	// multiplica um valor float
	Matrix& Multiply(const float v)
	{
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				p[r][c] *= v;
			}
		}
		return *this;
	}

	// divide um valor float
	Matrix& Divide(const float v)
	{
		return Multiply(1 / v);
	}

	// adição de matrizes
	friend Matrix operator+(const Matrix& a, const Matrix& b)
	{
		// verifica se as dimensões batem
		if (a.rows == b.rows && a.cols == b.cols)
		{
			Matrix res(a.rows, a.cols);

			for (int r = 0; r < a.rows; r++)
			{
				for (int c = 0; c < a.cols; c++)
				{
					res.p[r][c] = a.p[r][c] + b.p[r][c];
				}
			}
			return res;
		}
		else
		{
			// erro
			throw Exception("ERRO: as dimensoes sao diferentes");
		}

		// isso nunca acontece, precaução e preciosismo
		return Matrix();
	}

	// adição de matriz com um float
	friend Matrix operator+ (const Matrix& a, const float b)
	{
		Matrix res = a;
		res.Add(b);
		return res;
	}
	// adição de float com uma matriz
	friend Matrix operator+ (const float b, const Matrix& a)
	{
		Matrix res = a;
		res.Add(b);
		return res;
	}

	// subtração de matrizes
	friend Matrix operator- (const Matrix& a, const Matrix& b)
	{
		// verifica se as dimensões batem
		if (a.rows == b.rows && a.cols == b.cols)
		{
			Matrix res(a.rows, a.cols);

			for (int r = 0; r < a.rows; r++)
			{
				for (int c = 0; c < a.cols; c++)
				{
					res.p[r][c] = a.p[r][c] - b.p[r][c];
				}
			}
			return res;
		}
		else
		{
			// erro
			throw Exception("ERRO: as dimensoes sao diferentes");
		}

		// isso nunca acontece, precaução e preciosismo
		return Matrix();
	}

	// subtração de matriz com float
	friend Matrix operator- (const Matrix& a, const float b)
	{
		Matrix res = a;
		res.Subtract(b);
		return res;
	}
	// subtração de float com matriz
	friend Matrix operator- (const float b, const Matrix& a)
	{
		Matrix res = -a;
		res.Add(b);
		return res;
	}

	// operador uninário menos
	friend Matrix operator- (const Matrix& a)
	{
		Matrix res(a.rows, a.cols);

		for (int r = 0; r < a.rows; r++)
		{
			for (int c = 0; c < a.cols; c++)
			{
				res.p[r][c] = -a.p[r][c];
			}
		}

		return res;
	}

	// operador multiplicação
	friend Matrix operator* (const Matrix& a, const Matrix& b)
	{
		// verifica se as dimensões batem
		if (a.cols == b.rows)
		{
			Matrix res(a.rows, b.cols);

			for (int r = 0; r < a.rows; r++)
			{
				for (int c_res = 0; c_res < b.cols; c_res++)
				{
					res.p[r][c_res] = 0;
					for (int c = 0; c < a.cols; c++)
					{
						res.p[r][c_res] += a.p[r][c] * b.p[c][c_res];
					}
				}
			}
			return res;
		}
		else
		{
			// erro
			throw Exception("ERRO: as dimensoes sao diferentes");
		}

		// isso nunca acontece, precaução e preciosismo
		return Matrix();
	}

	// multiplicação de matriz com um float
	friend Matrix operator* (const Matrix& a, const float b)
	{
		Matrix res = a;
		res.Multiply(b);
		return res;
	}
	// multiplicação de um float com uma matriz
	friend Matrix operator* (const float b, const Matrix& a)
	{
		Matrix res = a;
		res.Multiply(b);
		return res;
	}

	// divisão de uma matriz com outra matriz
	friend Matrix operator/ (const Matrix& a, const Matrix& b)
	{
		// verifica se as dimensões batem
		if (a.rows == a.cols && a.rows == a.cols && b.rows == b.cols)
		{
			Matrix res(a.rows, a.cols);

			res = a * Inv(b);

			return res;
		}
		else
		{
			// erro
			throw Exception("ERRO: as dimensoes sao diferentes");
		}

		// isso nunca acontece, precaução e preciosismo
		return Matrix();
	}

	// divisão de uma matriz com float
	friend Matrix operator/ (const Matrix& a, const float b)
	{
		Matrix res = a;
		res.Divide(b);
		return res;
	}

	// divisão de um float com uma matriz
	friend Matrix operator/ (const float b, const Matrix& a)
	{
		Matrix b_matrix(1, 1);
		b_matrix(1, 1) = b;

		Matrix res = b_matrix / a;
		return res;
	}

	/**
	* retorna o Menor de uma Matriz (cálculo de co-fatores retirando uma linha e uma coluna)
	* determinantes de uma matriz quadrada
	*/
	Matrix Minor(const int row, const int col) const
	{
		Matrix res;
		if (row > 0 && row <= rows && col > 0 && col <= cols)
		{
			res = Matrix(rows - 1, cols - 1);

			// copia o conteúdo da matriz para o menor, exceto o que foi selecionado
			for (int r = 1; r <= (rows - (row >= rows)); r++)
			{
				for (int c = 1; c <= (cols - (col >= cols)); c++)
				{
					res(r - (r > row), c - (c > col)) = p[r - 1][c - 1];
				}
			}
		}
		else
		{
			throw Exception("ERRO: indice do menor fora do intervalo");
		}

		return res;
	}

	/*
	* retorna a dimensão da matriz
	* para i=1 retorna o número de linhas
	* para i-2 retorna o número de colunas
	* senão retorna 0
	*/
	int Size(const int i) const
	{
		if (i == 1)
		{
			return rows;
		}
		else if (i == 2)
		{
			return cols;
		}
		return 0;
	}

	// retorna o número de linhas
	int GetRows() const
	{
		return rows;
	}

	// retorna o número de colunas
	int GetCols() const
	{
		return cols;
	}

	// imprime o contéudo da matriz
	void Print() const
	{
		if (p != NULL)
		{
			cout << "[";
			for (int r = 0; r < rows; r++)
			{
				if (r > 0)
				{
					cout << " ";
				}
				for (int c = 0; c < cols - 1; c++)
				{
					cout << p[r][c] << " ";
				}
				if (r < rows - 1)
				{
					cout << p[r][cols - 1] << endl;
				}
				else
				{
					cout << p[r][cols - 1] << "]" << endl;
				}
			}
		}
		else
		{
			// matriz vazia
			cout << "[ ]" << endl;
		}
	}

	// método a
	float a() const
	{
		return get(1,1);
	}

	// método b
	float b() const
	{
		return get(1, 2);
	}

	// método c
	float c() const
	{
		return get(1, 3);
	}

	// método d
	float d() const
	{
		return get(2, 1);
	}

	// método e
	float e() const
	{
		return get(2, 2);
	}

	// método f
	float f() const
	{
		return get(2, 3);
	}

	// método g
	float g() const
	{
		return get(3, 1);
	}

	// método h
	float h() const
	{
		return get(3, 2);
	}

	// método k
	float k() const
	{
		return get(3, 3);
	}

public:
	// destructor
	~Matrix()
	{
		// limpar memória alocada
		for (int r = 0; r < rows; r++)
		{
			delete p[r];
		}
		delete p;
		p = NULL;
	}

private:
	int rows;
	int cols;
	float** p;     // ponteiro para uma matriz com floats
};