#pragma once

#include "ingredient.hpp"

#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace campus_vending
{

class MachineOperator;
class BaseProduct;

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
// из «неисправен» нельзя сразу в «работает»; в штатном сценарии обслуживание выполняет оператор.
class VendingMachine
{
private:

    std::string m_name{};
    MachineState m_state{ MachineState::eMaintenance }; // новый автомат ждёт загрузки оператором
    std::deque<Ingredient> m_ingredients{};             // композиция; deque не перемещает элементы при росте
    std::vector<std::string> m_journal{};
    std::vector<std::shared_ptr<BaseProduct>> m_products{};
    int m_cups{ 0 };
    int m_changeBox{ 0 };
    int m_revenue{ 0 };
    int m_day{ 0 };

    [[nodiscard]] std::deque<Ingredient>::iterator FindMutable(std::string_view name);
    OrderResult Reject(std::string_view drinkName, int price, OrderResult result, std::string_view detail = {});

public:
    VendingMachine(std::string_view name, int day);
    ~VendingMachine();

    // Операции обслуживания доступны через публичный API и сами проверяют состояние автомата.
    // MachineOperator дополнительно проверяет, закреплён ли за ним этот автомат.
    bool LoadIngredient(std::string_view name, int capacity, int amount, int expiryDay);
    bool Restock(std::string_view name, int amount);
    bool AddCups(int count);
    bool AddChange(int amount);
    bool SwitchToMaintenance();
    bool SwitchToWorking();

    // Каталог готовых товаров: напитки, снеки и блюда быстрого приготовления.
    bool AddProduct(const std::shared_ptr<BaseProduct>& product);
    bool SellProduct(std::string_view name, int paid);
    void PrintProducts() const;

    // Оформить заказ напитка по рецепту: проверки, оплата, сдача, списание, журнал.
    OrderResult OrderDrink(std::string_view drinkName, const Recipe& recipe, int price, int paid);

    // Автомат сам переходит в «неисправен» при сбое.
    void ReportFault();

    void AdvanceDay(int days);

    [[nodiscard]] std::deque<Ingredient>::const_iterator FindIngredient(std::string_view name) const;

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

} // namespace campus_vending
