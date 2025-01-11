#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <fstream>
#include <sstream>

#include "Image.h"

class ImageController
{
public:
	static void Print(const Image& img, std::ostream& out)
	{
		const auto [width, height] = img.GetSize();
		for (int y = 0; static_cast<unsigned int>(y) < height; ++y)
		{
			for (int x = 0; static_cast<unsigned int>(x) < width; ++x)
			{
				out.put(static_cast<char>(img.GetPixel({ x, y })));
			}
			out.put('\n');
		}
	}

	static Image LoadImage(const std::string& pixels)
	{
		std::istringstream s(pixels);
		Size size{};
		std::string line;
		while (std::getline(s, line))
		{
			size.width = std::max(size.width, static_cast<unsigned>(line.length()));
			++size.height;
		}

		Image img(size);

		s = std::istringstream(pixels);
		for (int y = 0; static_cast<unsigned int>(y) < size.height; ++y)
		{
			if (!std::getline(s, line))
			{
				break;
			}

			int x = 0;
			for (const char ch : line)
			{
				img.SetPixel({ x++, y }, ch);
			}
		}

		return img;
	}

	static void SaveImageToPPM(const Image& image, const std::string& filename)
	{
		std::ofstream file(filename);
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open the file for writing");
		}

		file << "P3\n";
		file << image.GetSize().width << " " << image.GetSize().height << "\n";
		file << "255\n";

		for (int y = 0; static_cast<unsigned int>(y) < image.GetSize().height; ++y)
		{
			for (int x = 0; static_cast<unsigned int>(x) < image.GetSize().width; ++x)
			{
				const uint32_t color = image.GetPixel({ x, y });

				const uint8_t r = (color >> 16) & 0xFF;
				const uint8_t g = (color >> 8) & 0xFF;
				const uint8_t b = color & 0xFF;

				file << static_cast<int>(r) << " "
					<< static_cast<int>(g) << " "
					<< static_cast<int>(b) << " ";
			}
			file << "\n";
		}

		file.close();
	}
};

#endif //IMAGECONTROLLER_H