#pragma once

#include <memory>
#include <string>
#include <string_view>

namespace campus_vending
{

class VendingMachine;

// Общие сведения о товаре. Несколько товаров могут ссылаться на один объект деталей.
struct ProductDetails
{
    std::string description{};
};

struct ShallowCopyTag {};

// Базовый товар университетского вендинга.
class BaseProduct : public std::enable_shared_from_this<BaseProduct>
{
private:
    std::string m_name{ "Без названия" };
    int m_price{ 0 };
    int m_stock{ 0 };
    int m_inventoryNumber{ 0 };
    std::shared_ptr<ProductDetails> m_details{};

    static int s_createdCount;
    static int NextInventoryNumber();

protected:
    // Общий заголовок нужен наследникам для построения собственного описания.
    [[nodiscard]] std::string Header() const;

public:
    BaseProduct();
    BaseProduct(std::string_view name, int price, int stock,
                std::shared_ptr<ProductDetails> details = {});
    BaseProduct(const BaseProduct& other);            // глубокое копирование деталей
    BaseProduct(const BaseProduct& other, ShallowCopyTag); // поверхностное копирование shared_ptr
    BaseProduct(BaseProduct&& other) noexcept;
    BaseProduct& operator=(const BaseProduct& other); // проверка самоприсваивания через this
    BaseProduct& operator=(BaseProduct&& other) noexcept;
    virtual ~BaseProduct();

    [[nodiscard]] virtual std::string_view GetCategory() const = 0;
    [[nodiscard]] virtual std::string Describe() const;
    [[nodiscard]] virtual std::unique_ptr<BaseProduct> Clone() const = 0;

    bool SellOne();
    bool Restock(int amount);
    BaseProduct& SetPrice(int price); // возвращает *this для цепочки вызовов
    void SetDetailsDescription(std::string_view description);
    void RegisterIn(VendingMachine& machine); // объект должен уже принадлежать shared_ptr

    [[nodiscard]] std::string_view GetName() const { return m_name; }
    [[nodiscard]] int GetPrice() const { return m_price; }
    [[nodiscard]] int GetStock() const { return m_stock; }
    [[nodiscard]] int GetInventoryNumber() const { return m_inventoryNumber; }
    [[nodiscard]] std::shared_ptr<const ProductDetails> GetSharedDetails() const { return m_details; }

    [[nodiscard]] static int GetCreatedCount();
};

} // namespace campus_vending
