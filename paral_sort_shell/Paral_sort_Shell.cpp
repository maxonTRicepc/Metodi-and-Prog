#include <iostream>
#include <vector>
#include <thread>

using namespace std;

vector <int> list_1;
vector <int> list_2;

// сортировка Шелла
void sort_shella(vector <int>& A, int size)
{
	for (auto d = size; d > 0; d /= 2) {
		for (auto i = d; i < size; i++) {
			for (auto j = i; j >= d && A[j] < A[j - d]; j -= d) {
				int tmp = A[j];
				A[j] = A[j - d];
				A[j - d] = tmp;
			}
		}
	}
}

void sort_shella_paral(vector <int>& A, int step, int d, int i) {
	for (i; i < (d - step); i++) {
        for (int j = i; j >= 0 && A[j] > A[j + step]; j--) {
			swap(A[j], A[j + step]);
        }
    }
}

// реализация многопоточности
void parallelSort(int d, int step, int max) {
	vector<thread> v;
		for (int i = 0; i < max; i++) {
			if (i > d) break;
			v.push_back(thread(sort_shella_paral, ref(list_2), ref(step), ref(d), ref(i)));
		}
		for (int i = 0; i < max; i++) {
			v[i].join();
		}
		if (step / 2 > 1) {
			parallelSort(d, step / 2, max / 2);
		}
		else {
			sort_shella_paral(list_2, step, d, 0);
		}
}

// подсчет времени при обычной и параллельной сортировке
void time_manag() {
	double stime, ftime;
	stime = clock();
	sort_shella(list_1, list_1.size());
	ftime = clock();
	cout << "Time default sort : " << (ftime - stime) / CLOCKS_PER_SEC << endl;

	stime = clock();
	parallelSort(list_2.size(), list_2.size() / 2, 16);
	ftime = clock();
	cout << "Time paralel sort : " << (ftime - stime) / CLOCKS_PER_SEC << endl << endl << endl;
}

void generation(int dl) {
	list_1.clear();
	list_2.clear();
	for (int i = 0; i < dl; i++) {
		int el = rand() % 2000 - 1000;
		list_1.push_back(el);
		list_2.push_back(el);
	}
}


void test(int n) {
	cout << n << endl;
	generation(n);
	time_manag();
}

int main() {
	test(100000);
	test(1000000);
	test(10000000);
}