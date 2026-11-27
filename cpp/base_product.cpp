#include "base_product.hpp"

#include "vending_machine.hpp"

#include <iostream>
#include <utility>

namespace campus_vending
{

int BaseProduct::s_createdCount = 0;

int BaseProduct::NextInventoryNumber()
{
    ++s_createdCount;
    return s_createdCount;
}

BaseProduct::BaseProduct()
    : m_inventoryNumber{ NextInventoryNumber() }
    , m_details{ std::make_shared<ProductDetails>() }
{
    std::cout << "[BaseProduct] конструктор по умолчанию, товар №" << m_inventoryNumber << '\n';
}

BaseProduct::BaseProduct(std::string_view name, int price, int stock,
                         std::shared_ptr<ProductDetails> details)
    : m_name{ name }
    , m_price{ price }
    , m_stock{ stock }
    , m_inventoryNumber{ NextInventoryNumber() }
    , m_details{ details ? std::move(details) : std::make_shared<ProductDetails>() }
{
    if (m_price < 0) m_price = 0;
    if (m_stock < 0) m_stock = 0;
    std::cout << "[BaseProduct] создан: " << m_name << " №" << m_inventoryNumber << '\n';
}

BaseProduct::BaseProduct(const BaseProduct& other)
    : std::enable_shared_from_this<BaseProduct>{}
    , m_name{ other.m_name }
    , m_price{ other.m_price }
    , m_stock{ other.m_stock }
    , m_inventoryNumber{ NextInventoryNumber() }
    , m_details{ other.m_details ? std::make_shared<ProductDetails>(*other.m_details) : std::make_shared<ProductDetails>() }
{
    std::cout << "[BaseProduct] глубокая копия: " << m_name << " №" << m_inventoryNumber << '\n';
}

BaseProduct::BaseProduct(const BaseProduct& other, ShallowCopyTag)
    : std::enable_shared_from_this<BaseProduct>{}
    , m_name{ other.m_name }
    , m_price{ other.m_price }
    , m_stock{ other.m_stock }
    , m_inventoryNumber{ NextInventoryNumber() }
    , m_details{ other.m_details }
{
    std::cout << "[BaseProduct] поверхностная копия: " << m_name << " №" << m_inventoryNumber << '\n';
}

BaseProduct::BaseProduct(BaseProduct&& other) noexcept
    : m_name{ std::move(other.m_name) }
    , m_price{ other.m_price }
    , m_stock{ other.m_stock }
    , m_inventoryNumber{ NextInventoryNumber() }
    , m_details{ std::move(other.m_details) }
{
    other.m_price = 0;
    other.m_stock = 0;
    std::cout << "[BaseProduct] перемещение, новый объект №" << m_inventoryNumber << '\n';
}

BaseProduct& BaseProduct::operator=(const BaseProduct& other)
{
    if (this != &other)
    {
        m_name = other.m_name;
        m_price = other.m_price;
        m_stock = other.m_stock;
        m_details = other.m_details ? std::make_shared<ProductDetails>(*other.m_details) : std::make_shared<ProductDetails>();
    }
    return *this;
}

BaseProduct& BaseProduct::operator=(BaseProduct&& other) noexcept
{
    if (this != &other)
    {
        m_name = std::move(other.m_name);
        m_price = other.m_price;
        m_stock = other.m_stock;
        m_details = std::move(other.m_details);
        other.m_price = 0;
        other.m_stock = 0;
    }
    return *this;
}

BaseProduct::~BaseProduct()
{
    std::cout << "[BaseProduct] уничтожается: " << m_name << " №" << m_inventoryNumber << '\n';
}

std::string BaseProduct::Header() const
{
    return "Товар №" + std::to_string(m_inventoryNumber) + " «" + m_name + "»";
}

std::string BaseProduct::Describe() const
{
    return Header() + ", категория: " + std::string(GetCategory()) + ", цена: " +
           std::to_string(m_price) + " руб., остаток: " + std::to_string(m_stock) +
           ", описание: " + (m_details ? m_details->description : "нет");
}

bool BaseProduct::SellOne()
{
    if (m_stock <= 0) return false;
    --m_stock;
    return true;
}

bool BaseProduct::Restock(int amount)
{
    if (amount <= 0) return false;
    m_stock += amount;
    return true;
}

BaseProduct& BaseProduct::SetPrice(int price)
{
    if (price >= 0) m_price = price;
    return *this;
}

void BaseProduct::SetDetailsDescription(std::string_view description)
{
    if (m_details) m_details->description = std::string(description);
}

// Вызывать только для объекта, уже созданного под управлением shared_ptr.
void BaseProduct::RegisterIn(VendingMachine& machine)
{
    machine.AddProduct(shared_from_this());
}

int BaseProduct::GetCreatedCount()
{
    return s_createdCount;
}

} // namespace campus_vending
