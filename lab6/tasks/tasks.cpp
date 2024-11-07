#include <iostream>
#include <string>

#include "Menu.h"


int main()
{
	std::cout << "New api or old?\nPlease enter \"y\" for new and \"n\" for old:  ";

	std::string line;
	const auto shouldUseNewApi = getline(std::cin, line) && (line == "y" || line == "Y");

	if (shouldUseNewApi)
	{
		app::PaintPictureOnModernGraphicsRenderer();
	}
	else
	{
		app::PaintPictureOnCanvas();
	}

	return 0;
}