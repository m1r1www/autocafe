#include "instant_meal_product.hpp"

#include <iostream>
#include <utility>

namespace campus_vending
{

InstantMealProduct::InstantMealProduct(std::string_view name, int price, int stock, int cookingMinutes,
                                       std::shared_ptr<ProductDetails> details)
    : BaseProduct{ name, price, stock, std::move(details) }
    , m_cookingMinutes{ cookingMinutes > 0 ? cookingMinutes : 0 }
{
    std::cout << "[InstantMealProduct] создан: " << name << '\n';
}

InstantMealProduct::InstantMealProduct(const InstantMealProduct& other)
    : BaseProduct{ other }
    , m_cookingMinutes{ other.m_cookingMinutes }
{
    std::cout << "[InstantMealProduct] копирование\n";
}

InstantMealProduct::InstantMealProduct(InstantMealProduct&& other) noexcept
    : BaseProduct{ std::move(other) }
    , m_cookingMinutes{ other.m_cookingMinutes }
{
    other.m_cookingMinutes = 0;
    std::cout << "[InstantMealProduct] перемещение\n";
}

InstantMealProduct& InstantMealProduct::operator=(const InstantMealProduct& other)
{
    if (this != &other)
    {
        BaseProduct::operator=(other);
        m_cookingMinutes = other.m_cookingMinutes;
    }
    return *this;
}

InstantMealProduct& InstantMealProduct::operator=(InstantMealProduct&& other) noexcept
{
    if (this != &other)
    {
        BaseProduct::operator=(std::move(other));
        m_cookingMinutes = other.m_cookingMinutes;
        other.m_cookingMinutes = 0;
    }
    return *this;
}

InstantMealProduct::~InstantMealProduct()
{
    std::cout << "[InstantMealProduct] уничтожается\n";
}

std::string_view InstantMealProduct::GetCategory() const
{
    return "быстрое блюдо";
}

std::string InstantMealProduct::Describe() const
{
    return BaseProduct::Describe() + ", приготовление: " + std::to_string(m_cookingMinutes) + " мин.";
}

std::unique_ptr<BaseProduct> InstantMealProduct::Clone() const
{
    return std::make_unique<InstantMealProduct>(*this);
}

} // namespace campus_vending
