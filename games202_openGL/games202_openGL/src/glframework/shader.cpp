#include"shader.h"

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	//����װ��shader�����ַ���������string
	std::string vertexCode;
	std::string fragmentCode;

	//�������ڶ�ȡvs��fs�ļ���inFileStream
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//��֤ifstream���������ʱ������׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//1 ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		//2 ���ļ����������е��ַ������뵽stringStream����
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//3 �ر��ļ�
		vShaderFile.close();
		fShaderFile.close();

		//4 ���ַ�����stringStream���ж�ȡ������ת����code String����
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR: Shader File Error: " << e.what() << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();
	//1 ����Shader����vs��fs��
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//2 Ϊshader��������shader����
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	//3 ִ��shader������� 
	glCompileShader(vertex);
	//���vertex������
	checkShaderErrors(vertex, "COMPILE");
	
	glCompileShader(fragment);
	//���fragment������
	checkShaderErrors(fragment, "COMPILE");
	
	//4 ����һ��Program����
	mProgram = glCreateProgram();

	//6 ��vs��fs����õĽ���ŵ�program���������
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	//7 ִ��program�����Ӳ������γ����տ�ִ��shader����
	glLinkProgram(mProgram);

	//������Ӵ���
	checkShaderErrors(mProgram, "LINK");

	//����
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
Shader::~Shader() {

}

void Shader::begin() {
	glUseProgram(mProgram);
}

void Shader::end() {
	glUseProgram(0);
}

void Shader::checkShaderErrors(GLuint target, std::string type) {
	int success = 0;
	char infoLog[1024];

	if (type == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error: SHADER COMPILE ERROR" << "\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK") {
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
		}
	}
	else {
		std::cout << "Error: Check shader errors Type is wrong" << std::endl;
	}
}


void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(mProgram, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Shader::set3Float(const std::string& name, float v1, float v2, float v3) {
	glUniform3f(glGetUniformLocation(mProgram, name.c_str()), v1, v2, v3);
}

void Shader::set3Float(const std::string& name, glm::vec3 v) {
	glUniform3f(glGetUniformLocation(mProgram, name.c_str()), v.x, v.y, v.z);
}

void Shader::set4Float(const std::string& name, float v1, float v2, float v3, float v4) {
	glUniform4f(glGetUniformLocation(mProgram, name.c_str()), v1, v2, v3, v4);
}

void Shader::set4Float(const std::string& name, aiColor4D color) {
	glUniform4f(glGetUniformLocation(mProgram, name.c_str()), color.r, color.g, color.b, color.a);
}

void Shader::set4Float(const std::string& name, glm::vec4 v) {
	glUniform4f(glGetUniformLocation(mProgram, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setMat4(const std::string& name, glm::mat4 val) {
	glUniformMatrix4fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}