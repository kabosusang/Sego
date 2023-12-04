#include "Window/Window.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include "Log/Log.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

void SegoWindow::Init()
{
    glfwSetWindowUserPointer(GLwindow, &m_Data);
    
    //GLFW CALLBack
   
    glfwSetKeyCallback(GLwindow,[](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        //key 我们按下的键盘key
        switch (action)
        {
				case GLFW_PRESS:
				{
					KeyDownEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: 
				{
					KeyRealseEvent event(key);
					data.EventCallback(event);
					break;
				}
        }
		if(action == GLFW_PRESS || GLFW_REPEAT ) //处理重复按键
 		{
			KeyDownRepeate event(key,1);
			data.EventCallback(event);
		}



        SG_INFO("Keycode = {0}:",key);
    });


  //Set GLFW CALLBACK
glfwSetWindowSizeCallback(GLwindow, [](GLFWwindow* window, int width, int height)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		data.Width = width;
		data.Height = height;

		WindowChangeSize event(width, height);
		data.EventCallback(event);
	});
glfwSetWindowCloseCallback(GLwindow, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		WindowCloseEvent event;
		data.EventCallback(event);
	});

glfwSetMouseButtonCallback(GLwindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			data.EventCallback(event);
			break;
		}

		}
	});

glfwSetScrollCallback(GLwindow, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrollEvent event(xoffset, yoffset);
		data.EventCallback(event);
		
	});

glfwSetCursorPosCallback(GLwindow, [](GLFWwindow* window, double xpos, double ypos)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMoveEvent event((float)xpos, (float)ypos);
		data.EventCallback(event);
	}
);
glfwSetCharCallback(GLwindow, [](GLFWwindow* window,unsigned int keycode) {

	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	KeyTypedEvent event(keycode);

	data.EventCallback(event);
	}
	
);








}







