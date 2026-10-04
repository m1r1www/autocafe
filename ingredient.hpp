#pragma once

#include <string>
#include <string_view>

namespace cafe
{

enum class IngredientStatus
{
    eOk,
    eInvalidAmount,
    eOverCapacity,
    eExpired,
    eInsufficient
};

[[nodiscard]] std::string_view ToString(IngredientStatus status);

// Ингредиент: продукт с запасом и сроком годности.
// Следит за правилами: запас в пределах 0..вместимость, просроченный ингредиент использовать нельзя.
class Ingredient
{
private:
    std::string m_name{ "noname" };
    int m_capacity{ 0 };
    int m_amount{ 0 };
    int m_expiryDay{ 0 }; // последний день пригодности (включительно)

    IngredientStatus Fail(IngredientStatus status) const;

public:
    Ingredient();
    Ingredient(std::string_view name, int capacity, int amount, int expiryDay);
    Ingredient(const Ingredient&)            = delete;
    Ingredient& operator=(const Ingredient&) = delete;
    ~Ingredient();

    // Проверить параметры без создания объекта и без вывода сообщений.
    [[nodiscard]] static IngredientStatus Validate(int capacity, int amount);

    // Задать параметры повторно (для объектов, созданных конструктором по умолчанию).
    // При ошибке объект не изменяется.
    IngredientStatus Setup(std::string_view name, int capacity, int amount, int expiryDay);

    // Пополнить запас.
    IngredientStatus Load(int value);

    // Списать запас в указанный день.
    IngredientStatus Consume(int value, int today);

    [[nodiscard]] bool IsExpired(int today) const;
    [[nodiscard]] bool CanProvide(int value, int today) const;

    void Print(int today) const;

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] int GetAmount() const
    {
        return m_amount;
    }

    [[nodiscard]] int GetCapacity() const
    {
        return m_capacity;
    }

    [[nodiscard]] int GetExpiryDay() const
    {
        return m_expiryDay;
    }
};

} // namespace cafe
