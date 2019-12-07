#include "stdafx.h"
#include "Info.h"


Info::Info()
{
}


Info::~Info()
{
}

void Info::Print() const
{
	std::cout << "\tFilenames:" << std::endl;
	std::copy(filenames.begin(),
		filenames.end(),
		std::ostream_iterator<std::string>(std::cout, "\n"));
	std::cout << "\tMyclasses:" << std::endl;
	for (int i = 0; i < this->myclasses.size(); i++) {
		std::cout << "[" << i << "] = " << myclasses[i]->x << std::endl;
	}
}

void Info::AddFilename(const std::string& filename)
{
	filenames.push_back(filename);
}
