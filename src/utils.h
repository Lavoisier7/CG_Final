#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <assert.h>
#include <cmath>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <ctime>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#define MAX_VERTICES_COT 8000
#define MAX_TRIANGLE_COT 8000

using namespace glm;


struct Triangle {
    vec3 triangleVertices[3];
    vec3 triangleNormals[3];
    vec2 triangleTextures[3];
}; 

class Model {
public:
    int triangleCount;
    int verticesCount;
    int normalCot;
    int textCot;
    vec3 centralPoint;
    vec3 vertices_data[MAX_VERTICES_COT];
    vec3 normals_data[MAX_VERTICES_COT];
    vec2 texture_data[MAX_VERTICES_COT];
    int** triangles;
    int** triangle_normals;
    int** triangle_texts;
    std::string loadedPath = "";
    QImage image;

    Model() {}

    ~Model() {
        VAO.destroy();
        VBO.destroy();
    }

    // 显式初始化函数
    void init() {
        // 为模型数据分配内存
        triangles = new int* [MAX_TRIANGLE_COT];
        triangle_normals = new int* [MAX_TRIANGLE_COT];
        triangle_texts = new int* [MAX_TRIANGLE_COT];
        for (int i = 0; i < MAX_TRIANGLE_COT; ++i) {
            triangles[i] = new int[3];
            triangle_normals[i] = new int[3];
            triangle_texts[i] = new int[3];
        }
        // 创建 VAO 和 VBO
        VAO.create();
        VBO.create();
        // 开启深度测试
        glEnable(GL_DEPTH_TEST);
        // 编译和链接着色器
        if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/vertex.glsl")) {
            qDebug() << "Vertex shader compile error:" << shaderProgram.log();
            return;
        }
        if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/fragment.glsl")) {
            qDebug() << "Fragment shader compile error:" << shaderProgram.log();
            return;
        }
        if (!shaderProgram.link()) {
            qDebug() << "Shader program link error:" << shaderProgram.log();
            return;
        }
    }

    // 辅助函数，使用 ID 获取一个三角形
    Triangle getTriangleByID(int id) {
        assert(id < triangleCount);
        int* nowTirVerIDs = triangles[id];
        int* nowTriNormIDs = triangle_normals[id];
        int* nowTriTextIDs = triangle_texts[id];
        vec3 nowTriangleVertices[3];
        vec3 nowTriNorms[3];
        vec2 nowTriTexts[3];
        for (int i = 0; i < 3; i++) {
            nowTriangleVertices[i] = vertices_data[nowTirVerIDs[i]];
            nowTriNorms[i] = normals_data[nowTriNormIDs[i]];
            nowTriTexts[i] = texture_data[nowTriTextIDs[i]];
        }

        Triangle nowTriangle;
        for (int i = 0; i < 3; i++) {
            nowTriangle.triangleVertices[i] = nowTriangleVertices[i];
            nowTriangle.triangleNormals[i] = nowTriNorms[i];
            nowTriangle.triangleTextures[i] = nowTriTexts[i];
        }
        return nowTriangle;
    }

    // 加载模型
    void loadModel(const std::string& path) {
        if (path == loadedPath)
            return;
        loadedPath = path;
        std::ifstream file(path);
        std::string line;
        this->triangleCount = 0;
        this->normalCot = 1;
        this->verticesCount = 1; //obj 文件顶点从1开始计数
        this->textCot = 1;
        this->centralPoint = vec3(0, 0, 0);
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            if (type == "v") {
                if (verticesCount >= MAX_VERTICES_COT)
                    continue;
                vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices_data[verticesCount] = vertex;
                centralPoint += vertex;
                verticesCount++;
            }
            else if (type == "vn") {
                if (normalCot >= MAX_VERTICES_COT)
                    continue;
                vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                normals_data[normalCot] = vertex;
                normalCot++;
            }
            else if (type == "vt") {
                if (textCot >= MAX_VERTICES_COT)
                    continue;
                vec2 text;
                iss >> text.x >> text.y;
                texture_data[textCot] = text;
                textCot++;
            }
            else if (type == "f") {
                if (triangleCount >= MAX_TRIANGLE_COT)
                    continue;
                for (int i = 0; i < 3; ++i) {
                    char slash;
                    int vertexIndex, normalIndex, textIndex;
                    iss >> vertexIndex >> slash >> textIndex >> slash >> normalIndex;
                    triangles[triangleCount][i] = vertexIndex;
                    triangle_texts[triangleCount][i] = textIndex;
                    triangle_normals[triangleCount][i] = normalIndex;
                }
                triangleCount++;
            }
        }
        centralPoint /= verticesCount;
        std::cout << "read " << triangleCount << " triangles" << std::endl;
        // 加载完成后，绑定数据
        bindData();
    }
    
    // 设置模型的纹理图片
    void setImage(const std::string& path) {
        const char*  texturePath = path.c_str();
        image = QImage(texturePath);
        if (image.isNull()) {
            qDebug() << "Failed to load texture image";
            return;
        }
    }

    // 绘制模型
    void draw(
        QVector3D lightPosition, 
        QVector3D camPosition,
        QMatrix4x4 viewMatrix,
        QMatrix4x4 projMatrix) {
        // 设置 uniform 变量
        shaderProgram.bind();
        shaderProgram.setUniformValue("viewMatrix", viewMatrix);
        shaderProgram.setUniformValue("projMatrix", projMatrix);
        shaderProgram.setUniformValue("lightPosition", lightPosition);
        shaderProgram.setUniformValue("camPosition", camPosition);
        QOpenGLTexture texture(image.mirrored());
        texture.bind(0);
        shaderProgram.setUniformValue("texture_image", 0);

        // 绘制物体
        VAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);

        shaderProgram.release();
        VAO.release();
    }

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO;

    void bindData() {
        // 申请内存储存每个三角形顶点的位置和法向量
        // 每个三角形有三个顶点，每个顶点有包括位置坐标、法向量和纹理坐标，大小分别为3、3、2
        // 故一个三角形中共有 3 * (3 + 3 + 2) = 24 个值
        int totalVertexCount = triangleCount * 24;
        GLfloat* Vertices = new GLfloat[totalVertexCount];
        for (int i = 0; i < triangleCount; i++) {
            Triangle nowTriangle = getTriangleByID(i);
            int start = i * 24;
            // 将每个向量的值逐个放置到对应的地址位置上
            // 存放顺序为：点坐标、法向量坐标、纹理坐标
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    Vertices[start + k] = nowTriangle.triangleVertices[j][k];
                    Vertices[start + 3 + k] = nowTriangle.triangleNormals[j][k];
                    if (k < 2)
                        Vertices[start + 6 + k] = nowTriangle.triangleTextures[j][k];
                }
                start += 8;
            }
        }
        // 将数据绑定到 VBO 上
        VAO.bind();
        VBO.bind();
        VBO.allocate(Vertices, sizeof(GLfloat) * totalVertexCount);
        // 指明数据的存取方式
        shaderProgram.bind();
        shaderProgram.setAttributeBuffer("pos", GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        shaderProgram.enableAttributeArray("pos");
        shaderProgram.setAttributeBuffer("normal", GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
        shaderProgram.enableAttributeArray("normal");
        shaderProgram.setAttributeBuffer("inTex", GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
        shaderProgram.enableAttributeArray("inTex");
        // 释放对象和内存
        VBO.release();
        VAO.release();
        shaderProgram.release();
        delete[] Vertices;
    }
};

#endif // UTILS.H