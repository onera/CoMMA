#ifndef COMMA_PROJECT_TEMPLATE_HELPERS_H
#define COMMA_PROJECT_TEMPLATE_HELPERS_H

template <bool condition, typename typeA, typename typeB>
void fill_value(typeA &A, typeB &B) {
  if
    constexpr(condition) { A = B; }
}

#endif
