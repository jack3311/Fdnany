#pragma once

#include "ResourceManagement.h"

namespace JEngine
{
	class Camera;
	class View;

	class Shader : public Resource
	{
	public:
		enum ShaderComponent : __int8
		{
			VERTEX = 0,
			GEOMETRY = 1,
			FRAGMENT = 2,
			SHADERCOMPONENT_NUM_ITEMS
		};

		static const GLenum glShaderComponent[ShaderComponent::SHADERCOMPONENT_NUM_ITEMS];
		static const std::string shaderComponentNames[ShaderComponent::SHADERCOMPONENT_NUM_ITEMS];

	private:
		GLuint program = 0;

		std::vector<std::string> componentPaths;
		std::vector<std::string> componentSources;

		struct
		{
			GLint
				projectionLocation,
				viewLocation,
				viewProjectionLocation;
		} uniformLocations;


		bool compileComponent(GLuint & _result, int _componentType) const;


		void loadUniformLocations();
		void setFrameUniforms() const;
		void setFrameViewUniforms(const View & _view) const;

	public:
		Shader(const std::initializer_list<std::pair<ShaderComponent, const std::string>> _componentPathsInit);
		~Shader();

		bool loadFromDisk();
		bool initialise();

		void begin() const;
		void begin(const View & _view) const;
		static void end();

		std::shared_ptr<JobCallFunction> loadFromDiskAsync();

		GLuint getProgramID() const;
	};
}