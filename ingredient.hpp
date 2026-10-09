#pragma once
#include <string>
#include <string_view>
namespace cafe {
class Ingredient {
private:
    std::string m_name{};
    int m_amount{0};
    int m_capacity{0};
public:
    Ingredient();
    Ingredient(std::string_view name, int amount, int capacity);
    ~Ingredient();
    bool Load(int amount);
    bool Consume(int amount);
    void Print() const;
    std::string_view GetName() const;
    int GetAmount() const;
};
}
