#pragma once

#include "vending_machine.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace cafe
{

// Оператор: сотрудник, обслуживающий автомат.
// Связь с автоматом - агрегация: оператор автоматом не владеет, поэтому хранит weak_ptr.
// Перед каждым действием проверяется, жив ли ещё автомат.
// Правило 16: пополнять запасы и менять состояние автомата можно только через оператора.
class MachineOperator
{
private:
    std::string m_name{};
    std::weak_ptr<VendingMachine> m_machine; // наблюдатель: жизнь автомата не продлевает

    // Возвращает автомат, если он ещё жив, иначе пустой указатель (и сообщает об отказе).
    std::shared_ptr<VendingMachine> GetMachine() const;

public:
    MachineOperator(std::string_view name, const std::shared_ptr<VendingMachine>& machine);
    MachineOperator(const MachineOperator&)            = delete;
    MachineOperator& operator=(const MachineOperator&) = delete;
    ~MachineOperator();

    bool AddCoffee(int amount);
    bool StartMachine();
    bool StopMachine();
    void PrintStatus() const;

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }
};

} // namespace cafe
