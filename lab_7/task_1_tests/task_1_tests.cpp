#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"

#include "../task_1/GroupShape.h"
#include "../task_1/CRectangle.h"
#include "../task_1/CEllipse.h"
#include "../task_1/СStyle\.h"

TEST_CASE("GroupShape: Add and Remove Shapes")
{
    GroupShape group;

    auto rectangle = std::make_shared<CRectangle>(
        RectD{ 0, 0, 100, 100 },
        std::make_unique<СStyle>(true, 0xFF0000),
        std::make_unique<СStyle>(true, 0x00FF00)
    );

    auto ellipse = std::make_shared<CEllipse>(
        RectD{ 50, 50, 150, 150 },
        std::make_unique<СStyle>(true, 0xFF0000),
        std::make_unique<СStyle>(true, 0x00FF00)
    );

    REQUIRE(group.GetShapesCount() == 0);

    SECTION("Add shapes")
    {
        group.InsertShape(rectangle, 0);
        group.InsertShape(ellipse, 1);

        REQUIRE(group.GetShapesCount() == 2);
        REQUIRE(group.GetShapeAtIndex(0) == rectangle);
        REQUIRE(group.GetShapeAtIndex(1) == ellipse);
    }

    SECTION("Remove shapes")
    {
        group.InsertShape(rectangle, 0);
        group.InsertShape(ellipse, 1);

        group.RemoveShapeAtIndex(0);

        REQUIRE(group.GetShapesCount() == 1);
        REQUIRE(group.GetShapeAtIndex(0) == ellipse);
    }

    SECTION("Out of range access")
    {
        REQUIRE_THROWS_AS(group.GetShapeAtIndex(0), std::out_of_range);
        REQUIRE_THROWS_AS(group.RemoveShapeAtIndex(0), std::out_of_range);
    }
}

TEST_CASE("GroupShape: Get and Set Frame")
{
    GroupShape group;

    auto rectangle = std::make_shared<CRectangle>(
        RectD{ 0, 0, 100, 100 },
        std::make_unique<СStyle>(true, 0xFF0000),
        std::make_unique<СStyle>(true, 0x00FF00)
    );

    auto ellipse = std::make_shared<CEllipse>(
        RectD{ 50, 50, 150, 150 },
        std::make_unique<СStyle>(true, 0xFF0000),
        std::make_unique<СStyle>(true, 0x00FF00)
    );

    rectangle->SetFrame({ 0, 0, 100, 100 });
    ellipse->SetFrame({ 50, 50, 150, 150 });

    group.InsertShape(rectangle, 0);
    group.InsertShape(ellipse, 1);

    SECTION("Calculate group frame")
    {
        RectD frame = group.GetFrame();

        REQUIRE(frame.left == 0);
        REQUIRE(frame.top == 0);
        REQUIRE(frame.width == 200);
        REQUIRE(frame.height == 200);
    }

    SECTION("Update group frame")
    {
        group.SetFrame({ 0, 0, 400, 400 });

        RectD rectangleFrame = rectangle->GetFrame();
        RectD ellipseFrame = ellipse->GetFrame();

        REQUIRE(rectangleFrame.left == 0);
        REQUIRE(rectangleFrame.top == 0);
        REQUIRE(rectangleFrame.width == 200);
        REQUIRE(rectangleFrame.height == 200);

        REQUIRE(ellipseFrame.left == 200);
        REQUIRE(ellipseFrame.top == 200);
        REQUIRE(ellipseFrame.width == 200);
        REQUIRE(ellipseFrame.height == 200);
    }
}

TEST_CASE("CompositeStyle: Uniform styles")
{
    auto rectangle = std::make_shared<CRectangle>(
        RectD{ 0, 0, 100, 100 },
        std::make_unique<СStyle>(true, 0xFF0000),
        std::make_unique<СStyle>(true, 0x00FF00)
    );

    auto ellipse = std::make_shared<CEllipse>(
        RectD{ 50, 50, 150, 150 },
        std::make_unique<СStyle>(true, 0xFF0000),
        std::make_unique<СStyle>(true, 0x00FF00)
    );

    std::vector<std::shared_ptr<IShape>> shapes = { rectangle, ellipse };
    CompositeStyle outlineStyleProxy(true, shapes);

    SECTION("Check uniform style")
    {
        REQUIRE(outlineStyleProxy.IsEnabled() == true);
        REQUIRE(outlineStyleProxy.GetColor() == 0xFF0000);
    }

    SECTION("Change style through proxy")
    {
        outlineStyleProxy.SetColor(0x00FF00);
        outlineStyleProxy.SetIsEnabled(false);

        REQUIRE(rectangle->GetOutlineStyle().GetColor() == 0x00FF00);
        REQUIRE(rectangle->GetOutlineStyle().IsEnabled() == false);
        REQUIRE(ellipse->GetOutlineStyle().GetColor() == 0x00FF00);
        REQUIRE(ellipse->GetOutlineStyle().IsEnabled() == false);
    }
}

TEST_CASE("CompositeStyle: Mixed styles")
{
    auto rectangle = std::make_shared<CRectangle>(
        RectD{ 0, 0, 100, 100 },
        std::make_unique<СStyle\>(true, 0xFF0000),
        std::make_unique<СStyle\>(true, 0x00FF00)
    );

    auto ellipse = std::make_shared<CEllipse>(
        RectD{ 50, 50, 150, 150 },
        std::make_unique<СStyle\>(true, 0x0000FF),
        std::make_unique<СStyle\>(false, std::nullopt)
    );

    std::vector<std::shared_ptr<IShape>> shapes = { rectangle, ellipse };
    CompositeStyle fillStyleProxy(false, shapes);

    SECTION("Check mixed styles")
    {
        REQUIRE(fillStyleProxy.IsEnabled() == std::nullopt);
        REQUIRE(fillStyleProxy.GetColor() == std::nullopt);
    }

    SECTION("Update style to uniform through proxy")
    {
        fillStyleProxy.SetColor(0x0000FF);
        fillStyleProxy.SetIsEnabled(true);

        REQUIRE(rectangle->GetFillStyle().GetColor() == 0x0000FF);
        REQUIRE(rectangle->GetFillStyle().IsEnabled() == true);
        REQUIRE(ellipse->GetFillStyle().GetColor() == 0x0000FF);
        REQUIRE(ellipse->GetFillStyle().IsEnabled() == true);
    }
}