#ifndef LIBHERD_XLSCC_IR_TRANSPILER_HPP
#define LIBHERD_XLSCC_IR_TRANSPILER_HPP

#include "herd/translator/common/tree/circuit.hpp"
#include "herd/translator/xlscc/detail/ir_parser.hpp"


namespace herd::translator::xlscc::detail
{
	[[nodiscard]] Circuit transpile_ir_program(const ProgramDefinition& definition);
}

#endif //LIBHERD_XLSCC_IR_TRANSPILER_HPP