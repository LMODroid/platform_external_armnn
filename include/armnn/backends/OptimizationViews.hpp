//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <armnn/backends/SubgraphView.hpp>

namespace armnn
{

class OptimizationViews
{
public:
    OptimizationViews() = default;
    OptimizationViews(const OptimizationViews&) = delete;
    OptimizationViews& operator=(const OptimizationViews&) = delete;
    OptimizationViews(OptimizationViews&&) = default;
    OptimizationViews& operator=(OptimizationViews&&) = default;

    struct SubstitutionPair
    {
        /// Subgraph of Layers from the original graph which should be replaced
        SubgraphView m_SubstitutableSubgraph;

        /// A subgraph of new layers which will replace layers in m_SubstitutableSubgraph
        SubgraphView m_ReplacementSubgraph;
    };

    using Subgraphs = std::vector<SubgraphView>;
    using Substitutions = std::vector<SubstitutionPair>;

    void AddSubstitution(SubstitutionPair&& substitution)
    {
        m_SuccesfulOptimizations.emplace_back(substitution);
    }

    void AddFailedSubgraph(SubgraphView&& subgraph)
    {
        m_FailedOptimizations.emplace_back(subgraph);
    }

    void AddUntouchedSubgraph(SubgraphView&& subgraph)
    {
        m_UntouchedSubgraphs.emplace_back(subgraph);
    }

    const Substitutions& GetSubstitutions() const { return m_SuccesfulOptimizations; }
    const Subgraphs& GetFailedSubgraphs() const { return m_FailedOptimizations; }
    const Subgraphs& GetUntouchedSubgraphs() const { return m_UntouchedSubgraphs; }

    Substitutions& GetSubstitutions() { return m_SuccesfulOptimizations; }
    Subgraphs& GetFailedSubgraphs() { return m_FailedOptimizations; }
    Subgraphs& GetUntouchedSubgraphs() { return m_UntouchedSubgraphs; }

    bool Validate(const SubgraphView& originalSubgraph) const;

    ARMNN_DEPRECATED_MSG_REMOVAL_DATE("GetGraph is deprecated. Use GetINetwork instead.", "22.11")
    Graph& GetGraph() { return m_Graph; }

    INetworkPtr& GetINetwork() { return m_INetwork; }

private:
    Substitutions m_SuccesfulOptimizations;     ///< Proposed substitutions from successful optimizations
    Subgraphs m_FailedOptimizations;            ///< Subgraphs from the original subgraph which cannot be supported
    Subgraphs m_UntouchedSubgraphs;             ///< Subgraphs from the original subgraph which remain unmodified

    /// Graph object used only as a container for any layer generated by the optimization process
    Graph m_Graph;

    /// INetworkPtr object used only as a container for any layer generated by the optimization process
    /// Also, can use to AddPrecompiledLayer to the SubstitutionPair
    /// Use in favour of m_Graph which depreciates 22.11
    INetworkPtr m_INetwork = INetwork::Create();;
};

} //namespace armnn
