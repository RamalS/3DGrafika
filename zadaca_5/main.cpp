#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <limits>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"

#include "CircleRenderer.h"
#include "LineRenderer.h"

using namespace std;
using namespace glm;

int Factorial(int x)
{
	if (x > 1)
		return x * Factorial(x - 1);
	else
		return 1;
}

int BinomialCoefficiant(int n, int k) 
{
	return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

struct BezierCurve 
{
	vec2 P0, P1, P2, P3;
  	vec2* holding = nullptr;
  	vector<vec2> points;
	
  	BezierCurve(vec2 P0, vec2 P1, vec2 P2, vec2 P3): P0(P0), P1(P1), P2(P2), P3(P3) {}
  	vector<vec2>& getCurve() { return points; }
  
	void updateCurve(float step) 
  	{
		for(float i = 0; i <= 1.0; i += step)
		{
      		points.push_back((1 - i) * (1 - i) *( 1 - i) * P0 + 3 * (1 - i) * (1 - i) * i * P1 + 3 * (1 - i) * i * i * P2 + i * i * i * P3);
    	}
  	}

  	void ClearPoints() 
  	{
    	points.clear();
  	}
};

BezierCurve bezier(vec2(50.0, 50.0), vec2(910.0, 50.0), vec2(50.0, 490.0), vec2(910.0, 490.0));
vec2* holding = nullptr;

void mouse_callback(GLFWwindow* window, int button, int action, int mods) 
{
  if (button == GLFW_MOUSE_BUTTON_LEFT) 
  {
	if(GLFW_PRESS == action)
	{
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

      	if(sqrt((x - bezier.P0.x) * (x - bezier.P0.x) + (y - bezier.P0.y) * (y - bezier.P0.y)) < 5) holding = &bezier.P0;
      	else if(sqrt((x - bezier.P1.x) * (x - bezier.P1.x) + (y - bezier.P1.y) * (y - bezier.P1.y)) < 5) holding = &bezier.P1;
      	else if(sqrt((x - bezier.P2.x) * (x - bezier.P2.x) + (y - bezier.P2.y) * (y - bezier.P2.y)) < 5) holding = &bezier.P2;
      	else if(sqrt((x - bezier.P3.x) * (x - bezier.P3.x) + (y - bezier.P3.y) * (y - bezier.P3.y)) < 5) holding = &bezier.P3;
    }
    else if(GLFW_RELEASE == action) 
	{
      holding = nullptr;
    }
  }
}

void cursor_callback(GLFWwindow* window, double x, double y) 
{
  if(holding != nullptr)
  {
    holding->x = x;
    holding->y = y;
    bezier.ClearPoints();
    bezier.updateCurve(0.0001);
  }
}

int main ()
{
	GLFWwindow* window;

	if (!glfwInit())
    {
		std::cout << "Error : could not initilize GLFW";
    }
    
	int width = 960;
	int height = 540;
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(width, height, "Bezier curve", NULL, NULL);
	if (!window)
	{
		std::cout << "Error : could not create window";
		glfwTerminate();
	}
    
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Error : could not initilize Glad";

	glfwSwapInterval(1);

	InitCircleRendering(32);
	InitLineRendering();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bezier.updateCurve(0.0001);

	glfwSetMouseButtonCallback(window, mouse_callback);
  	glfwSetCursorPosCallback(window, cursor_callback);
	
	while (!glfwWindowShouldClose(window)) 
	{
	   glClear(GL_COLOR_BUFFER_BIT);

    	RenderCircle(bezier.P0, 5);
    	RenderCircle(bezier.P1, 5);
    	RenderCircle(bezier.P2, 5);
    	RenderCircle(bezier.P3, 5);

    	RenderLine(bezier.getCurve());

    	glfwSwapBuffers(window);
    	glfwPollEvents();
  	}
	
	return 0;
}