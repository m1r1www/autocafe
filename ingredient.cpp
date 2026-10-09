#include "ingredient.hpp"

#include <algorithm>
#include <iostream>

namespace cafe
{

Ingredient::Ingredient()
{
    std::cout << "[Ingredient] создан пустой объект\n";
}

Ingredient::Ingredient(std::string_view name, int amount, int capacity)
{
    Setup(name, amount, capacity);
    std::cout << "[Ingredient] создан: " << m_name << '\n';
}

Ingredient::~Ingredient()
{
    std::cout << "[Ingredient] уничтожается: " << m_name << '\n';
}

bool Ingredient::Setup(std::string_view name, int amount, int capacity)
{
    m_name     = std::string(name);
    m_capacity = std::max(capacity, 0);
    m_amount   = std::clamp(amount, 0, m_capacity);

    if (m_amount != amount || m_capacity != capacity)
    {
        std::cout << "[Ingredient] ОТКАЗ: запас '" << m_name << "' должен быть в пределах 0.." << m_capacity
                  << ", установлено " << m_amount << '\n';
        return false;
    }
    return true;
}

bool Ingredient::Load(int amount)
{
    if (amount <= 0 || m_amount + amount > m_capacity)
    {
        std::cout << "[Ingredient] ОТКАЗ: нельзя добавить " << amount << " ед. ингредиента '" << m_name << "'\n";
        return false;
    }

    m_amount += amount;
    return true;
}

bool Ingredient::Consume(int amount)
{
    if (amount <= 0 || amount > m_amount)
    {
        std::cout << "[Ingredient] ОТКАЗ: недостаточно '" << m_name << "'\n";
        return false;
    }

    m_amount -= amount;
    return true;
}

void Ingredient::Print() const
{
    std::cout << "    " << m_name << ": " << m_amount << '/' << m_capacity << '\n';
}

} // namespace cafe
