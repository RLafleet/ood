#include <iostream>
#include "Image.h"
#include "ImageController.h"
#include "Drawer.h"

void DrawPicture()
{
	Image img({ 40, 30 }, 0xFFFFFF);

	Drawer::DrawLine(img, { 5, 20 }, { 35, 20 }, 0x00FF00);
	Drawer::DrawLine(img, { 5, 20 }, { 10, 10 }, 0x00FF00);
	Drawer::DrawLine(img, { 10, 10 }, { 30, 10 }, 0x00FF00);
	Drawer::DrawLine(img, { 30, 10 }, { 35, 20 }, 0x00FF00);
	Drawer::DrawLine(img, { 5, 20 }, { 5, 25 }, 0x00FF00);
	Drawer::DrawLine(img, { 35, 20 }, { 35, 25 }, 0x00FF00);
	Drawer::DrawLine(img, { 5, 25 }, { 35, 25 }, 0x000000);

	Drawer::FillCircle(img, { 12, 26 }, 3, 0x000000);
	Drawer::FillCircle(img, { 28, 26 }, 3, 0x000000);
	Drawer::DrawCircle(img, { 12, 26 }, 3, 0xFF8844);
	Drawer::DrawCircle(img, { 28, 26 }, 3, 0xFF8844);

	ImageController::Print(img, std::cout);
	std::cout << std::endl;

	ImageController::SaveImageToPPM(img, "picture.ppm");
}

void DrawSamplePicture()
{
	const Image img = ImageController::LoadImage(
		" /\\     /\\\n"
		"{  `---'  }\n"
		"{  O   O  }\n"
		"~~>  V  <~~\n"
		" \\ \\| |/ /\n"
		"  `-----'____\n"
		"  /     \\    \\_\n"
	);
	ImageController::Print(img, std::cout);
}

int main()
{
	DrawSamplePicture();
	DrawPicture();
	return 0;
}