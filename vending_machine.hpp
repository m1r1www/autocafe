#pragma once

#include "ingredient.hpp"

#include <deque>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cafe
{

class MachineOperator;

enum class MachineState
{
    eWorking,
    eBroken,
    eMaintenance
};

enum class OrderResult
{
    eOk,
    eMachineNotWorking,
    eInvalidOrder,
    eIngredientMissing,
    eIngredientExpired,
    eIngredientShortage,
    eNoCups,
    eInsufficientPayment,
    eNoChange
};

[[nodiscard]] std::string_view ToString(MachineState state);
[[nodiscard]] std::string_view ToString(OrderResult result);

// Рецепт: пары «название ингредиента — количество».
using Recipe = std::vector<std::pair<std::string, int>>;

// Автомат: хранит ингредиенты (композиция), стаканы и размен, принимает заказы, ведёт журнал.
// Следит за правилами: заказ только в рабочем состоянии; нет ингредиента, стакана, оплаты или размена — отказ;
// из «неисправен» нельзя сразу в «работает»; запасы и состояние меняет только оператор.
class VendingMachine
{
private:
    friend class MachineOperator; // только оператор вправе менять состояние и пополнять запасы

    std::string m_name{};
    MachineState m_state{ MachineState::eMaintenance }; // новый автомат ждёт загрузки оператором
    std::deque<Ingredient> m_ingredients{};             // композиция; deque не перемещает элементы при росте
    std::vector<std::string> m_journal{};
    int m_cups{ 0 };
    int m_changeBox{ 0 };
    int m_revenue{ 0 };
    int m_day{ 0 };

    // Операции оператора.
    bool LoadIngredient(std::string_view name, int capacity, int amount, int expiryDay);
    bool Restock(std::string_view name, int amount);
    bool AddCups(int count);
    bool AddChange(int amount);
    bool SwitchToMaintenance();
    bool SwitchToWorking();

    [[nodiscard]] Ingredient* FindMutable(std::string_view name);
    OrderResult Reject(std::string_view drinkName, int price, OrderResult result, std::string_view detail = {});

public:
    VendingMachine(std::string_view name, int day);
    VendingMachine(const VendingMachine&)            = delete;
    VendingMachine& operator=(const VendingMachine&) = delete;
    ~VendingMachine();

    // Оформить заказ: проверки, оплата, сдача, приготовление, списание, запись в журнал.
    OrderResult OrderDrink(std::string_view drinkName, const Recipe& recipe, int price, int paid);

    // Автомат сам переходит в «неисправен» при сбое.
    void ReportFault();

    void AdvanceDay(int days);

    [[nodiscard]] const Ingredient* FindIngredient(std::string_view name) const;

    void PrintState() const;
    void PrintJournal() const;

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] MachineState GetState() const
    {
        return m_state;
    }

    [[nodiscard]] int GetDay() const
    {
        return m_day;
    }

    [[nodiscard]] int GetCups() const
    {
        return m_cups;
    }

    [[nodiscard]] int GetChangeBox() const
    {
        return m_changeBox;
    }

    [[nodiscard]] int GetRevenue() const
    {
        return m_revenue;
    }

    [[nodiscard]] const std::deque<Ingredient>& GetIngredients() const
    {
        return m_ingredients;
    }

    [[nodiscard]] const std::vector<std::string>& GetJournal() const
    {
        return m_journal;
    }
};

} // namespace cafe
