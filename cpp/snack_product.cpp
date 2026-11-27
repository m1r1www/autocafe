#include "snack_product.hpp"

#include <iostream>
#include <utility>

namespace campus_vending
{

SnackProduct::SnackProduct()
    : BaseProduct{}
{
    std::cout << "[SnackProduct] конструктор по умолчанию\n";
}

SnackProduct::SnackProduct(std::string_view name, int price, int stock, int weightGrams,
                           std::shared_ptr<ProductDetails> details)
    : BaseProduct{ name, price, stock, std::move(details) }
    , m_weightGrams{ weightGrams > 0 ? weightGrams : 0 }
{
    std::cout << "[SnackProduct] создан: " << name << '\n';
}

SnackProduct::SnackProduct(const SnackProduct& other)
    : BaseProduct{ other }
    , m_weightGrams{ other.m_weightGrams }
{
    std::cout << "[SnackProduct] копирование\n";
}

SnackProduct::SnackProduct(SnackProduct&& other) noexcept
    : BaseProduct{ std::move(other) }
    , m_weightGrams{ other.m_weightGrams }
{
    other.m_weightGrams = 0;
    std::cout << "[SnackProduct] перемещение\n";
}

SnackProduct& SnackProduct::operator=(const SnackProduct& other)
{
    if (this != &other)
    {
        BaseProduct::operator=(other);
        m_weightGrams = other.m_weightGrams;
    }
    return *this;
}

SnackProduct& SnackProduct::operator=(SnackProduct&& other) noexcept
{
    if (this != &other)
    {
        BaseProduct::operator=(std::move(other));
        m_weightGrams = other.m_weightGrams;
        other.m_weightGrams = 0;
    }
    return *this;
}

SnackProduct::~SnackProduct()
{
    std::cout << "[SnackProduct] уничтожается\n";
}

std::string_view SnackProduct::GetCategory() const
{
    return "перекус";
}

std::string SnackProduct::Describe() const
{
    // Здесь не вызываем BaseProduct::Describe(), а собираем строку самостоятельно.
    return Header() + ", категория: перекус, вес: " + std::to_string(m_weightGrams) +
           " г, цена: " + std::to_string(GetPrice()) + " руб., остаток: " +
           std::to_string(GetStock());
}

std::unique_ptr<BaseProduct> SnackProduct::Clone() const
{
    return std::make_unique<SnackProduct>(*this);
}

} // namespace campus_vending
