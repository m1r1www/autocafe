#include "vending_machine.hpp"

#include <algorithm>
#include <iostream>

namespace cafe
{

std::string_view ToString(MachineState state)
{
    switch (state)
    {
    case MachineState::eWorking:
        return "работает";
    case MachineState::eBroken:
        return "неисправен";
    case MachineState::eMaintenance:
        return "на обслуживании";
    }
    return "неизвестно";
}

std::string_view ToString(OrderResult result)
{
    switch (result)
    {
    case OrderResult::eOk:
        return "заказ выполнен";
    case OrderResult::eMachineNotWorking:
        return "заказ принимается только в рабочем состоянии";
    case OrderResult::eInvalidOrder:
        return "некорректный заказ";
    case OrderResult::eIngredientMissing:
        return "ингредиент отсутствует в автомате";
    case OrderResult::eIngredientExpired:
        return "ингредиент просрочен";
    case OrderResult::eIngredientShortage:
        return "недостаточно ингредиента";
    case OrderResult::eNoCups:
        return "нет стаканов, выдача без стакана недопустима";
    case OrderResult::eInsufficientPayment:
        return "внесённая сумма меньше цены, оплата не принята";
    case OrderResult::eNoChange:
        return "недостаточно размена";
    }
    return "неизвестно";
}

VendingMachine::VendingMachine(std::string_view name, int day)
    : m_name{ name }
    , m_day{ day }
{
    std::cout << "  [VendingMachine] создан: " << m_name << " (состояние: " << ToString(m_state) << ")\n";
}

VendingMachine::~VendingMachine()
{
    std::cout << "  [VendingMachine] уничтожается: " << m_name << '\n';
}

const Ingredient* VendingMachine::FindIngredient(std::string_view name) const
{
    const auto it = std::find_if(m_ingredients.begin(), m_ingredients.end(),
                                 [name](const Ingredient& item) { return item.GetName() == name; });
    return it == m_ingredients.end() ? nullptr : &(*it);
}

Ingredient* VendingMachine::FindMutable(std::string_view name)
{
    return const_cast<Ingredient*>(FindIngredient(name));
}

OrderResult VendingMachine::Reject(std::string_view drinkName, int price, OrderResult result, std::string_view detail)
{
    std::string reason(ToString(result));
    if (!detail.empty())
    {
        reason += ": " + std::string(detail);
    }

    std::cout << "  [VendingMachine] ОТКАЗ: " << reason << '\n';
    m_journal.push_back("день " + std::to_string(m_day) + ": " + std::string(drinkName) + ", " + std::to_string(price) +
                        " руб. — ОТМЕНЁН (" + reason + ")");
    return result;
}

bool VendingMachine::LoadIngredient(std::string_view name, int capacity, int amount, int expiryDay)
{
    if (m_state != MachineState::eMaintenance)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: загрузка ингредиентов возможна только в режиме обслуживания\n";
        return false;
    }

    if (FindIngredient(name) != nullptr)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: ингредиент '" << name << "' уже загружен\n";
        return false;
    }

    if (const auto status = Ingredient::Validate(capacity, amount); status != IngredientStatus::eOk)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: ингредиент '" << name << "': " << ToString(status) << '\n';
        return false;
    }

    // Объект создаётся на месте и принадлежит автомату (композиция).
    m_ingredients.emplace_back(name, capacity, amount, expiryDay);
    return true;
}

bool VendingMachine::Restock(std::string_view name, int amount)
{
    if (m_state != MachineState::eMaintenance)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: пополнение возможно только в режиме обслуживания\n";
        return false;
    }

    Ingredient* ingredient = FindMutable(name);
    if (ingredient == nullptr)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: ингредиент '" << name << "' не найден\n";
        return false;
    }

    return ingredient->Load(amount) == IngredientStatus::eOk;
}

bool VendingMachine::AddCups(int count)
{
    if (m_state != MachineState::eMaintenance || count <= 0)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: стаканы добавляются только в режиме обслуживания, count > 0\n";
        return false;
    }

    m_cups += count;
    return true;
}

bool VendingMachine::AddChange(int amount)
{
    if (m_state != MachineState::eMaintenance || amount <= 0)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: размен пополняется только в режиме обслуживания, amount > 0\n";
        return false;
    }

    m_changeBox += amount;
    return true;
}

bool VendingMachine::SwitchToMaintenance()
{
    if (m_state == MachineState::eMaintenance)
    {
        std::cout << "  [VendingMachine] автомат уже на обслуживании\n";
        return false;
    }

    m_state = MachineState::eMaintenance;
    std::cout << "  [VendingMachine] состояние -> " << ToString(m_state) << '\n';
    return true;
}

bool VendingMachine::SwitchToWorking()
{
    if (m_state == MachineState::eBroken)
    {
        std::cout << "  [VendingMachine] ОТКАЗ: из «неисправен» нельзя сразу в «работает», сначала обслуживание\n";
        return false;
    }

    if (m_state == MachineState::eWorking)
    {
        std::cout << "  [VendingMachine] автомат уже работает\n";
        return false;
    }

    m_state = MachineState::eWorking;
    std::cout << "  [VendingMachine] состояние -> " << ToString(m_state) << '\n';
    return true;
}

void VendingMachine::ReportFault()
{
    if (m_state != MachineState::eWorking)
    {
        return;
    }

    m_state = MachineState::eBroken;
    std::cout << "  [VendingMachine] обнаружен сбой! состояние -> " << ToString(m_state) << '\n';
}

void VendingMachine::AdvanceDay(int days)
{
    if (days > 0)
    {
        m_day += days;
    }
}

OrderResult VendingMachine::OrderDrink(std::string_view drinkName, const Recipe& recipe, int price, int paid)
{
    std::cout << "  [VendingMachine] заказ: " << drinkName << ", цена " << price << ", внесено " << paid << '\n';

    if (m_state != MachineState::eWorking)
    {
        return Reject(drinkName, price, OrderResult::eMachineNotWorking, ToString(m_state));
    }

    if (price <= 0 || paid < 0 || recipe.empty())
    {
        return Reject(drinkName, price, OrderResult::eInvalidOrder);
    }

    for (const auto& [ingredientName, amount] : recipe)
    {
        if (amount <= 0)
        {
            return Reject(drinkName, price, OrderResult::eInvalidOrder, ingredientName);
        }
    }

    // Сначала проверяем всё, и только потом списываем (при неуспехе списания нет).
    for (const auto& [ingredientName, amount] : recipe)
    {
        const Ingredient* ingredient = FindIngredient(ingredientName);
        if (ingredient == nullptr)
        {
            return Reject(drinkName, price, OrderResult::eIngredientMissing, ingredientName);
        }
        if (ingredient->IsExpired(m_day))
        {
            return Reject(drinkName, price, OrderResult::eIngredientExpired, ingredientName);
        }
        if (!ingredient->CanProvide(amount, m_day))
        {
            return Reject(drinkName, price, OrderResult::eIngredientShortage, ingredientName);
        }
    }

    if (m_cups <= 0)
    {
        return Reject(drinkName, price, OrderResult::eNoCups);
    }

    if (paid < price)
    {
        return Reject(drinkName, price, OrderResult::eInsufficientPayment);
    }

    const int change = paid - price;
    if (change > m_changeBox)
    {
        return Reject(drinkName, price, OrderResult::eNoChange,
                      "сумма " + std::to_string(paid) + " возвращена полностью");
    }

    std::cout << "  [VendingMachine] приготовление...\n";
    for (const auto& [ingredientName, amount] : recipe)
    {
        FindMutable(ingredientName)->Consume(amount, m_day);
    }

    --m_cups;
    m_changeBox -= change;
    m_revenue += price;
    m_journal.push_back("день " + std::to_string(m_day) + ": " + std::string(drinkName) + ", " + std::to_string(price) +
                        " руб. — ВЫДАН (сдача " + std::to_string(change) + ")");

    std::cout << "  [VendingMachine] напиток выдан, сдача " << change << '\n';
    return OrderResult::eOk;
}

void VendingMachine::PrintState() const
{
    std::cout << "  Автомат '" << m_name << "': состояние: " << ToString(m_state) << ", день " << m_day << ", стаканов "
              << m_cups << ", размен " << m_changeBox << ", выручка " << m_revenue << '\n';
    for (const auto& ingredient : m_ingredients)
    {
        ingredient.Print(m_day);
    }
}

void VendingMachine::PrintJournal() const
{
    std::cout << "  Журнал продаж (" << m_journal.size() << " зап.):\n";
    for (const auto& line : m_journal)
    {
        std::cout << "    " << line << '\n';
    }
}

} // namespace cafe
