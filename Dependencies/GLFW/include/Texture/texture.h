#ifndef TEXTURE_H
#define TEXTURE_H
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <iostream>
#include <string>
#include "stb_image.h"

class Texture {
	public:
		GLuint ID;

		Texture(const char* texturePath,const bool verticalBool) {
			glGenTextures(1, &ID);
			glBindTexture(GL_TEXTURE_2D, ID);

            //Texture Wrapping 
            // What if the texture size is not enough

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x-axis texture wrapping
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y-axis texture wrapping

            //Texture filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //Texture loading
            int width, height, nrchannels; //nrchannles is the number of color channels
            stbi_set_flip_vertically_on_load(verticalBool);
            unsigned char* data = stbi_load(texturePath, &width, &height, &nrchannels, 0);

            //Check if my image has 3 or 4 color channels
            int channel = (nrchannels==4)? GL_RGBA: GL_RGB;
            std::cout <<"Width: "<< width << std::endl;
            std::cout <<"Height: "<< height << std::endl;
            

            //Now we can generate a texture using image loaded
            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, channel, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D); //Generate all required mipmaps for current texture
            }
            else {
                std::cout << "Failed to load texture." << std::endl;
            }
            //Free the Image memory
            stbi_image_free(data);
		}

        //Cubemap
        // loads a cubemap texture from 6 individual texture faces
        // order:
        // +X (right)
        // -X (left)
        // +Y (top)
        // -Y (bottom)
        // +Z (front) 
        // -Z (back)
        // -------------------------------------------------------

        Texture(const std::vector<std::string>& faces, const bool verticalBool) {
            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

            int width, height, nrChannels;
            for (unsigned int i = 0; i < faces.size(); i++)
            {
                stbi_set_flip_vertically_on_load(verticalBool);
                unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                }
                else
                {
                    std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                    stbi_image_free(data);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }

        Texture(string const& path, const std::string& directory) {
            std::string filename = std::string(path);
            filename = directory + '/' + filename;

            glGenTextures(1, &ID);

            int width, height, nrComponents;
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, ID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }
    }

};

#endif // !TEXTURE_H
