#define STB_IMAGE_IMPLEMENTATION

#include <Windows.h>
#include "stb_image.h"
#include <stdio.h>
#include <vector>

#include "../Headers/Includes.h"
#include "../Headers/Mesh.h"
#include "../Headers/Material.h"
#include "../Headers/Obj3D.h"
#include "../Headers/Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void processInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale, int idObjeto);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
vector<Obj3D*> listaDeObjs;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camera
float camSpeed = 0.01f;
float dirSpeed = 0.01f;

glm::vec3	cameraPos, 
			cameraFront, 
			cameraUp;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;

float fov = 90.f;
float nearPlane = 0.1f;
float farPlane = 1000.f;

int selecionado = 2;

int main()
{

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Trabalho GB", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;
	glewInit();

	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//read shaders
	GLuint shaderProgram = LoadShader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	glUseProgram(shaderProgram);



	glm::mat4 ProjectionMatrix(1.f);
	glm::mat4 ModelMatrix(1.f);

	cameraPos = glm::vec3(0.0, 0.0, 3.0);
	cameraFront = glm::vec3(0.0, 0.0, -1.0); 
	cameraUp = glm::vec3(0.0, 1.0, 0.0);

	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);

	GLint viewPosLoc = glGetUniformLocation(shaderProgram, "ViewMatrix");
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Obj3D* obj3d = new Obj3D();
	obj3d->Inicializar();
	obj3d->position.x = 12.0f;
	Mesh* mesh = obj3d->processObj("mesa01.obj");
	obj3d->mesh = mesh;
	vector<Material*> materials = obj3d->getMat();
	obj3d->materials = materials;
	listaDeObjs.push_back(obj3d);

	Obj3D* obj3d2 = new Obj3D();
	obj3d2->Inicializar();
	Mesh* mesh2 = obj3d2->processObj("mesa01.obj");
	obj3d2->mesh = mesh2;
	vector<Material*> materials2 = obj3d2->getMat();
	obj3d2->materials = materials2;
	listaDeObjs.push_back(obj3d2);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);
		processInput(window, listaDeObjs[selecionado - 1]->position, listaDeObjs[selecionado - 1]->rotation, listaDeObjs[selecionado - 1]->scale, selecionado);

		int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glm::mat4 view = glm::mat4(1); //matriz identidade;
		//Aqui atualizamos o view com a posição e orientação da câmera atualizados
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewPosLoc, 1, FALSE, glm::value_ptr(view));
		//Atualizar o shader com a posição da câmera
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		ProjectionMatrix = glm::mat4(1.0f);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		for (int i = 0; i < listaDeObjs.size(); i++)
		{
			listaDeObjs[i]->transform();
			GLuint texture;
			for (Group* g : listaDeObjs[i]->mesh->groups) {
				for (Material* m : listaDeObjs[i]->materials) {
					texture = m->texture;
					glUniform3f(glGetUniformLocation(shaderProgram, "Ka"), m->ka->r, m->ka->g, m->ka->b);
					glUniform3f(glGetUniformLocation(shaderProgram, "Kd"), m->kd->r, m->kd->g, m->kd->b);
					glUniform3f(glGetUniformLocation(shaderProgram, "Ks"), m->ks->r, m->ks->g, m->ks->b);
					glUniform1f(glGetUniformLocation(shaderProgram, "Ns"), m->ns);
				}

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);
				glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);

				glBindVertexArray(g->vao);

				glUniform1i((glGetUniformLocation(shaderProgram, "selecionado")), selecionado == i + 1);
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(listaDeObjs[i]->ModelMatrix));

				glDrawArrays(GL_TRIANGLES, 0, g->faces.size() * 6);
			}
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale, int idObjeto)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		selecionado = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		selecionado = 2;
	}

	if (selecionado == idObjeto)
	{

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position.y += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position.y += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position.x -= 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position.y -= 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position.x += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			rotation.y -= 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			rotation.y += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			rotation.x -= 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			rotation.x += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
			rotation.z -= 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS) {
			rotation.z += 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
			scale += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			scale -= 0.001f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos += camSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos -= camSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * dirSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * dirSpeed;

	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Na primeira vez o deslocamento será zero
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//Calculando o deslocamento do Mouse
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	//Amortizando, deixando o movimento mais suave
	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//Atualizando os ângulos de yaw e pitch
	yaw += xoffset;
	pitch += yoffset;

	//Colocando limite no ângulo de pitch (nao é a menina do exorcista)
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//Finalmente, usando os valiosos conhecimentos de Trigonometria para
	//atualizar o vetor front (Viva Pitágoras!)
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}

