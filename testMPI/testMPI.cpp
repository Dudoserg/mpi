// testMPI.cpp: определяет точку входа для консольного приложения.
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "mpi.h"
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "MyClass.h"
#include "Info.h"
#include "Tag.h"


#include <fstream>
#include <string>
#include <fstream>
#include <sstream>


#include <cstring>

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
	//Info info;
	//info.AddFilename("ThisFile.txt");
	//info.AddFilename("ThatFile.txt");
	//info.AddFilename("OtherFile.txt");

	//info.myclasses.push_back(new MyClass(123));
	//info.myclasses.push_back(new MyClass(234));
	//info.myclasses.push_back(new MyClass(1));
	//info.myclasses.push_back(new MyClass(-99));

	//// Save filename data contained in Info object
	//{
	//	// Create an output archive
	//	std::ofstream ofs("store.dat");
	//	boost::archive::text_oarchive ar(ofs);

	//	// Save the data
	//	ar & info;
	//}

	//// Restore from saved data and print to verify contents
	//Info *restored_info = new Info();
	//{
	//	// Create and input archive
	//	std::ifstream ifs("store.dat");
	//	boost::archive::text_iarchive ar(ifs);

	//	// Load the data
	//	ar & (*restored_info);
	//}

	//restored_info->Print();

	
	start(argc, argv);
	system("pause");

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
		// Получаем сообщение в котором лежит размер передаваемой строки (в которой сериализован передаваемый объект)
		//		от конкретного потока
		MPI_Recv(
			&size,								//	адрес начала расположения принимаемого сообщения;
			1,									//	максимальное число принимаемых элементов;
			MPI_INT,							//	тип элементов принимаемого сообщения;
			1,									//	номер процесса-отправителя;
			(int)Tag::ENUM::SIZE,				//	идентификатор сообщения;
			MPI_COMM_WORLD,						//	коммуникатор области связи;
			MPI_STATUS_IGNORE					//	атрибуты принятого сообщения.
		);
		cout << "rank # " << rank << "\t size : " << size << " from  rank # " << 1 << endl;
		
		// Создаем указатель на строку, с размером полученным в предыдущем сообщении
		char *get = new char[size];
		// Получаем строку (в которой сериализован передаваемый объект) от конкретного потока, размер уже знаем
		MPI_Recv(get, size, MPI_CHAR, 1, (int)Tag::ENUM::MESSAGE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("rank 0 get string %s from rank 1\n", get);


		// Restore from saved data and print to verify contents
		Info *restored_info = new Info();
		{
			// Create and input archive
			std::istringstream iss(get);
			boost::archive::text_iarchive ar(iss);

			// Load the data
			ar & (*restored_info);
		}
		cout << "========rank 0 get ===============" << endl;
		restored_info->Print();
		cout << "==================================" << endl;

		for (int i = 0; i < restored_info->myclasses.size(); i++)
			restored_info->myclasses[i]->x = restored_info->myclasses[i]->x * 2;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Отвечаем
		{
			// Save filename data contained in Info object
			std::string serialized;
			{
				// Create an output archive
				std::ofstream ofs("store.dat");
				std::ostringstream oss;
				boost::archive::text_oarchive ar(oss);

				// Save the data
				ar & (*restored_info);
				serialized = oss.str();
			}
			// Получаем размер строки
			int len = serialized.size();
			len++;
			// Переводим строку в массив символов
			char *helloStr = new char[len];
			strcpy(helloStr, serialized.c_str());	// or pass &s[0]

													// Посылаем размер строки (в которой сериализован передаваемый объект) в главный поток
			MPI_Send(
				&len,					//	адрес начала расположения пересылаемых данных; 
				1,						//	число пересылаемых элементов;
				MPI_INT,				//	тип посылаемых элементов;
				1,					//	номер процесса-получателя в группе, связанной с коммуникатором comm;
				(int)Tag::ENUM::SIZE,	//	идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
				MPI_COMM_WORLD			//	коммуникатор области связи.
			);
			// Посылаем саму строку (в которой сериализован передаваемый объект)
			MPI_Send(helloStr, len, MPI_CHAR, 1, (int)Tag::ENUM::MESSAGE, MPI_COMM_WORLD);
		}
		
	}
	else if (rank == 1) {
	
		Info info;
		info.filenames.push_back("rank == 1");
		info.filenames.push_back("rank == 1");
		info.filenames.push_back("rank == 1");
		info.myclasses.push_back(new MyClass(123));
		info.myclasses.push_back(new MyClass(-9));
		info.myclasses.push_back(new MyClass(INT_MAX));

		// Save filename data contained in Info object
		std::string serialized;
		{
			// Create an output archive
			std::ofstream ofs("store.dat");
			std::ostringstream oss;
			boost::archive::text_oarchive ar(oss);

			// Save the data
			ar & info;
			serialized = oss.str();
		}

		// Получаем размер строки
		int len = serialized.size();
		len++;
		// Переводим строку в массив символов
		char *helloStr = new char[len];
		strcpy(helloStr, serialized.c_str());	// or pass &s[0]

		// Посылаем размер строки (в которой сериализован передаваемый объект) в главный поток
		MPI_Send(
			&len,					//	адрес начала расположения пересылаемых данных; 
			1,						//	число пересылаемых элементов;
			MPI_INT,				//	тип посылаемых элементов;
			root,					//	номер процесса-получателя в группе, связанной с коммуникатором comm;
			(int)Tag::ENUM::SIZE,	//	идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
			MPI_COMM_WORLD			//	коммуникатор области связи.
		);
		// Посылаем саму строку (в которой сериализован передаваемый объект)
		MPI_Send(helloStr, len, MPI_CHAR, root, (int)Tag::ENUM::MESSAGE, MPI_COMM_WORLD);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Получаем ответ
		{
			int size;
			// Получаем сообщение в котором лежит размер передаваемой строки (в которой сериализован передаваемый объект)
			//		от конкретного потока
			MPI_Recv(
				&size,								//	адрес начала расположения принимаемого сообщения;
				1,									//	максимальное число принимаемых элементов;
				MPI_INT,							//	тип элементов принимаемого сообщения;
				root,									//	номер процесса-отправителя;
				(int)Tag::ENUM::SIZE,				//	идентификатор сообщения;
				MPI_COMM_WORLD,						//	коммуникатор области связи;
				MPI_STATUS_IGNORE					//	атрибуты принятого сообщения.
			);
			cout << "rank # " << rank << "\t size : " << size << " from  rank # " << 1 << endl;

			// Создаем указатель на строку, с размером полученным в предыдущем сообщении
			char *get = new char[size];
			// Получаем строку (в которой сериализован передаваемый объект) от конкретного потока, размер уже знаем
			MPI_Recv(get, size, MPI_CHAR, root, (int)Tag::ENUM::MESSAGE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("rank 0 get string %s from rank 1\n", get);


			// Restore from saved data and print to verify contents
			Info *restored_info = new Info();
			{
				// Create and input archive
				std::istringstream iss(get);
				boost::archive::text_iarchive ar(iss);

				// Load the data
				ar & (*restored_info);
			}
			cout << "========rank 1 get returned and changed value ===============" << endl;
			restored_info->Print();
			cout << "=============================================================" << endl;
		}
	}
	
	// освобождаем ресурсы
	MPI_Finalize();

	// Если это главный поток, то печатаем сообщение
	if (rank == root) {
		cout << "KONEC" << endl;
	}




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