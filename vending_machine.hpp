#pragma once
#include "ingredient.hpp"
#include <string>
#include <string_view>
namespace cafe {
class MachineOperator;
class VendingMachine {
private:
    std::string m_name{};
    Ingredient m_coffee;
    bool m_working{true};
    friend class MachineOperator;
    bool RestockCoffee(int amount);
public:
    VendingMachine(std::string_view name, int coffeeAmount);
    ~VendingMachine();
    bool MakeCoffee();
    void PrintState() const;
    void Stop();
    void Start();
    std::string_view GetName() const;
    bool IsWorking() const;
};
}
