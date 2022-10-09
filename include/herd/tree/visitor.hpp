#ifndef LIBHERD_VISITOR_HPP
#define LIBHERD_VISITOR_HPP

#include <functional>
#include "herd/tree/circuit_graph.hpp"


namespace herd::tree
{
	enum class VisitStatus
	{
		CONTINUE,
		ABORT
	};

	enum class VisitStrategy
	{
		BREADTH_FIRST_SEARCH,
		DEPTH_FIRST_SEARCH
	};

	template <typename Func>
	concept VisitFunction = requires(Func func, std::shared_ptr<AbstractNode>& node)
	{
		requires std::invocable<Func, std::shared_ptr<AbstractNode>&>;
		{ func(node) } -> std::same_as<VisitStatus>;
	};

	template <typename Func>
	concept ConstVisitFunction = requires(Func func, const std::shared_ptr<AbstractNode>& node)
	{
		requires std::invocable<Func, const std::shared_ptr<AbstractNode>&>;
		{ func(node) } -> std::same_as<VisitStatus>;
	};

	namespace detail
	{
		using visitor_function_t =  std::function<VisitStatus(std::shared_ptr<AbstractNode>&)>;
		using const_visitor_function_t =  std::function<VisitStatus(const std::shared_ptr<AbstractNode>&)>;

		void visit_bfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const visitor_function_t& visitor_func);
		void visit_dfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const visitor_function_t& visitor_func);

		void visit_bfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const const_visitor_function_t& visitor_func);
		void visit_dfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const const_visitor_function_t& visitor_func);
	}

	template <typename Func, VisitStrategy visit_strategy = VisitStrategy::BREADTH_FIRST_SEARCH>
	requires VisitFunction<Func>
	void visit(const CircuitGraph& graph, Func fun)
	{
		visit<Func, visit_strategy>(graph.input_nodes(), std::move(fun));
	}

	template <typename Func, VisitStrategy visit_strategy = VisitStrategy::BREADTH_FIRST_SEARCH>
	requires ConstVisitFunction<Func>
	void visit(const CircuitGraph& graph, Func fun)
	{
		visit<Func, visit_strategy>(graph.input_nodes(), std::move(fun));
	}

	template <typename Func, VisitStrategy visit_strategy = VisitStrategy::BREADTH_FIRST_SEARCH>
	requires VisitFunction<Func>
	void visit(const std::vector<std::shared_ptr<AbstractNode>>& node, Func fun)
	{
		if constexpr (visit_strategy == VisitStrategy::BREADTH_FIRST_SEARCH)
		{
			detail::visit_bfs(node, fun);
		}
		else
		{
			detail::visit_dfs(node, fun);
		}
	}

	template <typename Func, VisitStrategy visit_strategy = VisitStrategy::BREADTH_FIRST_SEARCH>
	requires ConstVisitFunction<Func>
	void visit(const std::vector<std::shared_ptr<AbstractNode>>& node, Func fun)
	{
		if constexpr (visit_strategy == VisitStrategy::BREADTH_FIRST_SEARCH)
		{
			detail::visit_bfs(node, fun);
		}
		else
		{
			detail::visit_dfs(node, fun);
		}
	}
}

#endif //LIBHERD_VISITOR_HPP
