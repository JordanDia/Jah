#pragma once
#include <cstdint>

namespace Jah {

	struct FramebufferSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(FramebufferSpecification& spec);
		~Framebuffer();

		void Invalidate();
		void Resize(uint32_t width, uint32_t height);

		void Bind();
		void Unbind();

		const uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

		const FramebufferSpecification& GetSpecification() const { return m_Specification; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};

}