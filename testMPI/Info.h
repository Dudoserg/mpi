#include <iostream>
#include <vector>;
#include <fstream>;
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "MyClass.h"

#pragma once
class Info
{


public:
	std::vector<std::string> filenames;

	std::vector<MyClass*> myclasses;

	Info();
	~Info();
	void AddFilename(const std::string& filename);
	void Print() const;

	// Serialize the std::vector member of Info
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & filenames;
		ar & myclasses;
	}

};