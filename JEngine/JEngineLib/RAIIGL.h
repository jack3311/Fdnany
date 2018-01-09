#pragma once

#include <GL\glew.h>

//DEFINES

#define EnableBlend RAIIGL::RAIIWrapper<RAIIGL::_EnableBlend<>> _enableBlendInstance
#define EnableCullFace RAIIGL::RAIIWrapper<RAIIGL::_EnableCullFace> _enableCullFaceInstance


namespace RAIIGL
{
	//WRAPPER

	template <typename T>
	class RAIIWrapper
	{
	private:
		RAIIWrapper(const RAIIWrapper &) = delete;
		RAIIWrapper & operator=(const RAIIWrapper &) = delete;

	public:
		inline RAIIWrapper()
		{
			T::begin();
		}
		inline ~RAIIWrapper()
		{
			T::end();
		}
	};


	//GL_BLEND

	template <GLenum src = GL_SRC_ALPHA, GLenum dst = GL_ONE_MINUS_SRC_ALPHA>
	class _EnableBlend
	{
	public:
		static inline void begin()
		{
			glEnable(GL_BLEND);
			glBlendFunc(src, dst);
		}
		static inline void end()
		{
			glDisable(GL_BLEND);
		}
	};


	//GL_CULL_FACE

	class _EnableCullFace
	{
	public:
		static inline void begin()
		{
			glEnable(GL_CULL_FACE);
		}
		static inline void end()
		{
			glDisable(GL_CULL_FACE);
		}
	};
}