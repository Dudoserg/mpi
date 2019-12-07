// testMPI.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "mpi.h"
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "MyClass.h"
#include "Info.h"


#include <fstream>
#include <string>
#include <fstream>

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


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

void sendNum(int *num, int to, int tag) {
	MPI_Send(
		&num,				//	адрес начала расположения пересылаемых данных; 
		1,					//	число пересылаемых элементов;
		MPI_INT,			//	тип посылаемых элементов;
		to,				//	номер процесса-получателя в группе, связанной с коммуникатором comm;
		tag,					//	идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
		MPI_COMM_WORLD		//	коммуникатор области связи.
	);
}

std::vector<MyClass*> vect;

int main(int argc, char *argv[])
{
	Info info;
	info.AddFilename("ThisFile.txt");
	info.AddFilename("ThatFile.txt");
	info.AddFilename("OtherFile.txt");

	info.myclasses.push_back(new MyClass(123));
	info.myclasses.push_back(new MyClass(234));
	info.myclasses.push_back(new MyClass(1));
	info.myclasses.push_back(new MyClass(-99));

	// Save filename data contained in Info object
	{
		// Create an output archive
		std::ofstream ofs("store.dat");
		boost::archive::text_oarchive ar(ofs);

		// Save the data
		ar & info;
	}

	// Restore from saved data and print to verify contents
	Info *restored_info = new Info();
	{
		// Create and input archive
		std::ifstream ifs("store.dat");
		boost::archive::text_iarchive ar(ifs);

		// Load the data
		ar & (*restored_info);
	}

	restored_info->Print();

	

	//for (int i = 0; i < 4; i++) {
	//	vect.push_back(new MyClass(0));
	//}
	//start(argc, argv);
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

	if (rank == root) {

		int size;
		for (int i = 1; i < 4; i++) {
			MPI_Recv(
				&size,								//	адрес начала расположения принимаемого сообщения;
				1,									//	максимальное число принимаемых элементов;
				MPI_INT,							//	тип элементов принимаемого сообщения;
				i,									//	номер процесса-отправителя;
				0,									//	идентификатор сообщения;
				MPI_COMM_WORLD,						//	коммуникатор области связи;
				MPI_STATUS_IGNORE					//	атрибуты принятого сообщения.
			);
			cout << "rank # " << rank << "\t i get : " << size << " from  rank # " << 1 << endl;
		}
		int num = 0;
		// считываем с островов
		for (int i = 0; i < 4; i++) {
			num += vect[i]->x;
		}
		cout << "sum = " << num << endl;
		// помещаем во все острова вычисленное
		for (int i = 0; i < 4; i++) {
			vect[i]->x = num;
		}
		cout << "Все дочерние потоки закончили итерацию" << endl;


		/*char *get = new char[1000];
		MPI_Recv(get, 1000, MPI_CHAR, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("rank 0 get string %s from rank 1\n", get);*/

	}
	else if (rank == 1) {
		/*char helloStr[] = "||Hello DUDUDUDUDUDUDUDDOOOOOS World||";
		MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, root, 1, MPI_COMM_WORLD);*/

		MyClass * tmp = vect[1];
		tmp->x =  (tmp->x) + 1;
		cout << 1 << " записал " << endl;
		int num = 1;
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
		/*char helloStr[] = "||Hello DUDUDUDUDUDUDUDDOOOOOS World||";
		MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, root, 1, MPI_COMM_WORLD);*/

		MyClass * tmp = vect[2];
		tmp->x = (tmp->x) + 1;
		cout << 2 << " записал " << endl;
		int num = 1;
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
		/*char helloStr[] = "||Hello DUDUDUDUDUDUDUDDOOOOOS World||";
		MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, root, 1, MPI_COMM_WORLD);*/

		MyClass * tmp = vect[3];
		tmp->x = (tmp->x) + 1;
		cout << 3 << " записал " << endl;
		int num = 1;
		MPI_Send(
			&num,				//	адрес начала расположения пересылаемых данных; 
			1,					//	число пересылаемых элементов;
			MPI_INT,			//	тип посылаемых элементов;
			root,				//	номер процесса-получателя в группе, связанной с коммуникатором comm;
			0,					//	идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
			MPI_COMM_WORLD		//	коммуникатор области связи.
		);
	}



	MPI_Finalize();
	if (rank == root) {
		cout << "KONEC" << endl;
		for (int i = 0; i < 4; i++) {
			cout << "vect[" << i + 1 << "] = " << vect[i]->x << endl;
		}
	}
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