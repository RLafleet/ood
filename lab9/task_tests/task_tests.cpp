#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../tasks/Image.h"

class ImageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        while (Tile::GetInstanceCount() > 0)
        {
        }
    }

    void TearDown() override
    {
        EXPECT_EQ(Tile::GetInstanceCount(), 0);
    }
};

TEST_F(ImageTest, CreateImageWithSingleColor)
{
    Image img({ 64, 64 }, '*');

    EXPECT_EQ(img.GetSize().width, 64);
    EXPECT_EQ(img.GetSize().height, 64);

    for (int y = 0; y < img.GetSize().height; ++y)
    {
        for (int x = 0; x < img.GetSize().width; ++x)
        {
            EXPECT_EQ(img.GetPixel({ x, y }), '*');
        }
    }

    EXPECT_EQ(Tile::GetInstanceCount(), 1);
}

TEST_F(ImageTest, SetAndGetPixel)
{
    Image img({ 16, 16 }, '.');
    EXPECT_EQ(Tile::GetInstanceCount(), 1);

    img.SetPixel({ 0, 0 }, '#');
    EXPECT_EQ(img.GetPixel({ 0, 0 }), '#');
    EXPECT_EQ(Tile::GetInstanceCount(), 2);

    img.SetPixel({ 10, 10 }, '@');
    EXPECT_EQ(img.GetPixel({ 10, 10 }), '@');
    EXPECT_EQ(Tile::GetInstanceCount(), 3);

    img.SetPixel({ 15, 15 }, 'X');
    EXPECT_EQ(img.GetPixel({ 15, 15 }), 'X');
    EXPECT_EQ(Tile::GetInstanceCount(), 3);
}

TEST_F(ImageTest, GetPixelOutOfBounds)
{
    Image img({ 16, 16 }, '.');

    EXPECT_EQ(img.GetPixel({ -1, -1 }), 0xFFFFFF);
    EXPECT_EQ(img.GetPixel({ 16, 16 }), 0xFFFFFF);
}

TEST_F(ImageTest, SetPixelOutOfBounds)
{
    Image img({ 16, 16 }, '.');

    img.SetPixel({ -1, -1 }, '#');
    EXPECT_EQ(img.GetPixel({ -1, -1 }), 0xFFFFFF);

    img.SetPixel({ 16, 16 }, '#');
    EXPECT_EQ(img.GetPixel({ 16, 16 }), 0xFFFFFF);
}

TEST_F(ImageTest, TileInstanceCount)
{
    {
        Image img1({ 8, 8 }, '*');
        EXPECT_EQ(Tile::GetInstanceCount(), 1);

        Image img2({ 8, 8 }, '#');
        EXPECT_EQ(Tile::GetInstanceCount(), 2);

        img1.SetPixel({ 0, 0 }, '@');
        EXPECT_EQ(Tile::GetInstanceCount(), 3);
        EXPECT_TRUE((img1.GetPixel({ 0, 0 }) == '@'));
        EXPECT_TRUE(!(img2.GetPixel({ 0, 0 }) == '@'));

        img2.SetPixel({ 0, 0 }, '&');
        EXPECT_EQ(Tile::GetInstanceCount(), 4);
        EXPECT_TRUE((img2.GetPixel({ 0, 0 }) == '&'));
        EXPECT_TRUE(!(img1.GetPixel({ 0, 0 }) == '&'));
    }

    EXPECT_EQ(Tile::GetInstanceCount(), 0);
}

class TileTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        while (Tile::GetInstanceCount() > 0)
        {
        }
    }

    void TearDown() override
    {
        EXPECT_EQ(Tile::GetInstanceCount(), 0);
    }
};

TEST_F(TileTest, DefaultConstructor)
{
    Tile tile;
    for (int y = 0; y < Tile::SIZE; ++y)
    {
        for (int x = 0; x < Tile::SIZE; ++x)
        {
            EXPECT_EQ(tile.GetPixel({ x, y }), 0xFFFFFF);
        }
    }
    EXPECT_EQ(Tile::GetInstanceCount(), 1);
}

TEST_F(TileTest, ParameterizedConstructor)
{
    Tile tile('*');
    EXPECT_EQ(Tile::GetInstanceCount(), 1);

    for (int y = 0; y < Tile::SIZE; ++y)
    {
        for (int x = 0; x < Tile::SIZE; ++x)
        {
            EXPECT_EQ(tile.GetPixel({ x, y }), '*');
        }
    }
}

TEST_F(TileTest, CopyConstructor)
{
    Tile tile1('*');
    EXPECT_EQ(Tile::GetInstanceCount(), 1);

    Tile tile2(tile1);
    EXPECT_EQ(Tile::GetInstanceCount(), 2);

    for (int y = 0; y < Tile::SIZE; ++y)
    {
        for (int x = 0; x < Tile::SIZE; ++x)
        {
            EXPECT_EQ(tile2.GetPixel({ x, y }), '*');
        }
    }
}

TEST_F(TileTest, SetPixel)
{
    Tile tile(0xFFFFFF);

    tile.SetPixel({ 0, 0 }, '#');
    EXPECT_EQ(tile.GetPixel({ 0, 0 }), '#');

    tile.SetPixel({ 7, 7 }, '@');
    EXPECT_EQ(tile.GetPixel({ 7, 7 }), '@');

    tile.SetPixel({ 8, 8 }, 'X');
    EXPECT_EQ(tile.GetPixel({ 8, 8 }), 0xFFFFFF);
}

TEST_F(TileTest, GetPixelOutOfBounds)
{
    Tile tile('*');

    EXPECT_EQ(tile.GetPixel({ -1, -1 }), 0xFFFFFF);
    EXPECT_EQ(tile.GetPixel({ 8, 8 }), 0xFFFFFF);
}

TEST_F(TileTest, DestructorDecreasesInstanceCount)
{
    {
        Tile tile;
        EXPECT_EQ(Tile::GetInstanceCount(), 1);
    }
    EXPECT_EQ(Tile::GetInstanceCount(), 0);
}

GTEST_API_ int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}