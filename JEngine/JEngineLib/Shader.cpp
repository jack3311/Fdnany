#include "Shader.h"

#include "Util.h"
#include "Logger.h"
#include "Engine.h"

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

			Logger::getLogger().log(strJoinConvert("Could not compile ", shaderComponentNames[_componentType], ": ", &shader_log[0]), LogLevel::ERROR);
			
			return false;
		}

		return true;
	}

	Shader::Shader(const std::initializer_list<std::pair<ShaderComponent, const std::string>> _componentPathsInit) :
		componentPaths(ShaderComponent::SHADERCOMPONENT_NUM_ITEMS),
		componentSources(ShaderComponent::SHADERCOMPONENT_NUM_ITEMS)
	{
		for (auto & item : _componentPathsInit)
		{
			componentPaths[item.first] = item.second;
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(program);
	}

	bool Shader::loadFromDisk()
	{
		for (int i = 0; i < ShaderComponent::SHADERCOMPONENT_NUM_ITEMS; ++i)
		{
			auto path = componentPaths[i];

			if (path.size() == 0)
			{
				if (i == ShaderComponent::VERTEX ||
					i == ShaderComponent::FRAGMENT)
				{
					Logger::getLogger().log(strJoin({ "Shader must have a ", shaderComponentNames[i], " component." }), LogLevel::ERROR);
					return false;
				}
				
				continue;
			}

			bool result = readAllFile(componentSources[i], path);

			if (!result)
			{
				Logger::getLogger().log(strJoin({ "Could not read shader file: ", path }), LogLevel::ERROR);
				return false;
			}
		}

		return true;
	}

	bool Shader::initialise()
	{
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
			
			Logger::getLogger().log(strJoinConvert("Could not link shader: ", &program_log[0]), LogLevel::ERROR);

			//Critical error - no need to clean up components

			return false;
		}

		//Clean up
		for (int i = 0; i < ShaderComponent::SHADERCOMPONENT_NUM_ITEMS; ++i)
		{
			glDeleteShader(componentsCompiled[i]);
		}

		componentSources.clear();

		return true;
	}

	void Shader::bind() const
	{
		glUseProgram(program);
	}

	void Shader::unbind()
	{
		glUseProgram(0);
	}

	std::shared_ptr<JobCallFunction> Shader::loadFromDiskAsync()
	{
		auto job = std::make_shared<JobCallFunction>([this]() -> bool {
			return loadFromDisk();
		});
		
		Engine::getEngine().getJobManager().enqueueJob(job);

		return job;
	}
}