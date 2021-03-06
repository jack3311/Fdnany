#pragma once

#include "UniformBuffer.h"
#include "Constants.h"
#include "Shader.h"
#include "ResourceManagement.h"

namespace JEngine
{
	class MaterialInterface
	{
	public:
		virtual void begin() = 0;

		virtual Shader & getShader() = 0;

	};

	template <typename UniformBufferFormat>
	class Material : public MaterialInterface
	{
	private:
		Shader * shader;
		std::unique_ptr<UniformBuffer<UniformBufferFormat>> materialProperties;
		std::map<GLuint, std::shared_ptr<ResourceTexture>> textures;

		void bindTextures() const;

	public:
		Material();
		~Material();

		bool initialise(Shader * _shader);

		UniformBuffer<UniformBufferFormat> & getMaterialProperties();
		std::map<GLuint, std::shared_ptr<ResourceTexture>> & getTextures();

		virtual void begin();
		virtual Shader & getShader();
	};

	template<typename UniformBufferFormat>
	inline void Material<UniformBufferFormat>::bindTextures() const
	{
		for (auto itr = textures.begin(); itr != textures.end(); ++itr)
		{
			itr->second->bind(itr->first);
		}
	}

	template<typename UniformBufferFormat>
	inline Material<UniformBufferFormat>::Material()
	{
		materialProperties = std::make_unique<UniformBuffer<UniformBufferFormat>>();
	}

	template<typename UniformBufferFormat>
	inline Material<UniformBufferFormat>::~Material()
	{
	}

	template<typename UniformBufferFormat>
	inline bool Material<UniformBufferFormat>::initialise(Shader * _shader)
	{
		shader = _shader;

		ERR_IF(!materialProperties->initialise(UNIFORM_BUFFER_MATERIAL_INFO_BINDING_LOCATION), "Could not initialise material properties uniform buffer", "Initialised material properties uniform buffer");

		return true;
	}

	template<typename UniformBufferFormat>
	inline UniformBuffer<UniformBufferFormat> & Material<UniformBufferFormat>::getMaterialProperties()
	{
		return materialProperties;
	}

	template<typename UniformBufferFormat>
	inline std::map<GLuint, std::shared_ptr<ResourceTexture>> & Material<UniformBufferFormat>::getTextures()
	{
		return textures;
	}

	template<typename UniformBufferFormat>
	inline void Material<UniformBufferFormat>::begin()
	{
		//Set these material properties as current
		materialProperties->bind();

		//Bind shader
		shader->begin();

		//Bind textures
		bindTextures();
	}

	template<typename UniformBufferFormat>
	inline Shader & Material<UniformBufferFormat>::getShader()
	{
		return *shader;
	}
}