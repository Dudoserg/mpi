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
	std::copy(filenames.begin(),
		filenames.end(),
		std::ostream_iterator<std::string>(std::cout, "\n"));
}

void Info::AddFilename(const std::string& filename)
{
	filenames.push_back(filename);
}
