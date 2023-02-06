#pragma once

#include <glm/glm.hpp>
#include <map>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "rendering/graphics_object.h"

namespace rendering {
	enum shader_type {
		vertex = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
	};

	struct shader_specification {
		std::string vertex_path;
		std::string fragment_path;

		shader_specification() = default;

		shader_specification(std::string vertex_path,
			std::string fragment_path)
			: vertex_path(std::move(vertex_path)), fragment_path(std::move(fragment_path)) {}
	};

	class shader : public graphics_object {
	public:
		shader(const shader_specification& specification);
		~shader();

		void bind() override;
		void un_bind() override;

		void set_int(const std::string& uniform, int value);
		void set_mat4(const std::string& uniform, const glm::mat4& value);

		unsigned int get_uniform_location(const std::string& uniform);

		static shared<shader> create(const shader_specification& specification);

	private:
		std::string
			parse_shader_contents_from_file(const std::string& shader_file_path);
		std::map<shader_type, std::string>
			process_shader_sources(const std::string& vertex_source,
				const std::string& fragment_source);
		void compile_shader();

		shader_specification m_shader_specification;

		std::map<shader_type, std::string> m_shader_sources;
		std::unordered_map<std::string, unsigned int> m_uniform_locations;
	};
} // namespace rendering
