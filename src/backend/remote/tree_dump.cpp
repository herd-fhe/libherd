#include "herd/backend/remote/tree_dump.hpp"

#include "herd/tree/visitor.hpp"
#include "herd/tree/node/operation_node.hpp"


namespace herd
{
	proto::CircuitGraph dump_tree(const herd::tree::CircuitGraph &graph)
	{
		proto::CircuitGraph out;

		auto input_nodes = out.mutable_input_nodes();
		auto output_nodes = out.mutable_output_nodes();
		auto operation_nodes = out.mutable_operation_nodes();

		std::unordered_map<std::shared_ptr<tree::AbstractNode>, uint32_t> id_map;
		uint32_t next_id = 0;

		for(size_t i = 0; i < graph.input_nodes().size(); ++i)
		{
			const auto& node = graph.input_nodes()[i];

			const uint32_t global_id = next_id++;
			id_map.try_emplace(node, global_id);

			auto new_node = input_nodes->Add();

			new_node->set_input_offset(static_cast<uint32_t>(i));
			new_node->mutable_meta()->set_id(global_id);

			for(const auto& child: node->children())
			{
				const uint32_t child_id = next_id++;
				id_map.try_emplace(child.lock(), child_id);

				auto child_meta = new_node->mutable_children()->Add();
				child_meta->set_id(child_id);
			}
		}


		for(size_t i = 0; i < graph.output_nodes().size(); ++i)
		{
			const auto& node = graph.input_nodes()[i];

			const uint32_t global_id = next_id++;

			id_map.try_emplace(node, global_id);

			auto new_node = output_nodes->Add();

			new_node->set_output_offset(static_cast<uint32_t>(i));
			new_node->mutable_meta()->set_id(global_id);
		}

		const auto assign_ids = [&next_id, &id_map] (std::shared_ptr<tree::AbstractNode>& node)
		{
			if(node->children().empty() || node->parents().empty())
			{
				return tree::VisitStatus::CONTINUE;
			}

			uint32_t id;

			if(!id_map.contains(node))
			{
				id = next_id++;
				id_map.try_emplace(node, id);
			}

			return tree::VisitStatus::CONTINUE;
		};

		const auto assign_children = [&operation_nodes, &id_map] (std::shared_ptr<tree::AbstractNode>& node)
		{
			if(node->children().empty() || node->parents().empty())
			{
				return tree::VisitStatus::CONTINUE;
			}

			const auto operation_node = std::dynamic_pointer_cast<tree::OperationNode>(node);
			assert(operation_node);

			const uint32_t own_id = id_map[node];

			auto added = operation_nodes->Add();
			added->set_operation_code(static_cast<uint32_t>(operation_node->operation_type()));
			added->mutable_meta()->set_id(own_id);
			auto dumped_children = added->mutable_children();

			for (const auto& child: node->children())
			{
				auto new_child = dumped_children->Add();
				new_child->set_id(id_map[child.lock()]);
			}

			return tree::VisitStatus::CONTINUE;
		};

		tree::visit(graph, assign_ids);
		tree::visit(graph, assign_children);

		return out;
	}
}