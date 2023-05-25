#include "herd/backend/remote/detail/mapper.hpp"


namespace herd::mapper
{
	common::SchemaType to_model(proto::SchemaType type)
	{
		switch(type)
		{
			case proto::BINFHE:
				return common::SchemaType::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	common::DataType to_model(proto::DataType data_type)
	{
		using herd::common::DataType;
		switch(data_type)
		{
			case herd::proto::BIT:
				return DataType::BIT;
			case herd::proto::UINT8:
				return DataType::UINT8;
			case herd::proto::UINT16:
				return DataType::UINT16;
			case herd::proto::UINT32:
				return DataType::UINT32;
			case herd::proto::UINT64:
				return DataType::UINT64;
			case herd::proto::INT8:
				return DataType::INT8;
			case herd::proto::INT16:
				return DataType::INT16;
			case herd::proto::INT32:
				return DataType::INT32;
			case herd::proto::INT64:
				return DataType::INT64;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	common::JobStatus to_model(proto::JobStatus job_status_proto)
	{
		switch(job_status_proto)
		{
			case proto::WAITING_FOR_EXECUTION:
				return common::JobStatus::WAITING_FOR_EXECUTION;
			case proto::PENDING:
				return common::JobStatus::PENDING;
			case proto::COMPLETED:
				return common::JobStatus::COMPLETED;
			case proto::FAILED:
				return common::JobStatus::FAILED;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	std::vector<storage::DataFrame::ColumnParameters> to_model(const google::protobuf::RepeatedPtrField<proto::ColumnDescriptor>& columns)
	{
		assert(std::numeric_limits<uint8_t>::max() >= columns.size() && "Columns limit");

		std::vector<storage::DataFrame::ColumnParameters> column_parameters;
		column_parameters.reserve(static_cast<std::size_t>(columns.size()));

		std::ranges::transform(columns, std::back_inserter(column_parameters),
			[](const auto& column)
			{
				return storage::DataFrame::ColumnParameters{ column.name(), to_model(column.type()) };
			}
        );

		return column_parameters;
	}

	common::Operation to_model(proto::Operation operation_proto)
	{
		switch(operation_proto)
		{
			case proto::AND:
				return common::Operation::AND;
			case proto::OR:
				return common::Operation::OR;
			case proto::NOT:
				return common::Operation::NOT;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	common::node_t to_model(const proto::Node& node_proto)
	{
		if(node_proto.has_input())
		{
			const auto& input_node_proto = node_proto.input();
			return common::InputNode{
					input_node_proto.tuple_index(),
					input_node_proto.bit_index()
			};
		}
		else if(node_proto.has_output())
		{
			const auto& output_node_proto = node_proto.output();
			return common::OutputNode{
					output_node_proto.tuple_index(),
					output_node_proto.bit_index()
			};
		}
		else if(node_proto.has_constant())
		{
			const auto& constant_node_proto = node_proto.constant();
			return common::ConstantNode{
					constant_node_proto.value()
			};
		}
		else if(node_proto.has_operation())
		{
			const auto& operation_node_proto = node_proto.operation();
			return common::OperationNode{
					to_model(operation_node_proto.type())
			};
		}
		else
		{
			throw MappingError("Proto schema, model mismatch");
		}
	}

	common::Circuit to_model(const proto::Circuit& circuit_proto)
	{
		common::Circuit circuit{};

		circuit.input.insert(std::end(circuit.input), std::begin(circuit_proto.input()), std::end(circuit_proto.input()));
		circuit.output.insert(std::end(circuit.output), std::begin(circuit_proto.output()), std::end(circuit_proto.output()));

		auto& graph = circuit.circuit_graph;

		for(const auto& node: circuit_proto.nodes())
		{
			graph.insert(to_model(node));
		}

		for(const auto& edge: circuit_proto.edges())
		{
			const auto start = edge.start();
			const auto end = edge.end();

			const auto begin = std::begin(graph);

			graph.add_edge(begin + start, begin + end);
		}

		return circuit;
	}

	common::stage_t to_model(const proto::Stage& stage_proto)
	{
		if(stage_proto.has_input())
		{
			const auto& input_stage_proto = stage_proto.input();

			common::InputStage input_stage;
			input_stage.data_frame_uuid = common::UUID(input_stage_proto.data_frame_uuid());
			return input_stage;
		}
		else if(stage_proto.has_output())
		{
			const auto& output_stage_proto = stage_proto.output();

			common::OutputStage output_stage;
			if(output_stage_proto.has_name())
			{
				output_stage.name = output_stage_proto.name();
			}

			return output_stage;
		}
		else if(stage_proto.has_mapper())
		{
			const auto& mapper_stage_proto = stage_proto.mapper();

			common::MapperStage mapper_stage;
			mapper_stage.circuit = to_model(mapper_stage_proto.circuit());
			return mapper_stage;
		}

		throw MappingError("Proto schema, model mismatch");
	}

	common::ExecutionPlan to_model(const proto::ExecutionPlan& execution_plan_proto)
	{
		common::ExecutionPlan execution_plan{};

		execution_plan.schema_type = to_model(execution_plan_proto.schema_type());

		const auto execution_graph_proto = execution_plan_proto.execution_graph();
		auto& execution_graph = execution_plan.execution_graph;
		for(const auto& stage_proto: execution_graph_proto.stages())
		{
			execution_graph.insert(to_model(stage_proto));
		}

		for(const auto& edge: execution_graph_proto.edges())
		{
			const auto start = edge.start();
			const auto end = edge.end();

			const auto begin_iter = std::begin(execution_graph);

			execution_graph.add_edge(begin_iter+start, begin_iter+end);
		}

		return execution_plan;
	}

	proto::SchemaType to_proto(common::SchemaType type)
	{
		switch(type)
		{
			case common::SchemaType::BINFHE:
				return proto::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	proto::DataType to_proto(common::DataType data_type)
	{
		using herd::common::DataType;
		switch(data_type)
		{
			case DataType::BIT:
				return herd::proto::BIT;
			case DataType::UINT8:
				return herd::proto::UINT8;
			case DataType::UINT16:
				return herd::proto::UINT16;
			case DataType::UINT32:
				return herd::proto::UINT32;
			case DataType::UINT64:
				return herd::proto::UINT64;
			case DataType::INT8:
				return herd::proto::INT8;
			case DataType::INT16:
				return herd::proto::INT16;
			case DataType::INT32:
				return herd::proto::INT32;
			case DataType::INT64:
				return herd::proto::INT64;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> to_proto(const std::vector<storage::DataFrame::ColumnParameters>& columns)
	{
		assert(std::numeric_limits<uint8_t>::max() >= columns.size() && "Columns limit");

		google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> proto_columns;
		proto_columns.Reserve(static_cast<int>(columns.size()));

		for(const auto& column: columns)
		{
			const auto proto_column = proto_columns.Add();
			proto_column->set_name(column.name);
			proto_column->set_type(to_proto(column.type));
		}

		return proto_columns;
	}

	proto::Operation to_proto(common::Operation operation)
	{
		switch(operation)
		{
			using enum common::Operation;
			case AND:
				return proto::Operation::AND;
			case OR:
				return proto::Operation::OR;
			case NOT:
				return proto::Operation::NOT;
			case MUX:
			case NAND:
			case XOR:
				throw MappingError("Not supported");
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	proto::Node to_proto(const common::node_t& node)
	{
		proto::Node node_proto;

		if(std::holds_alternative<common::InputNode>(node))
		{
			auto input_proto = node_proto.mutable_input();
			const auto& input = std::get<common::InputNode>(node);

			input_proto->set_bit_index(input.bit_index);
			input_proto->set_tuple_index(input.tuple_index);
		}
		else if(std::holds_alternative<common::OutputNode>(node))
		{
			auto output_proto = node_proto.mutable_output();
			const auto& output = std::get<common::OutputNode>(node);

			output_proto->set_bit_index(output.bit_index);
			output_proto->set_tuple_index(output.tuple_index);
		}
		else if(std::holds_alternative<common::ConstantNode>(node))
		{
			auto constant_proto = node_proto.mutable_constant();
			const auto& constant = std::get<common::ConstantNode>(node);

			constant_proto->set_value(constant.value);
		}
		else if(std::holds_alternative<common::OperationNode>(node))
		{
			auto operation_proto = node_proto.mutable_operation();
			const auto& operation = std::get<common::OperationNode>(node);

			operation_proto->set_type(to_proto(operation.type));
		}
		else
		{
			throw MappingError("Proto schema, model mismatch");
		}
		return node_proto;
	}

	proto::Circuit to_proto(const common::Circuit& circuit)
	{
		proto::Circuit circuit_proto{};

		const auto input_proto = circuit_proto.mutable_input();
		const auto output_proto = circuit_proto.mutable_input();
		input_proto->Add(std::begin(circuit.input), std::end(circuit.input));
		output_proto->Add(std::begin(circuit.output), std::end(circuit.output));

		const auto operations_proto = circuit_proto.mutable_nodes();
		for(const auto& operation: circuit.circuit_graph)
		{
			operations_proto->Add(to_proto(operation.value()));
		}

		const auto edges_proto = circuit_proto.mutable_edges();
		for(const auto& [start, end]: circuit.circuit_graph.edges())
		{
			const auto edge_proto = edges_proto->Add();
			edge_proto->set_start(static_cast<unsigned int>(start));
			edge_proto->set_end(static_cast<unsigned int>(end));
		}

		return circuit_proto;
	}

	proto::Stage to_proto(const common::stage_t& stage)
	{
		proto::Stage stage_proto{};

		if(std::holds_alternative<common::InputStage>(stage))
		{
			const auto& input_stage = std::get<common::InputStage>(stage);
			stage_proto.mutable_input()->set_data_frame_uuid(input_stage.data_frame_uuid.as_string());
		}
		else if(std::holds_alternative<common::OutputStage>(stage))
		{
			const auto& output_stage = std::get<common::OutputStage>(stage);
			if(output_stage.name.has_value())
			{
				stage_proto.mutable_output()->set_name(output_stage.name.value());
			}
		}
		else if(std::holds_alternative<common::MapperStage>(stage))
		{
			const auto& mapper_stage = std::get<common::MapperStage>(stage);
			const auto circuit_proto = stage_proto.mutable_mapper()->mutable_circuit();
			circuit_proto->CopyFrom(to_proto(mapper_stage.circuit));
		}
		else
		{
			throw MappingError("Proto schema, model mismatch");
		}

		return stage_proto;
	}

	proto::ExecutionPlan to_proto(const common::ExecutionPlan& plan)
	{
		proto::ExecutionPlan proto_plan;
		proto_plan.set_schema_type(to_proto(plan.schema_type));

		const auto execution_graph_proto = proto_plan.mutable_execution_graph();
		const auto stages_proto = execution_graph_proto->mutable_stages();

		for(const auto& stage: plan.execution_graph)
		{
			const auto& stage_details = stage.value();;
			stages_proto->Add(to_proto(stage_details));
		}

		const auto edges_proto = execution_graph_proto->mutable_edges();
		for(const auto& [start, end]: plan.execution_graph.edges())
		{
			const auto edge_proto = edges_proto->Add();
			edge_proto->set_start(static_cast<unsigned int>(start));
			edge_proto->set_end(static_cast<unsigned int>(end));
		}

		return proto_plan;
	}
}