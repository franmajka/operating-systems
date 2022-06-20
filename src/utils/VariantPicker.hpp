#pragma once

#include <algorithm>
#include <deque>
#include <string>
#include <random>

template <typename Variant>
class VariantPicker {
public:
  using Variants = std::deque<Variant>;

  VariantPicker(std::initializer_list<Variant> initList) : VariantPicker(Variants(initList)) {}
  VariantPicker(const Variants& variants) : m_variants(variants) {
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(m_variants), std::end(m_variants), rng);
  }

  Variant getOne() {
    Variant variant = m_variants.back();
    m_variants.pop_back();
    m_variants.push_front(variant);

    return variant;
  }

  std::vector<Variant> getMany(size_t quantity) {
    Variants ret;
    while (quantity--) {
      ret.push_back(getOne());
    }

    return ret;
  }
private:
  Variants m_variants;
};
