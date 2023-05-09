#include "herd/translator/common/tree/visitor.hpp"

#include <deque>
#include <unordered_set>


namespace herd::translator::detail
{
	void visit_bfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const visitor_function_t& visitor_func)
	{
		std::deque<std::shared_ptr<AbstractNode>> node_queue;
		std::unordered_set<std::shared_ptr<AbstractNode>> visited;

		for(const auto& node: nodes)
		{
			node_queue.push_back(node);
		}

		while(!node_queue.empty())
		{
			auto node = std::move(node_queue.front());
			node_queue.pop_front();

			if(visited.contains(node))
			{
				continue;
			}

			visited.insert(node);

			auto visit_status = visitor_func(node);
			if(visit_status == VisitStatus::CONTINUE)
			{
				for (const auto& child: node->children())
				{
					node_queue.push_back(child.lock());
				}
			}
		}
	}

	void visit_dfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const visitor_function_t& visitor_func)
	{
		std::deque<std::shared_ptr<AbstractNode>> node_queue;
		std::unordered_set<std::shared_ptr<AbstractNode>> visited;

		for(const auto& node: nodes)
		{
			node_queue.push_back(node);
		}

		while(!node_queue.empty())
		{
			auto node = std::move(node_queue.back());
			node_queue.pop_back();

			if(visited.contains(node))
			{
				continue;
			}

			visited.insert(node);

			const auto visit_status = visitor_func(node);
			if (visit_status == VisitStatus::CONTINUE)
			{
				for (const auto& child: node->children())
				{
					node_queue.push_back(child.lock());
				}
			}
		}
	}

	void visit_bfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const const_visitor_function_t& visitor_func)
	{
		std::deque<std::shared_ptr<AbstractNode>> node_queue;
		std::unordered_set<std::shared_ptr<AbstractNode>> visited;

		for(const auto& node: nodes)
		{
			node_queue.push_back(node);
		}

		while(!node_queue.empty())
		{
			const auto node = std::move(node_queue.front());
			node_queue.pop_front();

			if(visited.contains(node))
			{
				continue;
			}

			visited.insert(node);

			const auto visit_status = visitor_func(node);
			if(visit_status == VisitStatus::CONTINUE)
			{
				for (const auto& child: node->children())
				{
					node_queue.push_back(child.lock());
				}
			}
		}
	}

	void visit_dfs(const std::vector<std::shared_ptr<AbstractNode>>& nodes, const const_visitor_function_t& visitor_func)
	{
		std::deque<std::shared_ptr<AbstractNode>> node_queue;
		std::unordered_set<std::shared_ptr<AbstractNode>> visited;

		for(const auto& node: nodes)
		{
			node_queue.push_back(node);
		}

		while(!node_queue.empty())
		{
			const auto node = std::move(node_queue.back());
			node_queue.pop_back();

			if(visited.contains(node))
			{
				continue;
			}

			visited.insert(node);

			const auto visit_status = visitor_func(node);
			if (visit_status == VisitStatus::CONTINUE)
			{
				for (const auto& child: node->children())
				{
					node_queue.push_back(child.lock());
				}
			}
		}
	}
}