#pragma once

#include "base_product.hpp"

namespace campus_vending
{

class CoffeeProduct : public BaseProduct
{
private:
    int m_volumeMl{ 0 };
    bool m_hasMilk{ false };

public:
    CoffeeProduct();
    CoffeeProduct(std::string_view name, int price, int stock, int volumeMl, bool hasMilk,
                  std::shared_ptr<ProductDetails> details = {});
    CoffeeProduct(const CoffeeProduct& other);
    CoffeeProduct(const CoffeeProduct& other, ShallowCopyTag tag);
    CoffeeProduct(CoffeeProduct&& other) noexcept;
    CoffeeProduct& operator=(const CoffeeProduct& other);
    CoffeeProduct& operator=(CoffeeProduct&& other) noexcept;
    ~CoffeeProduct() override;

    [[nodiscard]] std::string_view GetCategory() const override;
    [[nodiscard]] std::string Describe() const override;
    [[nodiscard]] std::unique_ptr<BaseProduct> Clone() const override;
};

} // namespace campus_vending
