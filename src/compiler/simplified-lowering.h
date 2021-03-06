// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_SIMPLIFIED_LOWERING_H_
#define V8_COMPILER_SIMPLIFIED_LOWERING_H_

#include "src/base/flags.h"
#include "src/compiler/js-graph.h"
#include "src/compiler/machine-operator.h"
#include "src/compiler/node.h"
#include "src/compiler/simplified-operator.h"

namespace v8 {
namespace internal {

// Forward declarations.
class TypeCache;


namespace compiler {

// Forward declarations.
class RepresentationChanger;
class RepresentationSelector;
class SourcePositionTable;

class SimplifiedLowering final {
 public:
  enum Flag { kNoFlag = 0u, kTypeFeedbackEnabled = 1u << 0 };
  typedef base::Flags<Flag> Flags;
  SimplifiedLowering(JSGraph* jsgraph, Zone* zone,
                     SourcePositionTable* source_positions,
                     Flags flags = kNoFlag);
  ~SimplifiedLowering() {}

  void LowerAllNodes();

  void DoJSToNumberTruncatesToFloat64(Node* node,
                                      RepresentationSelector* selector);
  void DoJSToNumberTruncatesToWord32(Node* node,
                                     RepresentationSelector* selector);
  // TODO(turbofan): The representation can be removed once the result of the
  // representation analysis is stored in the node bounds.
  void DoLoadBuffer(Node* node, MachineRepresentation rep,
                    RepresentationChanger* changer);
  void DoStoreBuffer(Node* node);
  void DoShift(Node* node, Operator const* op, Type* rhs_type);

  Flags flags() const { return flags_; }

 private:
  JSGraph* const jsgraph_;
  Zone* const zone_;
  TypeCache const& type_cache_;
  SetOncePointer<Node> to_number_code_;
  SetOncePointer<Operator const> to_number_operator_;
  Flags flags_;

  // TODO(danno): SimplifiedLowering shouldn't know anything about the source
  // positions table, but must for now since there currently is no other way to
  // pass down source position information to nodes created during
  // lowering. Once this phase becomes a vanilla reducer, it should get source
  // position information via the SourcePositionWrapper like all other reducers.
  SourcePositionTable* source_positions_;

  Node* Float64Ceil(Node* const node);
  Node* Float64Floor(Node* const node);
  Node* Float64Round(Node* const node);
  Node* Float64Trunc(Node* const node);
  Node* Int32Div(Node* const node);
  Node* Int32Mod(Node* const node);
  Node* Uint32Div(Node* const node);
  Node* Uint32Mod(Node* const node);

  Node* ToNumberCode();
  Operator const* ToNumberOperator();

  friend class RepresentationSelector;

  Isolate* isolate() { return jsgraph_->isolate(); }
  Zone* zone() { return jsgraph_->zone(); }
  JSGraph* jsgraph() { return jsgraph_; }
  Graph* graph() { return jsgraph()->graph(); }
  CommonOperatorBuilder* common() { return jsgraph()->common(); }
  MachineOperatorBuilder* machine() { return jsgraph()->machine(); }
  SimplifiedOperatorBuilder* simplified() { return jsgraph()->simplified(); }
};

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_SIMPLIFIED_LOWERING_H_
