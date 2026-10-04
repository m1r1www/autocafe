#include "ingredient.hpp"

#include <iostream>

namespace cafe
{

std::string_view ToString(IngredientStatus status)
{
    switch (status)
    {
    case IngredientStatus::eOk:
        return "успешно";
    case IngredientStatus::eInvalidAmount:
        return "некорректное количество";
    case IngredientStatus::eOverCapacity:
        return "превышена вместимость";
    case IngredientStatus::eExpired:
        return "ингредиент просрочен";
    case IngredientStatus::eInsufficient:
        return "недостаточный запас";
    }
    return "неизвестно";
}

Ingredient::Ingredient()
{
    std::cout << "    [Ingredient] создан пустой ингредиент (по умолчанию)\n";
}

Ingredient::Ingredient(std::string_view name, int capacity, int amount, int expiryDay)
{
    Setup(name, capacity, amount, expiryDay);
    std::cout << "    [Ingredient] создан: " << m_name << '\n';
}

Ingredient::~Ingredient()
{
    std::cout << "    [Ingredient] уничтожается: " << m_name << '\n';
}

IngredientStatus Ingredient::Fail(IngredientStatus status) const
{
    std::cout << "    [Ingredient] ОТКАЗ: " << ToString(status) << " ('" << m_name << "')\n";
    return status;
}

IngredientStatus Ingredient::Validate(int capacity, int amount)
{
    if (capacity <= 0 || amount < 0)
    {
        return IngredientStatus::eInvalidAmount;
    }

    if (amount > capacity)
    {
        return IngredientStatus::eOverCapacity;
    }

    return IngredientStatus::eOk;
}

IngredientStatus Ingredient::Setup(std::string_view name, int capacity, int amount, int expiryDay)
{
    if (const auto status = Validate(capacity, amount); status != IngredientStatus::eOk)
    {
        return Fail(status);
    }

    m_name      = std::string(name);
    m_capacity  = capacity;
    m_amount    = amount;
    m_expiryDay = expiryDay;
    return IngredientStatus::eOk;
}

IngredientStatus Ingredient::Load(int value)
{
    if (value <= 0)
    {
        return Fail(IngredientStatus::eInvalidAmount);
    }

    if (m_amount + value > m_capacity)
    {
        return Fail(IngredientStatus::eOverCapacity);
    }

    m_amount += value;
    return IngredientStatus::eOk;
}

IngredientStatus Ingredient::Consume(int value, int today)
{
    if (value <= 0)
    {
        return Fail(IngredientStatus::eInvalidAmount);
    }

    if (IsExpired(today))
    {
        return Fail(IngredientStatus::eExpired);
    }

    if (value > m_amount)
    {
        return Fail(IngredientStatus::eInsufficient);
    }

    m_amount -= value;
    return IngredientStatus::eOk;
}

bool Ingredient::IsExpired(int today) const
{
    return today > m_expiryDay;
}

bool Ingredient::CanProvide(int value, int today) const
{
    return value > 0 && !IsExpired(today) && value <= m_amount;
}

void Ingredient::Print(int today) const
{
    std::cout << "    " << m_name << ": " << m_amount << '/' << m_capacity << ", годен до дня " << m_expiryDay
              << (IsExpired(today) ? " (ПРОСРОЧЕН)" : "") << '\n';
}

} // namespace cafe
