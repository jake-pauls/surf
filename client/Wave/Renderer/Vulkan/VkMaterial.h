#pragma once

#include <vector>

#include "VkTypes.h"
#include "VkInitializers.h"

namespace vkn
{
	class VkRenderer;
	class VkHardware;
	class VkShaderPipeline;

	struct VkTexture
	{
		VmaAllocatedImage m_Image;
		VkSampler m_Sampler;
	};

	class VkMaterial
	{
		friend class VkRenderer;
		friend class VkHardware;
		friend class VkShaderPipeine;

		public:
			VkMaterial() = default;

			explicit VkMaterial(const VkRenderer& renderer, const VkShaderPipeline& shaderPipeline);

			explicit VkMaterial(const VkRenderer& renderer, 
				const VkShaderPipeline& shaderPipeline, 
				const std::vector<std::string>& textureNames);

			~VkMaterial();

			void BindMaterialTextures(VkCommandBuffer commandBuffer) const;

			inline const VkShaderPipeline* GetShaderPipelinePtr() const { return m_ShaderPipeline; }
			
			inline constexpr bool IsTexturedMaterial() const { return m_IsTexturedMaterial; }

		private:
			void AllocateSamplerDescriptorSet();
			void CreateTexture(const std::string& filename, VkTexture* outTexture);

		private:
			const VkRenderer* c_VkRenderer = nullptr;
			const VkHardware* c_VkHardware = nullptr;
			const VkShaderPipeline* m_ShaderPipeline = nullptr;
			std::vector<VkTexture> m_Textures = {};
			bool m_IsTexturedMaterial = false;
	};
}