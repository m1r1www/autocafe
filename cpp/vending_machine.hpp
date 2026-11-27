#pragma once

#include "ingredient.hpp"

#include <string>
#include <string_view>

namespace cafe
{

class MachineOperator;

// Автомат: хранит ингредиент «Кофе» (композиция) и состояние, готовит кофе.
// Следит за правилами: заказ только в рабочем состоянии; нет ингредиента — отказ без списания;
// пополнять запасы и менять состояние вправе только оператор.
class VendingMachine
{
private:
    static constexpr int COFFEE_CAPACITY = 100;
    static constexpr int COFFEE_PORTION  = 20;

    friend class MachineOperator; // только оператор вправе пополнять запасы и менять состояние

    std::string m_name{};
    Ingredient m_coffee; // композиция: часть создаётся и уничтожается вместе с автоматом
    bool m_working{ true };

    bool RestockCoffee(int amount);
    void Start();
    void Stop();

public:
    VendingMachine(std::string_view name, int coffeeAmount);
    VendingMachine(const VendingMachine&)            = delete;
    VendingMachine& operator=(const VendingMachine&) = delete;
    ~VendingMachine();

    bool MakeCoffee();
    void PrintState() const;

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] bool IsWorking() const
    {
        return m_working;
    }
};

} // namespace cafe
