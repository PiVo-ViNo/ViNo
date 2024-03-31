#include "IVisitor.hpp"

#include "custom_errors.hpp"

namespace vino {

void IVisitor::error() const {
    throw SemanticError("Program error: calling deprecated visitor method\n");
}

} // namespace vino