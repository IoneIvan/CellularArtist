#include<iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb\stb_image.h>
#include <stb\stb_image_write.h>

#include "shaderClass.h"
#include "Texture.h"

const int WIDTH = 1024;
const int HEIGHT = 1024;
void createRandomTexture(GLuint& textureID);
void createBlackTexture(GLuint& textureID);
std::string removeQuotes(const std::string& str);
void createTextureFromFile(GLuint& textureID, const char* imagePath);
void createTextureFromFile(GLuint& textureID);

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Red Pixels", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    gladLoadGL();



    // Define vertices for a full-screen quad
    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    //widnow settings
    glViewport(0, 0, WIDTH, HEIGHT);


    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shColor  ("default.vert", "colorUpdate.frag");
    Shader shGene   ("default.vert", "geneUpdate.frag");
    Shader shRand   ("default.vert", "randomUpdate.frag");

    // Create and bind a vertex array object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind a vertex buffer object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify the vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    Texture promptImage(WIDTH, HEIGHT);
    Texture colorImage[2]{ Texture(WIDTH, HEIGHT), Texture(WIDTH, HEIGHT) };
    Texture geneImage[2]{ Texture(WIDTH, HEIGHT), Texture(WIDTH, HEIGHT) };



    createTextureFromFile(promptImage.texture);

    createBlackTexture(colorImage[0].texture);
    createBlackTexture(colorImage[0].framebuffer);
    createBlackTexture(colorImage[1].texture);
    createBlackTexture(colorImage[1].framebuffer);

    createBlackTexture(geneImage[0].texture);
    createBlackTexture(geneImage[0].framebuffer);
    createBlackTexture(geneImage[1].texture);
    createBlackTexture(geneImage[1].framebuffer);



    int count = 0;
    int currTexture = 0;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);
    float randomFloat;
    while (!glfwWindowShouldClose(window)) {

        // Shader Randomize
        shRand.Activate();
        randomFloat = distr(eng);
        glUniform1i(glGetUniformLocation(shColor.ID, "texture"), 0);
        glUniform1f(glGetUniformLocation(shRand.ID, "time"), randomFloat);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, geneImage[1 - currTexture].texture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindFramebuffer(GL_FRAMEBUFFER, geneImage[currTexture].framebuffer);
        glBindTexture(GL_TEXTURE_2D, geneImage[currTexture].texture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, WIDTH, HEIGHT, 0);
        std::swap(geneImage[0], geneImage[1]);

        // Shader Reproduction
        shGene.Activate();
        // Set the active texture unit for colorTexture
        glUniform1i(glGetUniformLocation(shGene.ID, "colorTexture"), 0);
        // Set the active texture unit for geneTexture
        glUniform1i(glGetUniformLocation(shGene.ID, "geneTexture"), 1);
        // Set the active texture unit for sampleTexture
        glUniform1i(glGetUniformLocation(shGene.ID, "sampleTexture"), 2);

        // Bind framebuffer for colorTexture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Bind textures for colorTexture, brain, and anotherTexture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorImage[1 - currTexture].texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, geneImage[1 - currTexture].texture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, promptImage.texture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // Copy result to the other texture
        glBindFramebuffer(GL_FRAMEBUFFER, geneImage[currTexture].framebuffer);
        glBindTexture(GL_TEXTURE_2D, geneImage[currTexture].texture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, WIDTH, HEIGHT, 0);
        std::swap(geneImage[0], geneImage[1]);

        // Shader Color
        shColor.Activate();
        // Set the active texture unit for colorTexture
        glUniform1i(glGetUniformLocation(shColor.ID, "colorTexture"), 0);
        glUniform1i(glGetUniformLocation(shColor.ID, "geneTexture"), 1);

        // Bind framebuffer for colorTexture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Bind textures for colorTexture, brain, and anotherTexture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorImage[1 - currTexture].texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, geneImage[1 - currTexture].texture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // Render for colorTexture
        // Copy result to the other texture
        glBindFramebuffer(GL_FRAMEBUFFER, colorImage[currTexture].framebuffer);
        glBindTexture(GL_TEXTURE_2D, colorImage[currTexture].texture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, WIDTH, HEIGHT, 0);
        std::swap(colorImage[0], colorImage[1]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	return 0;
}

void createRandomTexture(GLuint& textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Create an array of random values for each pixel (for RGB components)
    std::vector<GLubyte> randomData(WIDTH * HEIGHT * 3);
    for (int i = 0; i < WIDTH * HEIGHT * 3; i += 3)
    {
        randomData[i] = static_cast<GLubyte>(rand() % 256); // Red component
        randomData[i + 1] = static_cast<GLubyte>(rand() % 256); // Green component
        randomData[i + 2] = static_cast<GLubyte>(rand() % 256); // Blue component
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, randomData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);


}

std::string removeQuotes(const std::string& str)
{
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '\"'), result.end());
    return result;
}
void createTextureFromFile(GLuint& textureID, const char* imagePath)
{
    int width, height, numChannels;
    stbi_set_flip_vertically_on_load(true);  // Flip the image vertically (optional)

    // Load the image from file
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &numChannels, STBI_rgb_alpha); // Use STBI_rgb_alpha for 4 channels (RGB + Alpha)
    if (!imageData)
    {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        createRandomTexture(textureID);
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); // Use GL_RGBA and GL_UNSIGNED_BYTE

    // Set texture parameters for pixel-perfect representation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Free the image data
    stbi_image_free(imageData);

    glBindTexture(GL_TEXTURE_2D, 0);
}
void createTextureFromFile(GLuint& textureID)
{
    // Get the image path from the user
    std::cout << "Enter the path to the image: ";
    std::string imagePath;
    std::getline(std::cin, imagePath);
    // Remove quotes from the entered path
    imagePath = removeQuotes(imagePath);
    // Convert std::string to const char*
    const char* imagePathCStr = imagePath.c_str();
    createTextureFromFile(textureID, imagePathCStr);
}
void createBlackTexture(GLuint& textureID) {
    // Generate a texture ID
    glGenTextures(1, &textureID);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Create a black texture
    std::vector<GLubyte> blackData(WIDTH * HEIGHT * 4, 0);  // 4 channels for RGBA

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Provide the data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, blackData.data());

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}