#pragma once

#include <render/daBfg/detail/nodeNameId.h>
#include <render/daBfg/multiplexing.h>

#include <intermediateRepresentation.h>
#include <api/internalRegistry.h>
#include <resourceScheduling/resourceScheduler.h>
#include <nodes/nodeStateDeltas.h>
#include <nameResolver.h>


namespace dabfg
{

class NodeExecutor
{
public:
  NodeExecutor(ResourceScheduler &rs, intermediate::Graph &g, const intermediate::Mapping &m, InternalRegistry &reg,
    const NameResolver &res, ResourceProvider &rp) :
    resourceScheduler{rs}, graph{g}, mapping{m}, registry{reg}, nameResolver{res}, currentlyProvidedResources{rp}
  {}

  void execute(int prev_frame, int curr_frame, multiplexing::Extents multiplexing_extents,
    const ResourceScheduler::FrameEventsRef &events, const NodeStateDeltas &state_deltas);

  ExternalState externalState;

private:
  void gatherExternalResources(NodeNameId nameId, intermediate::MultiplexingIndex ir_multi_idx, multiplexing::Index multi_idx,
    IdIndexedMapping<intermediate::ResourceIndex, intermediate::Resource> &resources);

  void processEvents(ResourceScheduler::NodeEventsRef events) const;
  void applyState(const NodeStateDelta &state, int frame, int prev_frame) const;
  void applyBindings(const intermediate::BindingsMap &bindings, int frame, int prev_frame) const;

  void bindShaderVar(int bind_idx, const intermediate::Binding &binding, int frame, int prev_frame) const;
  template <typename ProjectedType, auto bindSetter>
  void bindBlob(int bind_idx, const intermediate::Binding &binding, int frame) const;

  ManagedTexView getManagedTexView(ResNameId res_name_id, int frame, intermediate::MultiplexingIndex multi_index) const;
  ManagedTexView getManagedTexView(intermediate::ResourceIndex res_idx, int frame) const;
  ManagedBufView getManagedBufView(ResNameId res_name_id, int frame, intermediate::MultiplexingIndex multi_index) const;
  ManagedBufView getManagedBufView(intermediate::ResourceIndex res_idx, int frame) const;
  BlobView getBlobView(ResNameId res_name_id, int frame, intermediate::MultiplexingIndex multi_index) const;
  BlobView getBlobView(intermediate::ResourceIndex res_idx, int frame) const;


private:
  ResourceScheduler &resourceScheduler;

  // TODO: make it const
  intermediate::Graph &graph;
  const intermediate::Mapping &mapping;

  InternalRegistry &registry;
  const NameResolver &nameResolver;
  ResourceProvider &currentlyProvidedResources;
};

} // namespace dabfg
