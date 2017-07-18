#pragma once

#include <ncl/gl/Scene.h>

using namespace std;
using namespace ncl;
using namespace gl;
using namespace glm;

class ComputeScene : public Scene {
public:
	ComputeScene() :Scene("compute scene") {
		_useImplictShaderLoad = true;
	}

	virtual void init() override {
		initQuad();
		initImage();
	}

	void initImage() {
		glDeleteTextures(1, &output_image);
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &output_image);
		glBindTexture(GL_TEXTURE_2D, output_image);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, _width, _height);
	}

	void aboutProg() {
		Logger logger = Logger::get("Compute Scene");
		stringstream ss;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxGroupSize);
		ss << "max group workd size: (" << maxGroupSize << ", ";
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxGroupSize);
		ss << maxGroupSize << ", ";
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxGroupSize);
		ss << maxGroupSize << ")" << endl;
		logger.info(ss.str());
	}

	void initQuad() {
		Mesh mesh;
		mesh.positions = {
			{-1.0f, -1.0f, 0.5f},
			{1.0f, -1.0f, 0.5f},
			{1.0f,  1.0f, 0.5f},
			{-1.0f,  1.0f, 0.5f}
		};
		mesh.primitiveType = GL_TRIANGLE_FAN;
		quad = new ProvidedMesh(mesh);
	}

	virtual void display() override {
		shader("compute")([&](Shader& s) {
			glBindImageTexture(0, output_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			s.sendUniform1ui("output_image", output_image);
			glDispatchCompute(_width/8, _height/16, 1);
		});

		shader("render")([&](Shader& s) {
			glBindTexture(GL_TEXTURE_2D, output_image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			quad->draw(s);
		});
	}

	virtual void resized() override {
		Logger::get("Compute Scene").info("Window resized");
		initImage();
	}

private:
	GLuint output_image;
	int maxGroupSize;
	Shape* quad;
	
};