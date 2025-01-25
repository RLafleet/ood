#ifndef INC_1_BEVERAGES_H
#define INC_1_BEVERAGES_H

#pragma once

#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class CBeverage : public IBeverage
{
public:
	explicit CBeverage(std::string description)
		: m_description(std::move(description))
	{}

	[[nodiscard]] std::string GetDescription()const final
	{
		return m_description;
	}
private:
	std::string m_description;
};

// Кофе
class CCoffee : public CBeverage
{
public:
	explicit CCoffee(const std::string& description = "Coffee")
		:CBeverage(description)
	{}

	double GetCost() const override
	{
		return 60;
	}
};

// Капуччино
class CCappuccino : public CCoffee
{
public:
	explicit CCappuccino(bool isDoublePortion = false)
		: CCoffee(isDoublePortion ? "Double Cappuccino" : "Standard Cappuccino"), m_isDoublePortion(isDoublePortion)
	{}

	double GetCost() const override
	{
		return m_isDoublePortion ? 120 : 80;
	}

private:
	bool m_isDoublePortion;
};

// Латте
class CLatte : public CCoffee
{
public:
	explicit CLatte(bool isDoublePortion = false)
		: CCoffee(isDoublePortion ? "Double Latte" : "Standard Latte"), m_isDoublePortion(isDoublePortion)
	{}

	double GetCost() const override
	{
		return m_isDoublePortion ? 130 : 90;;
	}

private:
	bool m_isDoublePortion;
};

enum class TeaType
{
	BLACK,
	GREEN,
	RED,
};

std::string TeaTypeToString(TeaType teaType)
{
	switch (teaType)
	{
	case TeaType::BLACK: return "Black";
	case TeaType::GREEN: return "Green";
	case TeaType::RED:   return "Red";
	default:             return "Unknown Tea Type";
	}
}

// Чай
class CTea : public CBeverage
{
public:
	explicit CTea(TeaType teaType)
		:CBeverage(TeaTypeToString(teaType) + " Tea")
	{}

	double GetCost() const override
	{
		return 30;
	}
};

enum class MilkshakeSizeType
{
	SMALL,
	MEDIUM,
	LARGE,
};

std::string MilkshakeSizeTypeToString(MilkshakeSizeType sizeType)
{
	switch (sizeType)
	{
	case MilkshakeSizeType::SMALL:  return "Small";
	case MilkshakeSizeType::MEDIUM: return "Medium";
	case MilkshakeSizeType::LARGE:  return "Large";
	default:                        return "Unknown Size Type";
	}
}


// Молочный коктейль
class CMilkshake : public CBeverage
{
public:
	explicit CMilkshake(MilkshakeSizeType sizeType)
		: CBeverage(MilkshakeSizeTypeToString(sizeType) + " Milkshake")
	{}

	double GetCost() const override
	{
		return 80;
	}
};

#endif //INC_1_BEVERAGES_H