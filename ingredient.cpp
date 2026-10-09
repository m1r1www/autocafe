#include "ingredient.hpp"
#include <iostream>
namespace cafe {
Ingredient::Ingredient() { std::cout << "[Ingredient] создан пустой объект\n"; }
Ingredient::Ingredient(std::string_view name, int amount, int capacity)
    : m_name{name}, m_amount{amount}, m_capacity{capacity} {
    std::cout << "[Ingredient] создан: " << m_name << '\n';
}
Ingredient::~Ingredient() { std::cout << "[Ingredient] уничтожается: " << m_name << '\n'; }
bool Ingredient::Load(int amount) {
    if (amount <= 0 || m_amount + amount > m_capacity) {
        std::cout << "[Ingredient] ОТКАЗ: нельзя добавить " << amount << " ед. ингредиента '" << m_name << "'\n";
        return false;
    }
    m_amount += amount;
    return true;
}
bool Ingredient::Consume(int amount) {
    if (amount <= 0 || amount > m_amount) {
        std::cout << "[Ingredient] ОТКАЗ: недостаточно '" << m_name << "'\n";
        return false;
    }
    m_amount -= amount;
    return true;
}
void Ingredient::Print() const { std::cout << "    " << m_name << ": " << m_amount << '/' << m_capacity << '\n'; }
std::string_view Ingredient::GetName() const { return m_name; }
int Ingredient::GetAmount() const { return m_amount; }
}
