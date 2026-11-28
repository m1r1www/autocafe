#pragma once

#include "base_product.hpp"

namespace campus_vending
{

// final: отдельный тип товара, от которого не требуется дальнейшее наследование.
class InstantMealProduct final : public BaseProduct
{
private:
    int m_cookingMinutes{ 0 };

public:
    InstantMealProduct(std::string_view name, int price, int stock, int cookingMinutes,
                       std::shared_ptr<ProductDetails> details = {});
    InstantMealProduct(const InstantMealProduct& other);
    InstantMealProduct(InstantMealProduct&& other) noexcept;
    InstantMealProduct& operator=(const InstantMealProduct& other);
    InstantMealProduct& operator=(InstantMealProduct&& other) noexcept;
    ~InstantMealProduct() override;

    [[nodiscard]] std::string_view GetCategory() const override;
    [[nodiscard]] std::string Describe() const override;
    [[nodiscard]] std::unique_ptr<BaseProduct> Clone() const override;
};

} // namespace campus_vending
