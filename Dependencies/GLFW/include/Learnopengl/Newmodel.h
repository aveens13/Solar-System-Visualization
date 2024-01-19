#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <Json/json.h>
#include <Learnopengl/Camera.h>
#include "mesh.h"

using json = nlohmann::json;

class Model {
public:
	Model(const char* file) {
		std::string text = get_file_contents(file);
		JSON = json::parse(text);
		this->file = file;
		data = getData();
	};
	void Draw(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	std::vector<unsigned char> getData() {
		std::string bytesText;
		std::string uri = JSON["buffers"][0]["uri"];

		std::string fileStr = std::string(file);
		std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
		bytesText = get_file_contents((fileDirectory + uri).c_str());

		std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
		return data;
	}
};
#endif // !MODEL_CLASS_H

