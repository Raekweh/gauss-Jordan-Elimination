#include <iostream>
#include <vector> 
#include <fstream>

using namespace std;

unsigned nrows = 3;
unsigned ncols = 3;

class MyMatrix
{
private:

	vector<double> elements; // array containing data
	vector<double*> rows; // array of pointers to start of each row
	unsigned ncols; // number of columns
public:

	// constructor takes matrix dimensions as arguments
	MyMatrix(unsigned nrows, unsigned ncols);

	// method returns the element in the ith row and jth column
	double get(unsigned irow, unsigned jcol);

	// method sets the element in the ith row and jth column
	void set(unsigned irow, unsigned jcol, double value);

	friend istream& operator>> (istream& in, MyMatrix&); //load dataset

	//Divide
	void divide(unsigned irow, double x);

	//Destroy
	void add_multiply(unsigned irow, double x, unsigned jrow);

	// Mulitply
	void multiply(unsigned irow, unsigned jcol, double x); //Shhh i spelt it wrong and i cbs fixing it
};

//Indentity Matrix
void setIdentity(MyMatrix identityRows) {

	for (unsigned row = 0; row < nrows; row++)
	{
		for (unsigned col = 0; col < ncols; col++) {
			if (row == col) {
				identityRows.set(row, col, 1);
			}
			else if (row != col) {
				identityRows.set(row, col, 0);
			}
		}
	}
}

int main()
{
	// construct a new instance/object of the class MyMatrix

	MyMatrix matrix(nrows, ncols);
	MyMatrix identityRows(nrows, ncols);

	setIdentity(identityRows);

	ifstream file("matrix.tsv");

	if (!file)
	{
		cerr << "could not open matrix.tsv" << endl;
		return -1;
	}

	file >> matrix;

	cout << "Original Matrix" << endl;
	for (unsigned irow = 0; irow < nrows; irow++)
	{
		for (unsigned icol = 0; icol < ncols; icol++)
		{
			cout << matrix.get(irow, icol) << "\t";
		}
		cout << endl;
	}


	//DIsplaying the identity matrix
	cout << "Identity Matrix" << endl;
	for (unsigned irow = 0; irow < nrows; irow++)
	{
		for (unsigned icol = 0; icol < ncols; icol++)
		{
			cout << identityRows.get(irow, icol) << "\t";
		}
		cout << endl;
	}

	// Changing the graph into the identity matrix
	for (unsigned icol = 0; icol < ncols; icol++)
	{
		for (unsigned irow = 0; irow < nrows; irow++)
		{
			// Just need to find an algorithm to implement everything
			// Also need to make an identity matrix to update it
			if (irow == icol)
			{
				matrix.divide(irow, matrix.get(irow, icol)); // Setting (irow,icol) to 1
				identityRows.divide(irow, matrix.get(irow, icol)); // Setting (irow,icol) to 1
			}
		}

		// Destorying the values that are not irow = icol
		for (unsigned irow = 0; irow < nrows; irow++) { // Change to rows
			if (irow != icol)
			{
				matrix.add_multiply(irow, -1 * matrix.get(irow, icol), icol); //Setting(1,0) to 0
				identityRows.add_multiply(irow, -1 * matrix.get(irow, icol), icol); //Setting(1,0) to 0
			}
		}
		cout << endl;
	}

	cout << "Conversation Matrix to identity Matrix" << endl;
	for (unsigned icol = 0; icol < ncols; icol++)
	{
		for (unsigned irow = 0; irow < nrows; irow++)
		{
			cout << matrix.get(icol, irow) << "\t";
		}
		cout << endl;
	}

	cout << "Conversation Indentity matrix to Inverse Matrix" << endl;
	for (unsigned icol = 0; icol < ncols; icol++)
	{
		for (unsigned irow = 0; irow < nrows; irow++)
		{
			cout << identityRows.get(irow, icol) << "\t";
		}
		cout << endl;
	}

	//Multiply
	for (unsigned i = 0; i < nrows; i++)
	{
		for (unsigned j = 0; j < ncols; j++)
		{
			double number = 0;
			for (unsigned k = 0; k < ncols; k++)
			{
				number += matrix.get(i, k) * identityRows.get(k, j);
				matrix.set(i, j, number);
			}
		}
		cout << endl;
	}

	cout << "Multiplication" << endl;
	for (unsigned icol = 0; icol < ncols; icol++)
	{
		for (unsigned irow = 0; irow < nrows; irow++)
		{
			cout << matrix.get(icol, irow) << "\t";
		}
		cout << endl;
	}
	return 0;
}

MyMatrix::MyMatrix(unsigned nrows, unsigned _ncols) {
	ncols = _ncols;

	elements.resize(nrows * ncols);
	rows.resize(nrows);

	// the & returns "the address of" the element in the array
	for (unsigned irow = 0; irow < nrows; irow++)
		rows[irow] = &(elements[irow * ncols]);
}

double MyMatrix::get(unsigned irow, unsigned jcol)
{
	return rows[irow][jcol];
}

void MyMatrix::set(unsigned irow, unsigned jcol, double value)
{
	rows[irow][jcol] = value; //private
	//columns[jcol][irow] = value; //private
}

istream& operator>> (istream& in, MyMatrix& matrix)
{
	unsigned ncols = matrix.ncols;
	unsigned nrows = matrix.rows.size();

	for (unsigned irow = 0; irow < nrows; irow++)
	{
		for (unsigned icol = 0; icol < ncols; icol++)
		{
			double value;
			in >> value;

			matrix.set(irow, icol, value);
		}
	}

	return in;
}

//Divide Method to set irow = icol to 1
void MyMatrix::divide(unsigned irow, double x) {

	// Interate through each row member
		//for each row number divide by x
	for (unsigned i = 0; i < ncols; i++) {
		rows[irow][i] = rows[irow][i] * 1 / x;
	}
}

//Setting rows to 0
// Make sure you rename the function
void  MyMatrix::add_multiply(unsigned irow, double x, unsigned jrow) {

	// Interate through each row member
			//for each row number multiply by x adding row(jrow)
	for (unsigned icol = 0; icol < ncols; icol++)
	{
		//Change later
		double y = rows[jrow][icol];
		y = y * x;
		rows[irow][icol] += y;
	}
}

void MyMatrix::multiply(unsigned irow, unsigned jcol, double x)
{
	rows[irow][jcol] = x;
}