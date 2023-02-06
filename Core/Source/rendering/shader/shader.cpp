#include "pch.h"

#include "rendering/shader/shader.h"
#include <glm/gtc/type_ptr.hpp>

namespace rendering {
	shader::shader(const shader_specification& specification) {
		m_shader_specification = specification;
		std::cout << "Loading shader" << std::endl;
		// Parse both vertex and fragment shaders sources from file.
		const std::string& vertexSource =
			parse_shader_contents_from_file(specification.vertex_path);
		const std::string& fragmentSource =
			parse_shader_contents_from_file(specification.fragment_path);

		m_shader_sources = process_shader_sources(vertexSource, fragmentSource);
		compile_shader();
	}

	shader::~shader() { glDeleteProgram(m_object_handle); }

	void shader::bind() { glUseProgram(m_object_handle); }

	void shader::un_bind() { glUseProgram(0); }

	void shader::set_int(const std::string& uniform, int value) {
		glUniform1i(get_uniform_location(uniform), value);
	}

	void shader::set_mat4(const std::string& uniform, const glm::mat4& value)
	{
		glUniformMatrix4fv(get_uniform_location(uniform), 1, GL_FALSE, glm::value_ptr(value));
	}

	unsigned int shader::get_uniform_location(const std::string& uniform) {
		if (m_uniform_locations.contains(uniform)) {
			return m_uniform_locations[uniform];
		}
		// Get uniform location.
		const int location = glGetUniformLocation(m_object_handle, uniform.c_str());
		// Store uniform location in map.
		m_uniform_locations[uniform] = location;
		return location;
	}

	shared<shader> shader::create(const shader_specification& specification) {
		return create_shared<shader>(specification);
	}

	std::string
		shader::parse_shader_contents_from_file(const std::string& shader_file_path) {
		std::string parsedContents;
		std::ifstream in(shader_file_path, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			const size_t size = in.tellg();
			// Valid size.
			if (size != -1) {
				parsedContents.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(parsedContents.data(), size);
				in.close();
			}
			else {
			}
		}
		else {
		}
		return parsedContents;
	}

	std::map<shader_type, std::string>
		shader::process_shader_sources(const std::string& vertex_source,
			const std::string& fragment_source) {
		std::map<shader_type, std::string> shaderSources = {};
		shaderSources[fragment] = fragment_source;
		shaderSources[vertex] = vertex_source;

		return shaderSources;
	}

	void shader::compile_shader() {
		const uint32_t shaderProgram = glCreateProgram();
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.resize(2);
		int shaderIndex = 0;
		// Iterate through the shader sources.
		for (auto& shader : m_shader_sources) {
			const shader_type shaderType = shader.first;
			const std::string& shaderSource = shader.second;

			// Creating the program
			const GLuint open_gl_shader = glCreateShader(shaderType);
			// Assigning source
			const GLchar* sourceCStr = shaderSource.c_str();
			glShaderSource(open_gl_shader, 1, &sourceCStr, nullptr);

			// Compiling the shader
			glCompileShader(open_gl_shader);

			// Error handling
			GLint success = 0;
			glGetShaderiv(open_gl_shader, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(open_gl_shader, GL_INFO_LOG_LENGTH, &maxLength);

				// Get error log.
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(open_gl_shader, maxLength, &maxLength, infoLog.data());

				// Delete the shader.
				glDeleteShader(open_gl_shader);

				// Print error log.
				// logger::error(
				//	"open_gl_shader::compile_shader | Shader compilation failed: " +
				// std::string( 		infoLog.begin(), infoLog.end()));
				std::cout << "compile_shader | Shader compilation failed: " + std::string(infoLog.begin(), infoLog.end()) << std::endl;
				break;
			}

			//	logger::info("open_gl_shader::compile_shader | Compiled shader type: " +
			// get_shader_type_to_string(shaderType));

			// Attach
			glAttachShader(shaderProgram, open_gl_shader);
			glShaderIDs[shaderIndex] = open_gl_shader;
			shaderIndex++;
		}
		// Assign program ID
		m_object_handle = shaderProgram;

		// Link the program
		glLinkProgram(m_object_handle);

		// Error handling
		GLint success = 0;
		glGetProgramiv(m_object_handle, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_object_handle, GL_INFO_LOG_LENGTH, &maxLength);

			// Getting the error
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_object_handle, maxLength, &maxLength, infoLog.data());

			// Delete the program
			glDeleteProgram(m_object_handle);

			// Delete shaders.
			for (const auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			std::cout << "compile_shader | Shader linking failed: " + std::string(infoLog.begin(), infoLog.end()) << std::endl;

			// Print error log.
			// logger::error(
			//"open_gl_shader::compile_shader | Shader linking failed: " +
			// std::string(infoLog.begin(), infoLog.end()));
			return;
		}

		// logger::info("open_gl_shader::compile_shader | Shader Compiled
		// Successfully!");
		std::cout << "Shader compiled successfully!" << std::endl;

		// Cleanup.
		for (const auto id : glShaderIDs) {
			glDetachShader(m_object_handle, id);
			glDeleteShader(id);
		}
	}
}
