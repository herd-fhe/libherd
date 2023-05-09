#ifndef LIBHERD_I_TRANSLATOR_HPP
#define LIBHERD_I_TRANSLATOR_HPP

#include <memory>

#include "herd/translator/common/tree/circuit_graph.hpp"


namespace herd::translator
{
    class ITranslator
    {
    public:
        virtual ~ITranslator() = default;

        virtual std::shared_ptr<CircuitGraph> translate(std::string_view source) = 0;
    };
}

#endif //LIBHERD_I_TRANSLATOR_HPP
