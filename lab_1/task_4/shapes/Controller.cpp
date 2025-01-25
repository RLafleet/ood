#include "Controller.h"

void Controller::ReadCommands()
{
    std::string line;


    while (std::getline(m_in, line))
    {
        if (line.empty())
        {
            break;
        }

        std::istringstream iss(line);
        std::string commandName;
        iss >> commandName;

        if (commandName == "AddShape")
        {
            std::cout << "AddShape" << std::endl;
            AddShape(iss);
        }
        else if (commandName == "MoveShape")
        {
            MoveShape(iss);
        }
        else if (commandName == "DeleteShape")
        {
            DeleteShape(iss);
        }
        else if (commandName == "List")
        {
            ListShape();
        }
        else if (commandName == "ChangeColor")
        {
            ChangeColor(iss);
        }
        else if (commandName == "MovePicture")
        {
            MovePicture(iss);
        }
        else if (commandName == "ChangeShape")
        {
            ChangeShape(iss);
        }
        else if (commandName == "DrawShape")
        {
            DrawShape(iss);
        }
        else if (commandName == "DrawPicture")
        {
            DrawPicture();
        }
    }
}

void Controller::AddShape(std::istringstream& iss)
{
    std::string id;
    uint32_t color;
    std::string type;

    iss >> id;

    std::string colorStr;
    iss >> colorStr;
    color = ParseColor(colorStr);

    iss >> type;

    if (type == Transmitter::ConvertShapeTypeToString(FiguresType::RECTANGLE)) {
        AddRectangle(iss, id, color);
    }
    else if (type == Transmitter::ConvertShapeTypeToString(FiguresType::CIRCLE)) {
        AddCircle(iss, id, color);
    }
    else if (type == Transmitter::ConvertShapeTypeToString(FiguresType::TRIANGLE)) {
        AddTriangle(iss, id, color);
    }
    else if (type == Transmitter::ConvertShapeTypeToString(FiguresType::LINE)) {
        AddLine(iss, id, color);
    }
    else if (type == Transmitter::ConvertShapeTypeToString(FiguresType::TEXT)) {
        AddText(iss, id, color);
    }
}

uint32_t Controller::ParseColor(const std::string& colorStr) 
{
    if (colorStr[0] == '#') 
    {
        std::string hexColor = colorStr.substr(1);
        uint32_t color;
        std::stringstream ss;
        ss << std::hex << hexColor;
        ss >> color;
        return color;
    }
    return 0; 
}

void Controller::AddRectangle(std::istringstream& iss, const std::string& id, uint32_t color)
{
    double leftTopX, leftTopY, width, height;
    iss >> leftTopX >> leftTopY >> width >> height;

    shapes::Point topLeft(leftTopX, leftTopY);

    auto rectangleDrawingStrategy = std::make_unique<shapes::RectanglePaintingStrategy>(topLeft, width, height);
    auto shape = std::make_unique<shapes::Shape>(id, color, std::move(rectangleDrawingStrategy));
    m_picture.AddShape(id, std::move(shape));
}

void Controller::AddCircle(std::istringstream& iss, const std::string& id, uint32_t color) 
{
    double centerX, centerY, radius;
    iss >> centerX >> centerY >> radius;

    auto circleDrawingStrategy = std::make_unique<shapes::CirclePaintingStrategy>(centerX, centerY, radius);
    auto shape = std::make_unique<shapes::Shape>(id, color, std::move(circleDrawingStrategy));

    m_picture.AddShape(id, std::move(shape));
}

void Controller::AddTriangle(std::istringstream& iss, const std::string& id, uint32_t color)
{
    double x1, y1, x2, y2, x3, y3;
    iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    shapes::Point point1(x1, y1);
    shapes::Point point2(x2, y2);
    shapes::Point point3(x3, y3);

    auto triangleDrawingStrategy = std::make_unique<shapes::TrianglePaintingStrategy>(point1, point2, point3);
    auto shape = std::make_unique<shapes::Shape>(id, color, std::move(triangleDrawingStrategy));
    m_picture.AddShape(id, std::move(shape));
}

void Controller::AddLine(std::istringstream& iss, const std::string& id, uint32_t color) 
{
    double x1, y1, x2, y2;
    iss >> x1 >> y1 >> x2 >> y2;

    shapes::Point startPoint(x1, y1);
    shapes::Point endPoint(x2, y2);

    auto lineDrawingStrategy = std::make_unique<shapes::LinePaintingStrategy>(startPoint, endPoint);
    auto shape = std::make_unique<shapes::Shape>(id, color, std::move(lineDrawingStrategy));

    m_picture.AddShape(id, std::move(shape));
}

void Controller::AddText(std::istringstream& iss, const std::string& id, uint32_t color) 
{
    double leftTopX, leftTopY, size;
    iss >> leftTopX >> leftTopY >> size;

    std::string text;
    std::getline(iss >> std::ws, text); 

    auto textDrawingStrategy = std::make_unique<shapes::TextPaintingStrategy>(leftTopX, leftTopY, size, text);
    auto shape = std::make_unique<shapes::Shape>(id, color, std::move(textDrawingStrategy));

    m_picture.AddShape(id, std::move(shape));
}

void Controller::MoveShape(std::istringstream& iss)
{
    std::string id;
    std::string dx;
    std::string dy;

    iss >> id >> dx >> dy;

    m_picture.MoveShape(id, std::stod(dx), std::stod(dy));
}

void Controller::DeleteShape(std::istringstream& iss)
{
    std::string id;

    iss >> id;

    m_picture.DeleteShape(id);
}

void Controller::ListShape()
{
    m_picture.List([](const std::string& type, const std::string& id, uint32_t color, const std::string& strategy) {
        std::cout << type << " " << id << " #" << std::hex << std::setw(6) << std::setfill('0') << color << std::defaultfloat << " " << strategy << std::endl;
        });;
}

void Controller::ChangeColor(std::istringstream& iss)
{
    std::string id;
    uint32_t color;

    iss >> id;
    iss >> std::hex >> color;

    m_picture.ChangeColor(id, color);
}

void Controller::MovePicture(std::istringstream& iss)
{
    std::string dx;
    std::string dy;

    iss >> dx >> dy;
    m_picture.MovePicture(std::stod(dx), std::stod(dy));
}

// разбить тна методы
void Controller::ChangeShape(std::istringstream& iss)
{
    std::vector<double> parameters;
    std::string id;
    uint32_t color;
    std::string type;

    iss >> id;

    std::string colorStr;
    iss >> colorStr;

    if (colorStr[0] == '#') {
        colorStr.erase(0, 1);
        std::stringstream ss;
        ss << std::hex << colorStr;
        ss >> color;
    }

    iss >> type;

    std::string rectangleString = Transmitter::ConvertShapeTypeToString(FiguresType::RECTANGLE);
    std::string circleString = Transmitter::ConvertShapeTypeToString(FiguresType::CIRCLE);
    std::string triangleString = Transmitter::ConvertShapeTypeToString(FiguresType::TRIANGLE);
    std::string lineString = Transmitter::ConvertShapeTypeToString(FiguresType::LINE);
    std::string textString = Transmitter::ConvertShapeTypeToString(FiguresType::TEXT);

    if (type == rectangleString)
    {
        std::string leftTopXStr;
        std::string leftTopYStr;
        std::string widthStr;
        std::string heightStr;

        iss >> leftTopXStr >> leftTopYStr >> widthStr >> heightStr;

        double leftTopX = std::stod(leftTopXStr);
        double leftTopY = std::stod(leftTopYStr);
        double width = std::stod(widthStr);
        double height = std::stod(heightStr);

        shapes::Point topLeft(leftTopX, leftTopY);
        auto rectangleDrawingStrategy = std::make_unique<shapes::RectanglePaintingStrategy>(topLeft, width, height);
        m_picture.ChangeShape(id, std::move(rectangleDrawingStrategy));
    }
    else if (type == circleString)
    {
        std::string centerXStr;
        std::string centerYStr;
        std::string radiusStr;

        iss >> centerXStr >> centerYStr >> radiusStr;

        double centerX = std::stod(centerXStr);
        double centerY = std::stod(centerYStr);
        double radius = std::stod(radiusStr);

        auto circleDrawingStrategy = std::make_unique<shapes::CirclePaintingStrategy>(centerX, centerY, radius);

        m_picture.ChangeShape(id, std::move(circleDrawingStrategy));
    }
    else if (type == triangleString)
    {
        std::string x1Str, y1Str, x2Str, y2Str, x3Str, y3Str;

        iss >> x1Str >> y1Str >> x2Str >> y2Str >> x3Str >> y3Str;

        double x1 = std::stod(x1Str);
        double y1 = std::stod(y1Str);
        double x2 = std::stod(x2Str);
        double y2 = std::stod(y2Str);
        double x3 = std::stod(x3Str);
        double y3 = std::stod(y3Str);

        shapes::Point point1(x1, y1);
        shapes::Point point2(x2, y2);
        shapes::Point point3(x3, y3);

        auto triangleDrawingStrategy = std::make_unique<shapes::TrianglePaintingStrategy>(point1, point2, point3);

        m_picture.ChangeShape(id, std::move(triangleDrawingStrategy));
    }
    else if (type == lineString)
    {
        std::string x1Str, y1Str, x2Str, y2Str;
        iss >> x1Str >> y1Str >> x2Str >> y2Str;

        double x1 = std::stod(x1Str);
        double y1 = std::stod(y1Str);
        double x2 = std::stod(x2Str);
        double y2 = std::stod(y2Str);

        shapes::Point startPoint(x1, y1);
        shapes::Point endPoint(x2, y2);

        auto lineDrawingStrategy = std::make_unique<shapes::LinePaintingStrategy>(startPoint, endPoint);

        m_picture.ChangeShape(id, std::move(lineDrawingStrategy));
    }
    else if (type == textString)
    {
        std::string leftTopXStr;
        std::string leftTopYStr;
        std::string sizeStr;
        std::string text;

        iss >> leftTopXStr >> leftTopYStr >> sizeStr;

        std::getline(iss, text);
        if (!text.empty() && text[0] == ' ')
        {
            text.erase(0, 1);
        }

        double leftTopX = std::stod(leftTopXStr);
        double leftTopY = std::stod(leftTopYStr);
        double size = std::stod(sizeStr);

        auto textDrawingStrategy = std::make_unique<shapes::TextPaintingStrategy>(leftTopX, leftTopY, size, text);

        m_picture.ChangeShape(id, std::move(textDrawingStrategy));
    }
}

void Controller::DrawShape(std::istringstream& iss)
{
    std::string id;
    iss >> id;

    m_picture.DrawShape(id, m_canvas);
}

void Controller::DrawPicture()
{
    m_picture.DrawPicture(m_canvas);
}