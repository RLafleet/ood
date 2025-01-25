#ifndef INC_4_SHAPECONTROLLER_H
#define INC_4_SHAPECONTROLLER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <vector>
#include <memory>
#include "Picture.h"
#include "FiguresType.h"
#include "Transmitter.h"
#include "RectanglePaintingStrategy.h"
#include "SVGCanvas.h"

class Controller
{
public:
    Controller(std::istream& in, std::ostream& out, shapes::Picture& picture, gfx::ICanvas& canvas)
        : m_in(in), m_out(out), m_picture(picture), m_canvas(canvas) {}

    void ReadCommands();

private:
    std::istream& m_in;
    std::ostream& m_out;
    shapes::Picture& m_picture;
    gfx::ICanvas& m_canvas;

    void AddShape(std::istringstream& iss);

    uint32_t ParseColor(const std::string& colorStr);

    void MoveShape(std::istringstream& iss);

    void DeleteShape(std::istringstream& iss);

    void AddRectangle(std::istringstream& iss, const std::string& id, uint32_t color);

    void AddCircle(std::istringstream& iss, const std::string& id, uint32_t color);

    void AddTriangle(std::istringstream& iss, const std::string& id, uint32_t color);

    void AddLine(std::istringstream& iss, const std::string& id, uint32_t color);

    void AddText(std::istringstream& iss, const std::string& id, uint32_t color);

    void ListShape();

    void ChangeColor(std::istringstream& iss);

    void MovePicture(std::istringstream& iss);

    void ChangeShape(std::istringstream& iss);

    void DrawShape(std::istringstream& iss);

    void DrawPicture();
};


#endif //INC_4_SHAPECONTROLLER_H