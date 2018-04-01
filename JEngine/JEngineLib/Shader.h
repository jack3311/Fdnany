#pragma once

#include "ResourceManagement.h"

#include "Engine.h"
#include "Entity.h"
#include "World.h"

namespace JEngine
{
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
				modelLocation,
				viewProjectionLocation,
				modelViewProjectionLocation;
		} uniformLocations;


		bool compileComponent(GLuint & _result, int _componentType) const;


		void loadUniformLocations();
		void setFrameUniforms() const;
		void setFrameViewUniforms(const View & _view) const;
		void setTransformUniforms(const View & _view, const ECS::Entity & _transform) const;

	public:
		Shader();
		~Shader();

		bool loadFromDisk(const std::vector<std::pair<ShaderComponent, const std::string>> _componentPathsInit);
		bool initialise();

		void begin(const ECS::Entity & _transform = Engine::get().getWorld().getEntityManager().getRoot(), 
			const View & _view = Engine::get().getStandardView()) const;
		static void end();

		std::shared_ptr<JobCallFunction> loadFromDiskAsync(const std::vector<std::pair<ShaderComponent, const std::string>> _componentPathsInit);

		GLuint getProgramID() const;
	};
}