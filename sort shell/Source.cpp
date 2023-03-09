#include <thread>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Shell(int A[], int n)
{
	int d = n;
	d = d / 2;
	while (d > 0)
	{
		for (int i = 0; i < n - d; i++)
		{
			int j = i;
			while (j >= 0 && A[j] > A[j + d])
			{
				int count = A[j];
				A[j] = A[j + d];
				A[j + d] = count;
				j--;
			}
		}
		d = d / 2;
	}
	for (int i = 0; i < n; i++) cout << A[i] << " ";
}

void main()
{
	cout << " Array size > "; 
	int n;
	cin >> n;
	int* A = new int[n];
	for (int i = 0; i < n; i++)
	{
		cout << i + 1 << " element > "; cin >> A[i];
	}
	cout << "\nResult array: ";
	Shell(A, n);
	delete[] A;
	system("pause>>void");
}