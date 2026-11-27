#include "coffee_product.hpp"

#include <iostream>
#include <utility>

namespace campus_vending
{

CoffeeProduct::CoffeeProduct()
    : BaseProduct{}
{
    std::cout << "[CoffeeProduct] конструктор по умолчанию\n";
}

CoffeeProduct::CoffeeProduct(std::string_view name, int price, int stock, int volumeMl, bool hasMilk,
                             std::shared_ptr<ProductDetails> details)
    : BaseProduct{ name, price, stock, std::move(details) }
    , m_volumeMl{ volumeMl > 0 ? volumeMl : 0 }
    , m_hasMilk{ hasMilk }
{
    std::cout << "[CoffeeProduct] создан: " << name << '\n';
}

CoffeeProduct::CoffeeProduct(const CoffeeProduct& other)
    : BaseProduct{ other }
    , m_volumeMl{ other.m_volumeMl }
    , m_hasMilk{ other.m_hasMilk }
{
    std::cout << "[CoffeeProduct] копирование\n";
}

CoffeeProduct::CoffeeProduct(const CoffeeProduct& other, ShallowCopyTag tag)
    : BaseProduct{ other, tag }
    , m_volumeMl{ other.m_volumeMl }
    , m_hasMilk{ other.m_hasMilk }
{
    std::cout << "[CoffeeProduct] поверхностное копирование\n";
}

CoffeeProduct::CoffeeProduct(CoffeeProduct&& other) noexcept
    : BaseProduct{ std::move(other) }
    , m_volumeMl{ other.m_volumeMl }
    , m_hasMilk{ other.m_hasMilk }
{
    other.m_volumeMl = 0;
    std::cout << "[CoffeeProduct] перемещение\n";
}

CoffeeProduct& CoffeeProduct::operator=(const CoffeeProduct& other)
{
    if (this != &other)
    {
        BaseProduct::operator=(other);
        m_volumeMl = other.m_volumeMl;
        m_hasMilk = other.m_hasMilk;
    }
    return *this;
}

CoffeeProduct& CoffeeProduct::operator=(CoffeeProduct&& other) noexcept
{
    if (this != &other)
    {
        BaseProduct::operator=(std::move(other));
        m_volumeMl = other.m_volumeMl;
        m_hasMilk = other.m_hasMilk;
        other.m_volumeMl = 0;
    }
    return *this;
}

CoffeeProduct::~CoffeeProduct()
{
    std::cout << "[CoffeeProduct] уничтожается\n";
}

std::string_view CoffeeProduct::GetCategory() const
{
    return "напиток";
}

std::string CoffeeProduct::Describe() const
{
    // Здесь намеренно дополняем готовое описание базового класса.
    return BaseProduct::Describe() + ", объём: " + std::to_string(m_volumeMl) + " мл" +
           (m_hasMilk ? ", с молоком" : ", без молока");
}

std::unique_ptr<BaseProduct> CoffeeProduct::Clone() const
{
    return std::make_unique<CoffeeProduct>(*this);
}

} // namespace campus_vending
