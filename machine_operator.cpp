#include "machine_operator.hpp"

#include <iostream>

namespace cafe
{

MachineOperator::MachineOperator(std::string_view name, const std::shared_ptr<VendingMachine>& machine)
    : m_name{ name }
    , m_machine{ machine }
{
    std::cout << "[MachineOperator] создан: " << m_name << ", закреплён за '"
              << (machine ? machine->GetName() : std::string_view("(нет автомата)")) << "'\n";
}

MachineOperator::~MachineOperator()
{
    std::cout << "[MachineOperator] уничтожается: " << m_name << " (автомат не удаляется)\n";
}

std::shared_ptr<VendingMachine> MachineOperator::GetMachine() const
{
    std::shared_ptr<VendingMachine> machine = m_machine.lock();
    if (!machine)
    {
        std::cout << "[MachineOperator] ОТКАЗ: автомат уже уничтожен\n";
    }
    return machine;
}

bool MachineOperator::AddCoffee(int amount)
{
    std::shared_ptr<VendingMachine> machine = GetMachine();
    return machine && machine->RestockCoffee(amount);
}

bool MachineOperator::StartMachine()
{
    std::shared_ptr<VendingMachine> machine = GetMachine();
    if (!machine)
    {
        return false;
    }

    machine->Start();
    std::cout << "[MachineOperator] автомат запущен\n";
    return true;
}

bool MachineOperator::StopMachine()
{
    std::shared_ptr<VendingMachine> machine = GetMachine();
    if (!machine)
    {
        return false;
    }

    machine->Stop();
    std::cout << "[MachineOperator] автомат остановлен\n";
    return true;
}

void MachineOperator::PrintStatus() const
{
    std::shared_ptr<VendingMachine> machine = m_machine.lock();
    if (machine)
    {
        std::cout << "  Оператор " << m_name << ": автомат '" << machine->GetName()
                  << "' жив (lock() вернул указатель)\n";
    }
    else
    {
        std::cout << "  Оператор " << m_name << ": автомат уничтожен (lock() вернул пустой указатель)\n";
    }
}

} // namespace cafe
