#ifndef LIBHERD_XLSCC_IR_TRANSPILER_HPP
#define LIBHERD_XLSCC_IR_TRANSPILER_HPP

#include "herd/common/model/circuit/circuit.hpp"
#include "herd/translator/xlscc/detail/ir_parser.hpp"


namespace herd::translator::xlscc::detail
{
	[[nodiscard]] common::Circuit transpile_ir_program(const ProgramDefinition& definition);
}

#endif //LIBHERD_XLSCC_IR_TRANSPILER_HPP
