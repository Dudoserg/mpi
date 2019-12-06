// testMPI.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "mpi.h"
#include <iostream>
#include <windows.h>
using namespace std;

double Fact(int n)
{
	if (n == 0)
		return 1;
	else
		return n * Fact(n - 1);
}

#define MAX 100 

void start(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	//	start(argc, argv);
	void *ptr;
	int num = 228;
	ptr = &num;
	cout << "..............." << endl;

	int *get = static_cast<int*>(ptr);
	cout << *get << endl;
	return 0;
}


void start(int argc, char *argv[]) {
	cout << argc << endl;
	for (int i = 0; i < argc; i++) {
		cout << i << "  " << argv[0] << endl;
	}
	int rank, size, n, i, ibeg, iend;
	int root = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//n = (MAX - 1) / size + 1;
	//ibeg = rank * n + 1;
	//iend = (rank + 1)*n;
	//for (i = ibeg; i <= ((iend>MAX) ? MAX : iend); i++)
	//	printf("process %d, %d^2=%d\t\tsize = %d\n", rank, i, i*i, size);
	for (int q = 0; q < 5; q++) {
		if (rank == root) {
			for (int i = 1; i < size; i++) {
				int get;
				MPI_Recv(
					&get,								//	адрес начала расположения принимаемого сообщения;
					1,									//	максимальное число принимаемых элементов;
					MPI_INT,							//	тип элементов принимаемого сообщения;
					i,									//	номер процесса-отправителя;
					0,									//	идентификатор сообщения;
					MPI_COMM_WORLD,						//	коммуникатор области связи;
					MPI_STATUS_IGNORE					//	атрибуты принятого сообщения.
				);
				cout << "rank # " << rank << "\t i get : " << get << " from  rank # " << i << endl;
			}
			cout << endl << endl;
		}
		else if (rank == 1) {
			int num = 123;
			MPI_Send(
				&num,				//	адрес начала расположения пересылаемых данных; 
				1,					//	число пересылаемых элементов;
				MPI_INT,			//	тип посылаемых элементов;
				root,				//	номер процесса-получателя в группе, связанной с коммуникатором comm;
				0,					//	идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
				MPI_COMM_WORLD		//	коммуникатор области связи.
			);
		}
		else if (rank == 2) {
			int num = 2;
			MPI_Send(
				&num,				//	адрес начала расположения пересылаемых данных; 
				1,					//	число пересылаемых элементов;
				MPI_INT,			//	тип посылаемых элементов;
				root,				//	номер процесса-получателя в группе, связанной с коммуникатором comm;
				0,					//	идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
				MPI_COMM_WORLD		//	коммуникатор области связи.
			);
		}
		else if (rank == 3) {
			int num = 3;
			MPI_Send(
				&num,				//	адрес начала расположения пересылаемых данных; 
				1,					//	число пересылаемых элементов;
				MPI_INT,			//	тип посылаемых элементов;
				root,				//	номер процесса-получателя в группе, связанной с коммуникатором comm;
				0,					//	идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
				MPI_COMM_WORLD		//	коммуникатор области связи.
			);
		}
	}

	MPI_Finalize();

	int q23123123; cin >> q23123123;
	/*SetConsoleOutputCP(1251);
	int n;
	int myid;
	int numprocs;
	int i;
	int rc;
	long double drob, drobSum = 0, Result, sum;
	double startwtime = 0.0;
	double endwtime;

	n = 2;

	if (rc = MPI_Init(&argc, &argv))
	{
	cout << "Ошибка запуска, выполнение остановлено " << endl;
	MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);


	if (myid == 0)
	{
	startwtime = MPI_Wtime();
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	for (i = myid; i <= n; i += numprocs)
	{
	drob = 1 / Fact(i);
	drobSum += drob;
	}

	MPI_Reduce(&drobSum, &Result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	cout.precision(20);
	if (myid == 0)
	{
	cout << Result << endl;
	endwtime = MPI_Wtime();
	cout << (endwtime - startwtime) * 1000 << endl;
	}

	MPI_Finalize();*/
}

