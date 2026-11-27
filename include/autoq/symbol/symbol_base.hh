/*****************************************************************************
 *  AUTOQ Tree Automata Library
 *
 *  Description:
 *    CRTP base for symbol types that share internal/leaf and qubit tagging.
 *    Used by Concrete and Symbolic to avoid duplicating is_internal()/is_leaf().
 *
 *****************************************************************************/

#ifndef _AUTOQ_SYMBOL_SYMBOL_BASE_HH_
#define _AUTOQ_SYMBOL_SYMBOL_BASE_HH_

namespace AUTOQ {
namespace Symbol {

struct WildcardAmplitude {};

/** CRTP base for internal symbols and leaf labels. */
template <typename Derived>
struct SymbolBase {
    bool is_internal() const { return kind_ == Kind::Internal; }
    bool is_leaf() const { return !is_internal(); }
    bool is_wildcard() const { return kind_ == Kind::Wildcard; }

protected:
    enum class Kind : unsigned char { Internal, Amplitude, Wildcard };
    Kind kind_{Kind::Amplitude};
    SymbolBase() = default;
    explicit SymbolBase(bool internal) : kind_(internal ? Kind::Internal : Kind::Amplitude) {}
    explicit SymbolBase(WildcardAmplitude) : kind_(Kind::Wildcard) {}
};

}  // namespace Symbol
}  // namespace AUTOQ

#endif
