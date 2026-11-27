#pragma once

#include "base_product.hpp"

namespace campus_vending
{

class SnackProduct : public BaseProduct
{
private:
    int m_weightGrams{ 0 };

public:
    SnackProduct();
    SnackProduct(std::string_view name, int price, int stock, int weightGrams,
                 std::shared_ptr<ProductDetails> details = {});
    SnackProduct(const SnackProduct& other);
    SnackProduct(SnackProduct&& other) noexcept;
    SnackProduct& operator=(const SnackProduct& other);
    SnackProduct& operator=(SnackProduct&& other) noexcept;
    ~SnackProduct() override;

    [[nodiscard]] std::string_view GetCategory() const override;
    [[nodiscard]] std::string Describe() const override;
    [[nodiscard]] std::unique_ptr<BaseProduct> Clone() const override;
};

} // namespace campus_vending
