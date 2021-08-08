#ifndef FST_UTIL_OPERATORS_CONCATRANGE_H_
#define FST_UTIL_OPERATORS_CONCATRANGE_H_

#include <memory>

#include <fst/types.h>
#include <fst/fstlib.h>

namespace fst {
namespace internal {

// Makes it possible to exit from the current start state. Normally it suffices
// to mark the start state final, but if the start state is already final this
// will overwrite the current final weight and eliminate a path. Instead, we
// accomplish this by adding an epsilon-transition from the start state to a new
// final state.
template <class Arc>
void SetStartFinal(MutableFst<Arc> *fst) {
  static const auto *const kStartFinal = [] {
    auto *sf = new VectorFst<Arc>;
    const auto state = sf->AddState();
    sf->SetStart(state);
    sf->SetFinal(state);
    return sf;
  }();
  Union(fst, *kStartFinal);
}

}  // namespace internal

// This function is a generalization of FST closure and PCRE's curly brace
// quantifiers. It destructively computes the concatenative closure of an input
// FST as follows. If A transduces strings x to y with weight w, then
// ConcatRange(A, 0, 0) is equivalent to Closure(A, CLOSURE_STAR) which mutates
// A so that it transduces between empty strings with weight One, transduces
// strings x to y with weight w, transduces xx to yy with weight w \otimes w,
// strings xxx to yyy with weight w \otimes w \otimes w (and so on).
//
// When called with two non-zero positive integers as the trailing arguments,
// these act as upper and lower bounds, respectively, for the number of cycles
// through the original FST one is permitted to take in the modified FST. So,
// ConcatRange(A, 0, 1) is mutates A so it transduces between empty strings with
// weight One and // transduces strings x to y with weight w, similar to the ?
// quantifier in PCRE. And, ConcatRange(A, 2, 5) mutates A so that it behaves
// like the concatenation of between 2 and 5 A's.
//
// When the third argument is zero, it is interpreted to indicate an infinite
// upper bound. Thus, ConcatRange(A, 1, 0) is equivalent to Closure(A,
// CLOSURE_PLUS).
//
// The following provide equvialents to the PCRE operators:
//
//     /x*/        ConcatRange(x, 0, 0)
//     /x+/        ConcatRange(x, 1, 0)
//     /x?/        ConcatRange(x, 0, 1)
//     /x{N}/      ConcatRange(x, N, N)
//     /x{M,N}/    ConcatRange(x, M, N)
//     /x{N,}/     ConcatRange(x, N, 0)
//     /x{,N}/     ConcatRange(x, 0, N)
template <class Arc>
void ConcatRange(MutableFst<Arc> *fst, int32 lower = 0, int32 upper = 0) {
  if (fst->Start() == kNoStateId) return;
  const std::unique_ptr<const MutableFst<Arc>> copy(fst->Copy());
  if (upper == 0) {
    // Infinite upper bound.
    // The last element in the concatenation is star-closed; remaining
    // concatenations are copies of the input.
    Closure(fst, CLOSURE_STAR);
    for (; lower > 0; --lower) Concat(*copy, fst);
  } else if (lower == 0) {
    // Finite upper bound, lower bound includes zero.
    for (; upper > 1; --upper) {
      internal::SetStartFinal(fst);
      Concat(*copy, fst);
    }
    internal::SetStartFinal(fst);
  } else {
    // Finite upper bound, lower bound does not include zero.
    for (; upper > lower; --upper) {
      internal::SetStartFinal(fst);
      Concat(*copy, fst);
    }
    for (; lower > 1; --lower) Concat(*copy, fst);
  }
}

}  // namespace fst

#endif  // FST_UTIL_OPERATORS_CONCATRANGE_H_
