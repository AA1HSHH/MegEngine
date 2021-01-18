/**
 * \file src/opr/impl/loop/forward_sereg.h
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#pragma once

#include "megbrain/opr/loop.h"
#include "megbrain/serialization/sereg.h"

namespace mgb {
namespace serialization {
    template<>
    struct OprLoadDumpImpl<opr::Loop, 0> {
        static void dump(
                OprDumpContext &ctx, const cg::OperatorNodeBase &opr);
        static cg::OperatorNodeBase* load(
                OprLoadContext &ctx, const cg::VarNodeArray &inputs,
                const OperatorNodeConfig &config);
    };

    struct LoopSerializerReg {
        //! entry for registering serializers related to loop
        static void entry();
    };

    cg::OperatorNodeBase* opr_shallow_copy_loop(
            const OprShallowCopyContext &ctx,
            const cg::OperatorNodeBase &opr, const VarNodeArray &inputs,
            const OperatorNodeConfig &config);

} // namespace serialization
} // namespace mgb


// vim: ft=cpp syntax=cpp.doxygen foldmethod=marker foldmarker=f{{{,f}}}

