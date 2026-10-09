#pragma once

#include <string>
#include <string_view>

namespace cafe
{

// Ингредиент: хранит название, текущий запас и вместимость.
// Следит за правилом: запас не отрицателен и не превышает вместимость.
class Ingredient
{
private:
    std::string m_name{};
    int m_amount{ 0 };
    int m_capacity{ 0 };

public:
    Ingredient();
    Ingredient(std::string_view name, int amount, int capacity);
    Ingredient(const Ingredient&)            = delete;
    Ingredient& operator=(const Ingredient&) = delete;
    ~Ingredient();

    // Задать параметры повторно (для объектов, созданных конструктором по умолчанию).
    // Некорректный запас приводится к границам 0..вместимость, возвращается false.
    bool Setup(std::string_view name, int amount, int capacity);

    bool Load(int amount);    // пополнить запас
    bool Consume(int amount); // списать часть запаса
    void Print() const;

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] int GetAmount() const
    {
        return m_amount;
    }
};

} // namespace cafe
