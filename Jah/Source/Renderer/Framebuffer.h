#pragma once

#include <cstdint>
#include <vector>

namespace Jah {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

		// TODO: filtering / wrap
	};

	struct FramebufferAttachmentSpecification
	{

		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Samples = 1;
		FramebufferAttachmentSpecification Attachments;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(FramebufferSpecification& spec);
		~Framebuffer();

		void Bind();
		void Unbind();

		void Invalidate();
		void Resize(uint32_t width, uint32_t height);
		int ReadPixel(uint32_t attachmentIndex, int x, int y);

		void ClearAttachment(uint32_t attachmentIndex, int value);

		const uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const { JAH_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }

		const FramebufferSpecification& GetSpecification() const { return m_Specification; }

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		uint32_t m_DepthAttachment = 0;
		std::vector<uint32_t> m_ColorAttachments;
	};

}