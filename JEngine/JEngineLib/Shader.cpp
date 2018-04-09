#include "Shader.h"

#include "Util.h"
#include "Logger.h"
#include "Engine.h"
#include "Camera.h"
#include "Maths.h"
#include "View.h"

namespace JEngine
{
	const GLenum Shader::glShaderComponent[ShaderComponent::SHADERCOMPONENT_NUM_ITEMS] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	const std::string Shader::shaderComponentNames[ShaderComponent::SHADERCOMPONENT_NUM_ITEMS] = { "Vertex Shader", "Geometry Shader", "Fragment Shader" };


	bool Shader::compileComponent(GLuint & _result, int _componentType) const
	{
		_result = glCreateShader(glShaderComponent[_componentType]);

		const auto & source = componentSources[_componentType];
		const char * shader_code_ptr = source.c_str();
		GLint shader_code_size = static_cast<GLint>(source.size());

		glShaderSource(_result, 1, &shader_code_ptr, &shader_code_size);
		glCompileShader(_result);

		GLint compileResult = 0;
		glGetShaderiv(_result, GL_COMPILE_STATUS, &compileResult);

		if (compileResult == GL_FALSE)
		{
			int info_log_length = 0;
			glGetShaderiv(_result, GL_INFO_LOG_LENGTH, &info_log_length);
			std::vector<char> shader_log(info_log_length);
			glGetShaderInfoLog(_result, info_log_length, NULL, &shader_log[0]);

			Logger::get().log(strJoinConvert("Could not compile ", shaderComponentNames[_componentType], ": ", &shader_log[0]), LogLevel::ERROR);
			
			return false;
		}

		return true;
	}

	Shader::Shader() :
		componentPaths(ShaderComponent::SHADERCOMPONENT_NUM_ITEMS),
		componentSources(ShaderComponent::SHADERCOMPONENT_NUM_ITEMS)
	{
	}

	Shader::~Shader()
	{
		glDeleteProgram(program);
	}

	bool Shader::loadFromDisk(const std::vector<std::pair<ShaderComponent, const std::string>> _componentPathsInit)
	{
		//Split the argument
		for (auto & item : _componentPathsInit)
		{
			componentPaths[item.first] = item.second;
		}

		//Load the files from disk
		for (int i = 0; i < ShaderComponent::SHADERCOMPONENT_NUM_ITEMS; ++i)
		{
			auto path = componentPaths[i];

			if (path.size() == 0)
			{
				if (i == ShaderComponent::VERTEX ||
					i == ShaderComponent::FRAGMENT)
				{
					Logger::get().log(strJoin({ "Shader must have a ", shaderComponentNames[i], " component." }), LogLevel::ERROR);
					return false;
				}
				
				continue;
			}

			bool result = readAllFile(componentSources[i], path);

			if (!result)
			{
				Logger::get().log(strJoin({ "Could not read shader file: ", path }), LogLevel::ERROR);
				return false;
			}
		}

		return true;
	}

	bool Shader::initialise()
	{
		assert(Engine::get().isCurrentThreadMain());

		std::vector<GLuint> componentsCompiled(ShaderComponent::SHADERCOMPONENT_NUM_ITEMS, 0u);
		
		//Compile each component
		for (int i = 0; i < ShaderComponent::SHADERCOMPONENT_NUM_ITEMS; ++i)
		{
			if (componentSources[i].size() == 0)
			{
				continue;
			}

			if (!compileComponent(componentsCompiled[i], i))
			{
				//Error message within compileComponent
				//Critical error - no need to clean up components
				return false;
			}
		}

		//Create program
		program = glCreateProgram();

		//Attach each component
		for (int i = 0; i < ShaderComponent::SHADERCOMPONENT_NUM_ITEMS; ++i)
		{
			if (componentsCompiled[i] == 0)
			{
				continue;
			}

			glAttachShader(program, componentsCompiled[i]);
		}

		//Link program
		glLinkProgram(program);

		//Check for errs
		GLint linkStatus = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		
		if (linkStatus == GL_FALSE)
		{
			int info_log_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
			std::vector<char> program_log(info_log_length);
			glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
			
			Logger::get().log(strJoinConvert("Could not link shader: ", &program_log[0]), LogLevel::ERROR);

			//Critical error - no need to clean up components

			return false;
		}

		//Clean up
		for (int i = 0; i < ShaderComponent::SHADERCOMPONENT_NUM_ITEMS; ++i)
		{
			glDeleteShader(componentsCompiled[i]);
		}

		componentSources.clear();

		//Load uniform locations
		loadUniformLocations();

		return true;
	}

	void Shader::begin(const ECS::Entity & _transform) const
	{
		assert(Engine::get().isCurrentThreadMain());

		glUseProgram(program);

		setFrameUniforms();
		setFrameViewUniforms();
		setTransformUniforms(_transform);
	}

	void Shader::end()
	{
		assert(Engine::get().isCurrentThreadMain());

		glUseProgram(0);
	}

	std::shared_ptr<JobCallFunction> Shader::loadFromDiskAsync(const std::vector<std::pair<ShaderComponent, const std::string>> _componentPathsInit)
	{
		auto job = std::make_shared<JobCallFunction>([this, _componentPathsInit]() -> bool {
			return loadFromDisk(_componentPathsInit);
		});
		
		Engine::get().getJobManager().enqueueJob(job);

		return job;
	}

	GLuint Shader::getProgramID() const
	{
		return program;
	}

	void Shader::loadUniformLocations()
	{
		uniformLocations.viewLocation = glGetUniformLocation(program, "viewMatrix");
		uniformLocations.projectionLocation = glGetUniformLocation(program, "projectionMatrix");
		uniformLocations.modelLocation = glGetUniformLocation(program, "modelMatrix");
		uniformLocations.viewProjectionLocation = glGetUniformLocation(program, "viewProjectionMatrix");
		uniformLocations.modelViewProjectionLocation = glGetUniformLocation(program, "modelViewProjectionMatrix");
	}

	void Shader::setFrameUniforms() const
	{
		//Below is example
	}

	void Shader::setFrameViewUniforms() const
	{
		const View & currentView = Engine::get().getCurrentView();
		const std::shared_ptr<Camera> & camera = currentView.getCamera();

		glUniformMatrix4fv(uniformLocations.viewLocation, 1, false, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(uniformLocations.projectionLocation, 1, false, glm::value_ptr(camera->getProjectionMatrix()));
		glUniformMatrix4fv(uniformLocations.viewProjectionLocation, 1, false, glm::value_ptr(camera->getViewProjectionMatrix()));
	}

	void Shader::setTransformUniforms(const ECS::Entity & _transform) const
	{
		const View & currentView = Engine::get().getCurrentView();
		const std::shared_ptr<Camera> & camera = currentView.getCamera();
		
		glUniformMatrix4fv(uniformLocations.modelLocation, 1, false, glm::value_ptr(_transform.getGlobalTransformMatrix()));
		
		//Calculate and set MVP
		glUniformMatrix4fv(uniformLocations.modelViewProjectionLocation, 1, false, glm::value_ptr(
			camera->getViewProjectionMatrix() * _transform.getGlobalTransformMatrix()
		));
	}
}