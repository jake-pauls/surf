#pragma once

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
				const std::string& textureName);

			~VkMaterial();

			inline const VkShaderPipeline* GetShaderPipelinePtr() const { return m_ShaderPipeline; }
			
			inline VkTexture& GetTextureRef() { return m_Texture; }

			inline constexpr bool IsTexturedMaterial() const { return m_IsTexturedMaterial; }

		private:
			void AllocateDescriptorSets();
			void CreateTexture(const std::string& filename);

		private:
			const VkRenderer* c_VkRenderer;
			const VkHardware* c_VkHardware;
			const VkShaderPipeline* m_ShaderPipeline;
			VkTexture m_Texture = {};
			bool m_IsTexturedMaterial = false;
	};
}