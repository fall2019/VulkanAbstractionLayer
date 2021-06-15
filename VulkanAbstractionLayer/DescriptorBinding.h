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

#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "Buffer.h"
#include "Image.h"
#include "Sampler.h"
#include "ShaderReflection.h"

namespace VulkanAbstractionLayer
{
	class DescriptorBinding
	{
		struct DescriptorWriteInfo
		{
			UniformType Type;
			uint32_t Binding;
			uint32_t FirstIndex;
			uint32_t Count;
		};

		std::vector<DescriptorWriteInfo> descriptorWrites;
		std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos;
		std::vector<vk::DescriptorImageInfo> descriptorImageInfos;

		size_t AllocateBinding(const Buffer& buffer);
		size_t AllocateBinding(const Image& image);
		size_t AllocateBinding(const Sampler& sampler);
	public:
		using BufferRef = std::reference_wrapper<const Buffer>;
		using ImageRef = std::reference_wrapper<const Image>;
		using SamplerRef = std::reference_wrapper<const Sampler>;

		DescriptorBinding& Bind(uint32_t binding, const Buffer& buffer, UniformType type);
		DescriptorBinding& Bind(uint32_t binding, const Image& image, UniformType type);
		DescriptorBinding& Bind(uint32_t binding, const Sampler& sampler, UniformType type);

		DescriptorBinding& Bind(uint32_t binding, const std::vector<BufferRef>& buffers, UniformType type);
		DescriptorBinding& Bind(uint32_t binding, const std::vector<ImageRef>& images, UniformType type);
		DescriptorBinding& Bind(uint32_t binding, const std::vector<SamplerRef>& samplers, UniformType type);

		void Write(const vk::DescriptorSet& descriptorSet) const;
	};
}