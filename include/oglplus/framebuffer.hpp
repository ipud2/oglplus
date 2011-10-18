/**
 *  @file oglplus/framebuffer.hpp
 *  @brief Framebuffer object wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_FRAMEBUFFER_1107121519_HPP
#define OGLPLUS_FRAMEBUFFER_1107121519_HPP

#include <oglplus/error.hpp>
#include <oglplus/object.hpp>
#include <oglplus/friend_of.hpp>
#include <oglplus/auxiliary/binding_query.hpp>
#include <oglplus/renderbuffer.hpp>
#include <oglplus/texture.hpp>
#include <cassert>

namespace oglplus {

#ifdef OGLPLUS_DOCUMENTATION_ONLY
/// Type for the framebuffer color attachment (implementation-dependent) number
class FramebufferColorAttachment
 : public LimitedCount
{
public:
	FramebufferColorAttachment(GLuint count);
};
#else
OGLPLUS_DECLARE_LIMITED_COUNT_TYPE(
	FramebufferColorAttachment,
	MAX_COLOR_ATTACHMENTS
)
#endif

/// Wrapper for OpenGL framebuffer operations
/**
 *  @note Do not use this class directly, use FrameBuffer instead
 *
 *  @see Framebuffer
 */
class FramebufferOps
 : public Named
 , public FriendOf<RenderbufferOps>
 , public FriendOf<TextureOps>
{
protected:
	static void _init(GLsizei count, GLuint& _name)
	{
		::glGenFramebuffers(count, &_name);
		ThrowOnError(OGLPLUS_ERROR_INFO(GenFramebuffers));
	}

	static void _cleanup(GLsizei count, GLuint& _name)
	{
		assert(_name != 0);
		::glDeleteFramebuffers(count, &_name);
	}

	static GLboolean _is_x(GLuint _name)
	{
		assert(_name != 0);
		return ::glIsFramebuffer(_name);
	}

	friend class FriendOf<FramebufferOps>;
public:
	/// Framebuffer renderbuffer attachment points
	enum class Attachment : GLenum {
#include <oglplus/enums/framebuffer_attachment.ipp>
	};

	/// Framebuffer bind targets
	enum class Target : GLenum {
#include <oglplus/enums/framebuffer_target.ipp>
	};

	/// Framebuffer status enumeration
	enum class Status : GLenum {
#include <oglplus/enums/framebuffer_status.ipp>
	};
protected:
	static GLenum _binding_query(Target target)
	{
		switch(GLenum(target))
		{
#include <oglplus/enums/framebuffer_target_bq.ipp>
			default:;
		}
		return 0;
	}
	friend class BindingQuery<FramebufferOps>;
public:

	/// Bind this framebuffer to the specified target
	/**
	 *  @throws Error
	 */
	void Bind(Target target) const
	{
		assert(_name != 0);
		::glBindFramebuffer(GLenum(target), _name);
		AssertNoError(OGLPLUS_OBJECT_ERROR_INFO(
			BindFramebuffer,
			Framebuffer,
			_name
		));
	}

	/// Binds the default framebuffer to the specified target
	/**
	 *  @throws Error
	 */
	static void BindDefault(Target target)
	{
		::glBindFramebuffer(GLenum(target), 0);
		AssertNoError(OGLPLUS_ERROR_INFO(BindFramebuffer));
	}

	/// Checks the status of the framebuffer
	/** Returns one of the values in the @c Status enumeration. For complete
	 *  framebuffers this member function returns Status::Complete.
	 *
	 *  @see IsComplete
	 */
	static Status CheckStatus(Target target)
	{
		GLenum result = ::glCheckFramebufferStatus(GLenum(target));
		if(result == 0) ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CheckFramebufferStatus,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
		return Status(result);
	}

	/// Returns true if the framebuffer is complete
	/**
	 *  @see Status
	 *  @see CheckStatus
	 */
	static bool IsComplete(Target target)
	{
		return CheckStatus(target) == Status::Complete;
	}

	/// Attach a @p renderbuffer to the @p attachment point of @p target
	static void AttachRenderbuffer(
		Target target,
		Attachment attachment,
		const RenderbufferOps& renderbuffer
	)
	{
		::glFramebufferRenderbuffer(
			GLenum(target),
			GLenum(attachment),
			GL_RENDERBUFFER,
			FriendOf<RenderbufferOps>::GetName(renderbuffer)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferRenderbuffer,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}

	/// Attach a @p renderbuffer to the color @p attachment of @p target
	static void AttachColorRenderbuffer(
		Target target,
		FramebufferColorAttachment attachment,
		const RenderbufferOps& renderbuffer
	)
	{
		::glFramebufferRenderbuffer(
			GLenum(target),
			GL_COLOR_ATTACHMENT0 + GLuint(attachment),
			GL_RENDERBUFFER,
			FriendOf<RenderbufferOps>::GetName(renderbuffer)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferRenderbuffer,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}

	/// Attach a @p texture to the @p attachment point of @p target
	static void AttachTexture(
		Target target,
		Attachment attachment,
		const TextureOps& texture,
		GLint level
	)
	{
		::glFramebufferTexture(
			GLenum(target),
			GLenum(attachment),
			FriendOf<TextureOps>::GetName(texture),
			level
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferTexture,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}

	/// Attach a @p texture to the color @p attachment point of @p target
	static void AttachColorTexture(
		Target target,
		FramebufferColorAttachment attachment,
		const TextureOps& texture,
		GLint level
	)
	{
		::glFramebufferTexture(
			GLenum(target),
			GL_COLOR_ATTACHMENT0 + GLenum(attachment),
			FriendOf<TextureOps>::GetName(texture),
			level
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferTexture,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}

	/// Attach a 1D @p texture to the @p attachment point of @p target
	static void AttachTexture1D(
		Target target,
		Attachment attachment,
		Texture::Target textarget,
		const TextureOps& texture,
		GLint level
	)
	{
		::glFramebufferTexture1D(
			GLenum(target),
			GLenum(attachment),
			GLenum(textarget),
			FriendOf<TextureOps>::GetName(texture),
			level
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferTexture1D,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}

	/// Attach a 2D @p texture to the @p attachment point of @p target
	static void AttachTexture2D(
		Target target,
		Attachment attachment,
		Texture::Target textarget,
		const TextureOps& texture,
		GLint level
	)
	{
		::glFramebufferTexture2D(
			GLenum(target),
			GLenum(attachment),
			GLenum(textarget),
			FriendOf<TextureOps>::GetName(texture),
			level
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferTexture2D,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}

	/// Attach a 3D @p texture to the @p attachment point of @p target
	static void AttachTexture3D(
		Target target,
		Attachment attachment,
		Texture::Target textarget,
		const TextureOps& texture,
		GLint level,
		GLint layer
	)
	{
		::glFramebufferTexture3D(
			GLenum(target),
			GLenum(attachment),
			GLenum(textarget),
			FriendOf<TextureOps>::GetName(texture),
			level,
			layer
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferTexture3D,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}

	/// Attach a @p texture layer to the @p attachment point of @p target
	static void AttachTextureLayer(
		Target target,
		Attachment attachment,
		const TextureOps& texture,
		GLint level,
		GLint layer
	)
	{
		::glFramebufferTextureLayer(
			GLenum(target),
			GLenum(attachment),
			FriendOf<TextureOps>::GetName(texture),
			level,
			layer
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			FramebufferTextureLayer,
			Framebuffer,
			BindingQuery<FramebufferOps>::QueryBinding(target)
		));
	}
};

#ifdef OGLPLUS_DOCUMENTATION_ONLY
/// An @ref oglplus_object encapsulating the OpenGL framebuffer functionality
/**
 *  @ingroup objects
 */
class Framebuffer
 : public FramebufferOps
{ };
#else
typedef Object<FramebufferOps, true> Framebuffer;
#endif

} // namespace oglplus

#endif // include guard
