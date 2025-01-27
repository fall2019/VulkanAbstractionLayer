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

#include "Image.h"
#include "VulkanContext.h"
#include <cassert>

namespace VulkanAbstractionLayer
{
    vk::ImageViewType GetImageViewType(const Image& image)
    {
        if (image.GetLayerCount() == 1)
            return vk::ImageViewType::e2D;
        else
            return vk::ImageViewType::eCube;
    }

    vk::ImageAspectFlags ImageFormatToImageAspect(Format format)
    {
        switch (format)
        {
        case Format::D16_UNORM:
            return vk::ImageAspectFlagBits::eDepth;
        case Format::X8D24_UNORM_PACK_32:
            return vk::ImageAspectFlagBits::eDepth;
        case Format::D32_SFLOAT:
            return vk::ImageAspectFlagBits::eDepth;
        case Format::D16_UNORM_S8_UINT:
            return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        case Format::D24_UNORM_S8_UINT:
            return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        case Format::D32_SFLOAT_S8_UINT:
            return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        default:
            return vk::ImageAspectFlagBits::eColor;
        }
    }

    vk::ImageLayout ImageUsageToImageLayout(ImageUsage::Bits layout)
    {
        switch (layout)
        {
        case VulkanAbstractionLayer::ImageUsage::UNKNOWN:
            return vk::ImageLayout::eUndefined;
        case VulkanAbstractionLayer::ImageUsage::TRANSFER_SOURCE:
            return vk::ImageLayout::eTransferSrcOptimal;
        case VulkanAbstractionLayer::ImageUsage::TRANSFER_DISTINATION:
            return vk::ImageLayout::eTransferDstOptimal;
        case VulkanAbstractionLayer::ImageUsage::SHADER_READ:
            return vk::ImageLayout::eShaderReadOnlyOptimal;
        case VulkanAbstractionLayer::ImageUsage::STORAGE:
            return vk::ImageLayout::eGeneral;
        case VulkanAbstractionLayer::ImageUsage::COLOR_ATTACHMENT:
            return vk::ImageLayout::eColorAttachmentOptimal;
        case VulkanAbstractionLayer::ImageUsage::DEPTH_SPENCIL_ATTACHMENT:
            return vk::ImageLayout::eDepthStencilAttachmentOptimal;
        case VulkanAbstractionLayer::ImageUsage::INPUT_ATTACHMENT:
            return vk::ImageLayout::eAttachmentOptimalKHR; // TODO: is it ok?
        case VulkanAbstractionLayer::ImageUsage::FRAGMENT_SHADING_RATE_ATTACHMENT:
            return vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;
        default:
            assert(false);
            return vk::ImageLayout::eUndefined;
        }
    }

    vk::AccessFlags ImageUsageToAccessFlags(ImageUsage::Bits layout)
    {
        switch (layout)
        {
        case ImageUsage::UNKNOWN:
            return vk::AccessFlags{ };
        case ImageUsage::TRANSFER_SOURCE:
            return vk::AccessFlagBits::eTransferRead;
        case ImageUsage::TRANSFER_DISTINATION:
            return vk::AccessFlagBits::eTransferWrite;
        case ImageUsage::SHADER_READ:
            return vk::AccessFlagBits::eShaderRead;
        case ImageUsage::STORAGE:
            return vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite; // TODO: what if storage is not read or write?
        case ImageUsage::COLOR_ATTACHMENT:
            return vk::AccessFlagBits::eColorAttachmentWrite;
        case ImageUsage::DEPTH_SPENCIL_ATTACHMENT:
            return vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        case ImageUsage::INPUT_ATTACHMENT:
            return vk::AccessFlagBits::eInputAttachmentRead;
        case ImageUsage::FRAGMENT_SHADING_RATE_ATTACHMENT:
            return vk::AccessFlagBits::eFragmentShadingRateAttachmentReadKHR;
        default:
            assert(false);
            return vk::AccessFlags{ };
        }
    }

    vk::PipelineStageFlags ImageUsageToPipelineStage(ImageUsage::Bits layout)
    {
        switch (layout)
        {
        case ImageUsage::UNKNOWN:
            return vk::PipelineStageFlagBits::eTopOfPipe;
        case ImageUsage::TRANSFER_SOURCE:
            return vk::PipelineStageFlagBits::eTransfer;
        case ImageUsage::TRANSFER_DISTINATION:
            return vk::PipelineStageFlagBits::eTransfer;
        case ImageUsage::SHADER_READ:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: whats for vertex shader reads?
        case ImageUsage::STORAGE:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: whats for vertex shader reads?
        case ImageUsage::COLOR_ATTACHMENT:
            return vk::PipelineStageFlagBits::eColorAttachmentOutput;
        case ImageUsage::DEPTH_SPENCIL_ATTACHMENT:
            return vk::PipelineStageFlagBits::eEarlyFragmentTests; // TODO: whats for late fragment test?
        case ImageUsage::INPUT_ATTACHMENT:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: check if at least works
        case ImageUsage::FRAGMENT_SHADING_RATE_ATTACHMENT:
            return vk::PipelineStageFlagBits::eFragmentShadingRateAttachmentKHR;
        default:
            assert(false);
            return vk::PipelineStageFlags{ };
        }
    }

    vk::ImageSubresourceLayers GetDefaultImageSubresourceLayers(const Image& image)
    {
        auto subresourceRange = GetDefaultImageSubresourceRange(image);
        return vk::ImageSubresourceLayers{
            subresourceRange.aspectMask,
            subresourceRange.baseMipLevel,
            subresourceRange.baseArrayLayer,
            subresourceRange.layerCount
        };
    }

    vk::ImageSubresourceLayers GetDefaultImageSubresourceLayers(const Image& image, uint32_t mipLevel, uint32_t layer)
    {
        return vk::ImageSubresourceLayers{
            ImageFormatToImageAspect(image.GetFormat()),
            mipLevel,
            layer,
            1
        };
    }

    vk::ImageSubresourceRange GetDefaultImageSubresourceRange(const Image& image)
    {
        return vk::ImageSubresourceRange{
            ImageFormatToImageAspect(image.GetFormat()),
            0, // base mip level
            image.GetMipLevelCount(),
            0, // base layer
            image.GetLayerCount()
        };
    }

    uint32_t CalculateImageMipLevelCount(ImageOptions::Value options, uint32_t width, uint32_t height)
    {
        if (options & ImageOptions::MIPMAPS)
            return (uint32_t)std::floor(std::log2(std::max(width, height))) + 1;
        else
            return 1;
    }

    uint32_t CalculateImageLayerCount(ImageOptions::Value options)
    {
        if (options & ImageOptions::CUBEMAP)
            return 6;
        else
            return 1;
    }

    void Image::Destroy()
    {
        if ((bool)this->handle)
        {
            if ((bool)this->allocation) // allocated
            {
                DeallocateImage(this->handle, this->allocation);
            }

            GetCurrentVulkanContext().GetDevice().destroyImageView(this->defaultImageViews.NativeView);
            if ((bool)this->defaultImageViews.DepthOnlyView)
                GetCurrentVulkanContext().GetDevice().destroyImageView(this->defaultImageViews.DepthOnlyView);
            if ((bool)this->defaultImageViews.StencilOnlyView)
                GetCurrentVulkanContext().GetDevice().destroyImageView(this->defaultImageViews.StencilOnlyView);

            for (auto& imageViewLayer : this->cubemapImageViews)
            {
                GetCurrentVulkanContext().GetDevice().destroyImageView(imageViewLayer.NativeView);
                if ((bool)imageViewLayer.DepthOnlyView)
                    GetCurrentVulkanContext().GetDevice().destroyImageView(imageViewLayer.DepthOnlyView);
                if ((bool)imageViewLayer.StencilOnlyView)
                    GetCurrentVulkanContext().GetDevice().destroyImageView(imageViewLayer.StencilOnlyView);
            }

            this->handle = vk::Image{ };
            this->defaultImageViews = { };
            this->cubemapImageViews.clear();
            this->extent = vk::Extent2D{ 0u, 0u };
            this->mipLevelCount = 1;
            this->layerCount = 1;
        }
    }

    void Image::InitViews(const vk::Image& image, Format format)
    {
        this->handle = image;
        this->format = format;

        auto& Vulkan = GetCurrentVulkanContext();
        auto subresourceRange = GetDefaultImageSubresourceRange(*this);

        vk::ImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo
            .setImage(this->handle)
            .setViewType(GetImageViewType(*this))
            .setFormat(ToNative(format))
            .setComponents(vk::ComponentMapping{
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity
            })
            .setSubresourceRange(subresourceRange);

        auto nativeSubresourceRange = GetDefaultImageSubresourceRange(*this);
        imageViewCreateInfo.setSubresourceRange(nativeSubresourceRange);
        this->defaultImageViews.NativeView = Vulkan.GetDevice().createImageView(imageViewCreateInfo);

        auto depthSubresourceRange = GetDefaultImageSubresourceRange(*this);
        depthSubresourceRange.setAspectMask(depthSubresourceRange.aspectMask & vk::ImageAspectFlagBits::eDepth);
        if (depthSubresourceRange.aspectMask != vk::ImageAspectFlags{ })
        {
            imageViewCreateInfo.setSubresourceRange(depthSubresourceRange);
            this->defaultImageViews.DepthOnlyView = GetCurrentVulkanContext().GetDevice().createImageView(imageViewCreateInfo);
        }

        auto stencilSubresourceRange = GetDefaultImageSubresourceRange(*this);
        stencilSubresourceRange.setAspectMask(stencilSubresourceRange.aspectMask & vk::ImageAspectFlagBits::eStencil);
        if (stencilSubresourceRange.aspectMask != vk::ImageAspectFlags{ })
        {
            imageViewCreateInfo.setSubresourceRange(stencilSubresourceRange);
            this->defaultImageViews.StencilOnlyView = GetCurrentVulkanContext().GetDevice().createImageView(imageViewCreateInfo);
        }

        if (this->layerCount > 1)
        {
            this->cubemapImageViews.resize(this->layerCount);
            imageViewCreateInfo.setViewType(vk::ImageViewType::e2DArray);
        }
        uint32_t layer = 0;
        for (auto& imageViewLayer : this->cubemapImageViews)
        {
            auto nativeSubresourceRange = GetDefaultImageSubresourceRange(*this);
            nativeSubresourceRange
                .setBaseArrayLayer(layer)
                .setLayerCount(VK_REMAINING_ARRAY_LAYERS);
            imageViewCreateInfo.setSubresourceRange(nativeSubresourceRange);
            imageViewLayer.NativeView = Vulkan.GetDevice().createImageView(imageViewCreateInfo);

            auto depthSubresourceRange = GetDefaultImageSubresourceRange(*this);
            depthSubresourceRange
                .setAspectMask(depthSubresourceRange.aspectMask & vk::ImageAspectFlagBits::eDepth)
                .setBaseArrayLayer(layer)
                .setLayerCount(VK_REMAINING_ARRAY_LAYERS);
            if (depthSubresourceRange.aspectMask != vk::ImageAspectFlags{ })
            {
                imageViewCreateInfo.setSubresourceRange(depthSubresourceRange);
                imageViewLayer.DepthOnlyView = GetCurrentVulkanContext().GetDevice().createImageView(imageViewCreateInfo);
            }

            auto stencilSubresourceRange = GetDefaultImageSubresourceRange(*this);
            stencilSubresourceRange
                .setAspectMask(stencilSubresourceRange.aspectMask & vk::ImageAspectFlagBits::eStencil)
                .setBaseArrayLayer(layer)
                .setLayerCount(VK_REMAINING_ARRAY_LAYERS);
            if (stencilSubresourceRange.aspectMask != vk::ImageAspectFlags{ })
            {
                imageViewCreateInfo.setSubresourceRange(stencilSubresourceRange);
                imageViewLayer.StencilOnlyView = GetCurrentVulkanContext().GetDevice().createImageView(imageViewCreateInfo);
            }

            layer++;
        }
    }

    Image::Image(uint32_t width, uint32_t height, Format format, ImageUsage::Value usage, MemoryUsage memoryUsage, ImageOptions::Value options)
    {
        this->Init(width, height, format, usage, memoryUsage, options);
    }

    Image::Image(vk::Image image, uint32_t width, uint32_t height, Format format)
    {
        this->extent = vk::Extent2D{ width, height };
        this->allocation = { }; // image is external resource
        this->InitViews(image, format);
    }

    Image::Image(Image&& other) noexcept
    {
        this->handle = other.handle;
        this->defaultImageViews = other.defaultImageViews;
        this->cubemapImageViews = std::move(other.cubemapImageViews);
        this->extent = other.extent;
        this->format = other.format;
        this->allocation = other.allocation;
        this->mipLevelCount = other.mipLevelCount;
        this->layerCount = other.layerCount;

        other.handle = vk::Image{ };
        other.defaultImageViews = { };
        other.cubemapImageViews.clear();
        other.extent = vk::Extent2D{ 0u, 0u };
        other.format = Format::UNDEFINED;
        other.allocation = { };
        other.mipLevelCount = 1;
        other.layerCount = 1;
    }

    Image& Image::operator=(Image&& other) noexcept
    {
        this->Destroy();

        this->handle = other.handle;
        this->defaultImageViews = other.defaultImageViews;
        this->cubemapImageViews = std::move(other.cubemapImageViews);
        this->extent = other.extent;
        this->format = other.format;
        this->allocation = other.allocation;
        this->mipLevelCount = other.mipLevelCount;
        this->layerCount = other.layerCount;

        other.handle = vk::Image{ };
        other.defaultImageViews = { };
        other.cubemapImageViews.clear();
        other.extent = vk::Extent2D{ 0u, 0u };
        other.format = Format::UNDEFINED;
        other.allocation = { };
        other.mipLevelCount = 1;
        other.layerCount = 1;

        return *this;
    }

    Image::~Image()
    {
        this->Destroy();
    }

    void Image::Init(uint32_t width, uint32_t height, Format format, ImageUsage::Value usage, MemoryUsage memoryUsage, ImageOptions::Value options)
    {
        this->mipLevelCount = CalculateImageMipLevelCount(options, width, height);
        this->layerCount = CalculateImageLayerCount(options);

        vk::ImageCreateInfo imageCreateInfo;
        imageCreateInfo
            .setImageType(vk::ImageType::e2D)
            .setFormat(ToNative(format))
            .setExtent(vk::Extent3D{ width, height, 1 })
            .setSamples(vk::SampleCountFlagBits::e1)
            .setMipLevels(this->GetMipLevelCount())
            .setArrayLayers(this->layerCount)
            .setTiling(vk::ImageTiling::eOptimal)
            .setUsage((vk::ImageUsageFlags)usage)
            .setSharingMode(vk::SharingMode::eExclusive)
            .setInitialLayout(vk::ImageLayout::eUndefined);

        if (options & ImageOptions::CUBEMAP)
            imageCreateInfo.setFlags(vk::ImageCreateFlagBits::eCubeCompatible);
        
        this->extent = vk::Extent2D{ (uint32_t)width, (uint32_t)height };
        this->allocation = AllocateImage(imageCreateInfo, memoryUsage, &this->handle);
        this->InitViews(this->handle, format);
    }

    vk::ImageView Image::GetNativeView(ImageView view) const
    {
        switch (view)
        {
        case VulkanAbstractionLayer::ImageView::NATIVE:
            return this->defaultImageViews.NativeView;
        case VulkanAbstractionLayer::ImageView::DEPTH_ONLY:
            return this->defaultImageViews.DepthOnlyView;
        case VulkanAbstractionLayer::ImageView::STENCIL_ONLY:
            return this->defaultImageViews.StencilOnlyView;
        default:
            assert(false);
            return this->defaultImageViews.NativeView;
        }
    }

    vk::ImageView Image::GetNativeView(ImageView view, uint32_t layer) const
    {
        if (this->layerCount == 1)
            return this->GetNativeView(view);

        switch (view)
        {
        case VulkanAbstractionLayer::ImageView::NATIVE:
            return this->cubemapImageViews[layer].NativeView;
        case VulkanAbstractionLayer::ImageView::DEPTH_ONLY:
            return this->cubemapImageViews[layer].DepthOnlyView;
        case VulkanAbstractionLayer::ImageView::STENCIL_ONLY:
            return this->cubemapImageViews[layer].StencilOnlyView;
        default:
            assert(false);
            return this->cubemapImageViews[layer].NativeView;
        }
    }

    uint32_t Image::GetMipLevelWidth(uint32_t mipLevel) const
    {
        auto safeWidth = std::max(this->GetWidth(), 1u << mipLevel);
        return safeWidth >> mipLevel;
    }

    uint32_t Image::GetMipLevelHeight(uint32_t mipLevel) const
    {
        auto safeHeight = std::max(this->GetHeight(), 1u << mipLevel);
        return safeHeight >> mipLevel;
    }
}