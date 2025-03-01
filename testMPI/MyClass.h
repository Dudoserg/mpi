#include <iostream>
#include <vector>;
#include <fstream>;
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#pragma once
class MyClass
{
public:
	MyClass();
	MyClass(int x);
	~MyClass();

	int x;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & x;
	}
};

