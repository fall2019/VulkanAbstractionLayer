// Copyright(c) 2021, #Momo
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "RenderGraph.h"
#include "VulkanContext.h"
#include "CommandBuffer.h"

namespace VulkanAbstractionLayer
{
    RenderGraph::RenderGraph(std::vector<RenderGraphNode> nodes, std::unordered_map<StringId, Image> images, StringId outputName, PresentCallback onPresent, DestroyCallback onDestroy)
        : nodes(std::move(nodes)), images(std::move(images)), outputName(std::move(outputName)), onPresent(std::move(onPresent)), onDestroy(std::move(onDestroy))
    {
    }

    void RenderGraph::ExecuteRenderGraphNode(const RenderGraphNode& node, CommandBuffer& commandBuffer)
    {
        RenderState state{ *this, commandBuffer, node.ColorAttachments, node.Pass.GetPipelineLayout() };

        if((bool)node.BeforeRender)
            node.BeforeRender(state);

        commandBuffer.BeginRenderPass(node.Pass);

        if ((bool)node.OnRender)
            node.OnRender(state);

        commandBuffer.EndRenderPass();

        if ((bool)node.AfterRender)
            node.AfterRender(state);
    }

    void RenderGraph::Execute(CommandBuffer& commandBuffer)
    {
        for (const auto& node : this->nodes)
        {
            this->ExecuteRenderGraphNode(node, CommandBuffer{ commandBuffer });
        }
    }

    void RenderGraph::Present(CommandBuffer& commandBuffer, const Image& presentImage)
    {
        this->onPresent(commandBuffer, this->images.at(this->outputName), presentImage);
    }

    const RenderGraphNode& RenderGraph::GetNodeByName(StringId name) const
    {
        auto it = std::find_if(this->nodes.begin(), this->nodes.end(), [name](const RenderGraphNode& node) { return node.Name == name; });
        assert(it != this->nodes.end());
        return *it;
    }

    RenderGraph& RenderGraph::operator=(RenderGraph&& other) noexcept
    {
        if (this != std::addressof(other))
        {
            this->~RenderGraph();
            return *std::launder(new(this) RenderGraph(std::move(other)));
        }
        else return *this;
    }

    RenderGraph::~RenderGraph()
    {
        for (const auto& node : this->nodes)
        {
            this->onDestroy(node.Pass);
        }
        this->nodes.clear();
        this->images.clear();
    }

    const Image& RenderGraph::GetImageByName(StringId name) const
    {
        return this->images.at(name);
    }

    const Image& RenderState::GetOutputColorAttachment(size_t index) const
    {
        return this->Graph.GetImageByName(this->ColorAttachments[index]);
    }
}