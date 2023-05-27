#ifndef LIBHERD_MAPPER_HPP
#define LIBHERD_MAPPER_HPP

#include <stdexcept>

#include <common.pb.h>
#include <storage.pb.h>
#include <execution_plan.pb.h>
#include <execution.pb.h>

#include "herd/common/model/data_type.hpp"
#include "herd/common/model/executor/execution_plan.hpp"
#include "herd/common/model/job.hpp"
#include "herd/common/model/schema_type.hpp"
#include "herd/storage/data_frame.hpp"


namespace herd
{

	struct MappingError: public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	namespace mapper
	{
		[[nodiscard]] common::SchemaType to_model(proto::SchemaType type);
		[[nodiscard]] common::DataType to_model(proto::DataType data_type);
		[[nodiscard]] std::vector<storage::DataFrame::ColumnParameters> to_model(const google::protobuf::RepeatedPtrField<proto::ColumnDescriptor>& execution_plan_proto);
		[[nodiscard]] common::JobStatus to_model(proto::JobStatus status_proto);
		[[nodiscard]] common::Operation to_model(proto::Operation operation_proto);
		[[nodiscard]] common::Circuit to_model(const proto::Circuit& circuit_proto);
		[[nodiscard]] common::stage_t to_model(const proto::Stage& stage_proto);
		[[nodiscard]] common::ExecutionPlan to_model(const proto::ExecutionPlan& execution_plan_proto);

		[[nodiscard]] proto::SchemaType to_proto(common::SchemaType type);
		[[nodiscard]] proto::DataType to_proto(common::DataType data_type);
		[[nodiscard]] google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> to_proto(const std::vector<storage::DataFrame::ColumnParameters>& columns);
		[[nodiscard]] proto::Operation to_proto(common::Operation operation);
		[[nodiscard]] proto::Node to_proto(const common::node_t& node);
		[[nodiscard]] proto::Circuit to_proto(const common::Circuit& circuit);
		[[nodiscard]] proto::Stage to_proto(const common::stage_t& stage);
		[[nodiscard]] proto::ExecutionPlan to_proto(const common::ExecutionPlan& plan);
	}
}

#endif //LIBHERD_MAPPER_HPP
