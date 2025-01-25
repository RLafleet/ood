#include <iostream>
#include "Image.h"
#include "ImageController.h"
#include "Drawer.h"

void DrawPicture()
{
    {
        Image img({ 30, 20 }, '.');

        Drawer::DrawLine(img, { 15, 18 }, { 15, 12 }, '#');

        Drawer::DrawCircle(img, { 15, 8 }, 6, '#');

        Drawer::DrawCircle(img, { 13, 6 }, 1, '0');
        Drawer::DrawCircle(img, { 17, 7 }, 1, '0');
        Drawer::DrawCircle(img, { 14, 10 }, 1, '0');
        Drawer::DrawCircle(img, { 16, 9 }, 1, '0');

        ImageController::Print(img, std::cout);
    }

    {
        Image img({ 30, 20 }, 0xFFFFFF);

        Drawer::DrawLine(img, { 15, 18 }, { 15, 10 }, 0x8B4513);

        Drawer::DrawLine(img, { 8, 10 }, { 22, 10 }, 0x00FF00);
        Drawer::DrawLine(img, { 8, 10 }, { 15, 1 }, 0x00FF00);
        Drawer::DrawLine(img, { 22, 10 }, { 15, 1 }, 0x00FF00);

        Drawer::DrawLine(img, { 10, 8 }, { 20, 8 }, 0x00FF00);
        Drawer::DrawLine(img, { 11, 6 }, { 19, 6 }, 0x00FF00);
        Drawer::DrawLine(img, { 12, 4 }, { 18, 4 }, 0x00FF00);
        Drawer::DrawLine(img, { 13, 2 }, { 17, 2 }, 0x00FF00);

        Drawer::DrawLine(img, { 11, 9 }, { 15, 3 }, 0x00FF00);
        Drawer::DrawLine(img, { 19, 9 }, { 15, 3 }, 0x00FF00);

        Drawer::FillCircle(img, { 12, 7 }, 1, 0xFF0000);
        Drawer::FillCircle(img, { 18, 8 }, 1, 0xFF0000);
        Drawer::FillCircle(img, { 13, 9 }, 1, 0xFF0000);
        Drawer::FillCircle(img, { 17, 6 }, 1, 0xFF0000);

	    ImageController::SaveImageToPPM(img, "picture.ppm");
    }
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
    // потворить отношения между классами
	DrawSamplePicture();
	DrawPicture();
	return 0;
}