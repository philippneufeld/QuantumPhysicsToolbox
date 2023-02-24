// Philipp Neufeld, 2023

#ifndef QTP_SCOPEGUARD_H_
#define QTP_SCOPEGUARD_H_

#include <type_traits>
#include <utility>

namespace QPT {

template <typename Lambda>
class ScopeGuard {
 public:
  ScopeGuard(Lambda onScopeExit)
      : m_onScopeExit(onScopeExit), m_dismissed(false) {}
  ~ScopeGuard() {
    if (!m_dismissed) m_onScopeExit();
  }

  ScopeGuard(const ScopeGuard&) = delete;
  ScopeGuard& operator=(const ScopeGuard&) = delete;

  ScopeGuard(ScopeGuard&& other)
      : m_onScopeExit(std::move(other.m_onScopeExit)), m_dismissed(false) {
    other.Dismiss();
  }
  ScopeGuard& operator=(ScopeGuard&& other) = delete;

  void Dismiss() { m_dismissed = true; }

 private:
  bool m_dismissed;
  Lambda m_onScopeExit;
};

// Caution: When using this function ALWAYS save the return value into
// a variable. Otherwise, the scope of the ScopeGuard ends just after the
// function call because the ScopeGuard object is discarded!
// Proper usage:
// auto scopeGuard = CreateScopeGuard(cleanup_func);
template <typename Lambda>
[[nodiscard]] auto CreateScopeGuard(Lambda onScopeExit) {
  return ScopeGuard<Lambda>{onScopeExit};
}

}  // namespace QPT

#endif  // !QTP_SCOPEGUARD_H_
