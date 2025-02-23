/*
 * main.cpp
 * 
 * Test VAModel class.
 *
 * 23-07-2023 by madpl (LANA Systems)
 */
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vaModel.hpp"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

sf::Clock clk;
float n = 4.0f;

const glm::mat4 m{std::move(glm::mat4(1))};
glm::mat4 v{};
glm::mat4 p{};
glm::mat4 mv{};

glm::vec3 cameraPos{0.0f, 3.0f, 14.0f};


void reshapeGL(int&& w, int&& h)
{
	if(h <= 0)
		h = 1;
	
	float fAspect = (float)w / (float)h;
	
	glViewport(0, 0, w, h);
	
	p = glm::perspective(glm::radians(55.0f), fAspect, 0.1f, 100.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(p));
}


void initGL()
{
	glClearColor(0.4f, 0.5f, 0.3f, 1.0f);
	
	glFrontFace(GL_CCW);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	
	v = std::move(glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	mv = std::move(v * m);
}


void renderGL(VAModel& model)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float alpha = (360.0f / n) * clk.getElapsedTime().asSeconds();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(mv));

	glColor3f(0.0f, 0.2f, 0.6f);
	
	glRotatef(std::move(alpha), 1.0f, 1.0f, 1.0f);
	model.Draw();
}


int main(void)
{
	// logo
	std::cout << "vaTest by madpl 2023\n";
	
	sf::ContextSettings cs;

	cs.antialiasingLevel = 2;
	cs.depthBits = 24;
	cs.sRgbCapable = false;
	cs.attributeFlags = sf::ContextSettings::Default;

	sf::RenderWindow win(sf::VideoMode(WIDTH, HEIGHT), "", sf::Style::Default, cs);

	win.setTitle("vaTest - LANA Systems 2023");
	win.setPosition(sf::Vector2i(200, 160));
	win.setVerticalSyncEnabled(true);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		return 1;

	VAModel model(std::move("./resources/surface1.obj"));

	initGL();
	reshapeGL(std::move(win.getSize().x), std::move(win.getSize().y));

	bool quit = false;
	while(not quit)
	{
		sf::Event e;
		while(win.pollEvent(e))
		{
			switch(e.type)
			{
				case sf::Event::Closed:
					quit = true;
					break;
					
				case sf::Event::KeyPressed:
					if(e.key.code == sf::Keyboard::Escape)
						quit = true;
					break;
					
				case sf::Event::Resized:
					reshapeGL(std::move(win.getSize().x), std::move(win.getSize().y));
					break;
			}
		}
		
		renderGL(model);
		win.display();
	}

	win.close();

	std::cout << "done.\n";

	return 0;
}
