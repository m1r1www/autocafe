#include "vending_machine.hpp"

#include <iostream>

namespace cafe
{

VendingMachine::VendingMachine(std::string_view name, int coffeeAmount)
    : m_name{ name }
    , m_coffee{ "Кофе", coffeeAmount, COFFEE_CAPACITY }
{
    std::cout << "[VendingMachine] создан: " << m_name << '\n';
}

VendingMachine::~VendingMachine()
{
    std::cout << "[VendingMachine] уничтожается: " << m_name << '\n';
}

bool VendingMachine::RestockCoffee(int amount)
{
    return m_coffee.Load(amount);
}

bool VendingMachine::MakeCoffee()
{
    std::cout << "[VendingMachine] заказ: кофе\n";

    if (!m_working)
    {
        std::cout << "[VendingMachine] ОТКАЗ: автомат выключен\n";
        return false;
    }

    if (!m_coffee.Consume(COFFEE_PORTION))
    {
        std::cout << "[VendingMachine] ОТКАЗ: кофе приготовить нельзя\n";
        return false;
    }

    std::cout << "[VendingMachine] кофе приготовлен\n";
    return true;
}

void VendingMachine::PrintState() const
{
    std::cout << "  Автомат '" << m_name << "': " << (m_working ? "работает" : "выключен") << '\n';
    m_coffee.Print();
}

void VendingMachine::Start()
{
    m_working = true;
}

void VendingMachine::Stop()
{
    m_working = false;
}

} // namespace cafe
