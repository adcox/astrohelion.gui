/**
 *  @file 
 *	@brief 
 *
 *	@author Andrew Cox
 *	@version 
 *	@copyright GNU GPL v3.0
 */
 
/*
 *	Astrohelion 
 *	Copyright 2016, Andrew Cox; Protected under the GNU GPL v3.0
 *	
 *	This file is part of Astrohelion
 *
 *  Astrohelion is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Astrohelion is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Astrohelion.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Window.hpp"

#ifdef _WIN32
	#undef APIENTRY
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define GLFW_EXPOSE_NATIVE_WGL
	#include <GLFW/glfw3native.h>
#endif

#include <stdexcept>

#include <iostream>

namespace astrohelion{
namespace gui{
//-----------------------------------------------------
//      *structors
//-----------------------------------------------------

Window::Window(){

}//====================================================

/**
 *  @brief Construct a window object
 *  @details This function does not create the GLFW window; see Window::create
 *  for that functionality
 * 
 *  @param w window width, pixels
 *  @param h window height, pixesl
 */
Window::Window(int w, int h){
	width = w;
	height = h;
}//====================================================

/**
 *  @brief Construct a window object
 *  @details This function creates a GLFW window. Note that this function
 *  tells GLFW to set this window as the current context.
 * 
 *  @param w window width, pixels
 *  @param h window height, pixesl
 *  @param title window title
 *  @param pMonitor Monitor pointer - see GLFW Window docs for more infor
 *  @param share another window to share resources with (optional)
 */
Window::Window(int id, int w, int h, const char* title, GLFWmonitor *pMonitor, Window* share){
	this->id = id;
	width = w;
	height = h;
	this->title = title;

	create(title, pMonitor, share);
}//====================================================

Window::~Window(){}

Window::Window(const Window &w){
	copyMe(w);
}//====================================================

/**
 *  @brief Create and initialize a GLFW window.
 *  @details Note that this function tells GLFW to set this window as 
 *  the current context.
 * 
 *  @param w window width, pixels
 *  @param h window height, pixesl
 *  @param title window title
 *  @param pMonitor Monitor pointer - see GLFW Window docs for more infor
 *  @param share another window to share resources with (optional)
 */
void Window::create(const char* title, GLFWmonitor* pMonitor, Window* share){
	this->title = title;
	
	if(this->id < 0){
		throw std::runtime_error("Window::create: Must set Window ID before creating the window!");
	}

	if(share){
		pWindow = glfwCreateWindow(width, height, title, pMonitor, share->getWindowPtr());
	}else{
		pWindow = glfwCreateWindow(width, height, title, pMonitor, nullptr);
	}

	if(pWindow == nullptr){
		throw std::runtime_error("Window::create: Failed to create GLFW window");
	}

	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		throw std::runtime_error("Window::create: Failed to initialize GLEW");
	}

	glfwGetFramebufferSize(pWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);	// make the buffer take up the entire screen

    if(id == 0)
    	imguiContext = ImGui::GetCurrentContext();
}//====================================================

bool Window::ImGui_init(){
    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;	// Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    io.RenderDrawListsFn = nullptr;
    // io.RenderDrawListsFn = ImGui_ImplGlfwGL3_RenderDrawLists;       	// Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
    // io.SetClipboardTextFn = ImGui_ImplGlfwGL3_SetClipboardText;			// Could put these functions in the main script, use getCurrentContext() to determine which window to use	
    // io.GetClipboardTextFn = ImGui_ImplGlfwGL3_GetClipboardText;

#ifdef _WIN32
    io.ImeWindowHandle = glfwGetWin32Window(pWindow);
#endif

    return true;
}//====================================================

bool Window::ImGui_createDeviceObjects(){
    // Backup GL state
    GLint last_texture, last_array_buffer, last_vertex_array;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

    glGenBuffers(1, &imgui_VBO);
    glGenBuffers(1, &imgui_EBO);

    unsigned int shaderID = appRM->getShader("imgui").getID();
    unsigned int g_AttribLocationPosition = glGetAttribLocation(shaderID, "Position");
    unsigned int g_AttribLocationUV = glGetAttribLocation(shaderID, "UV");
    unsigned int g_AttribLocationColor = glGetAttribLocation(shaderID, "Color");

    glGenVertexArrays(1, &imgui_VAO);
    glBindVertexArray(imgui_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, imgui_VBO);
    glEnableVertexAttribArray(g_AttribLocationPosition);
    glEnableVertexAttribArray(g_AttribLocationUV);
    glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

    createFontsTexture();

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindVertexArray(last_vertex_array);

    return true;
}//====================================================

bool Window::createFontsTexture(){
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int w, h;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &imgui_fontTexture);
    glBindTexture(GL_TEXTURE_2D, imgui_fontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (void *)(intptr_t)imgui_fontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}//====================================================

void Window::destruct(){
	if (imgui_VAO) glDeleteVertexArrays(1, &imgui_VAO);
    if (imgui_VBO) glDeleteBuffers(1, &imgui_VBO);
    if (imgui_EBO) glDeleteBuffers(1, &imgui_EBO);

    imgui_VAO = imgui_VBO = imgui_EBO = 0;

    if (imgui_fontTexture){
        glDeleteTextures(1, &imgui_fontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        imgui_fontTexture = 0;
    }
    ImGui::Shutdown();
}//====================================================

//-----------------------------------------------------
//      Operators
//-----------------------------------------------------

Window& Window::operator =(const Window &w){
	copyMe(w);
	return *this;
}//====================================================

//-----------------------------------------------------
//      Event Loop Functions
//-----------------------------------------------------

void Window::init(){}

void Window::computeMetrics(){
	GLfloat currentFrame = glfwGetTime();
	frame_dt = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = frame_dt;
}//====================================================

void Window::update(){}

void Window::preDraw(){
	if (!imgui_fontTexture)
        ImGui_createDeviceObjects();

    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    // TODO - Move this to window_resized event handler
    io.DisplaySize = ImVec2((float)width, (float)height);
    io.DisplayFramebufferScale = ImVec2(width > 0 ? ((float)bufferWidth / width) : 0, height > 0 ? ((float)bufferHeight / height) : 0);

    // Setup inputs
    // TODO - may have some issues with my own mouse button listening and may need to implement similar code
    for (int i = 0; i < 3; i++){
        io.MouseDown[i] = bMousePressed[i] || glfwGetMouseButton(pWindow, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        bMousePressed[i] = false;
    }

    io.MouseWheel = mouse_scrollYOffset;
    mouse_scrollYOffset = 0.0f;

    // Hide OS mouse cursor if ImGui is drawing it
    glfwSetInputMode(pWindow, GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);

    // Start the frame
    ImGui::NewFrame();
}//====================================================

void Window::draw(){
	
}//====================================================

void Window::postDraw(){
	ImGui::Render();
	ImGui_RenderDrawLists(ImGui::GetDrawData());
}//====================================================

/**
 *  @brief Main rendering function to provide to ImGui
 *  @details This is the main rendering function that you 
 *  have to implement and provide to ImGui (via setting up 
 *  'RenderDrawListsFn' in the ImGuiIO structure).
 *  
 *  If text or lines are blurry when integrating ImGui in your engine:
 * 	- in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
 * 	
 *  @param draw_data ImGui passes in data here to be drawn
 */
void Window::ImGui_RenderDrawLists(ImDrawData* draw_data){
	if(!appRM)
		throw std::runtime_error("Window::ImGui_RenderDrawLists: Must initialize Resource Manager object before calling this function!");

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    
    if (fb_width == 0 || fb_height == 0)
        return;
    
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    GLint last_program; 				glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; 				glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_active_texture; 			glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
    GLint last_array_buffer; 			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer; 	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    GLint last_vertex_array; 			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_blend_src; 				glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
    GLint last_blend_dst; 				glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
    GLint last_blend_equation_rgb; 		glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
    GLint last_blend_equation_alpha; 	glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
    GLint last_viewport[4]; 			glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLboolean last_enable_blend = 		glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = 	glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = 	glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    glm::mat4 ortho (1.0f);
    ortho[0][0] = 2.0f/io.DisplaySize.x;
    ortho[1][1] = 2.0f/-io.DisplaySize.y;
    ortho[2][2] = -1.0f;
    ortho[3][0] = -1.0f;
    ortho[3][1] = 1.0f;

    Shader &shade = appRM->getShader("imgui");
    
    shade.setInteger("Texture", 0, true);	// true: use this shader
    shade.setMatrix4("ProjMtx", ortho);
    
    glBindVertexArray(imgui_VAO);

    for (int n = 0; n < draw_data->CmdListsCount; n++){
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, imgui_VBO);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imgui_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

        for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++){
            
            if (pcmd->UserCallback){
                pcmd->UserCallback(cmd_list, pcmd);
            }else{
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
            }

            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram(last_program);
    glActiveTexture(last_active_texture);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindVertexArray(last_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFunc(last_blend_src, last_blend_dst);

    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}//====================================================

//-----------------------------------------------------
//      Event Handling Functions
//-----------------------------------------------------

void Window::handleCharCallback(unsigned int c){
    ImGuiIO& io = ImGui::GetIO();
    if (c > 0 && c < 0x10000)
        io.AddInputCharacter(static_cast<unsigned short>(c));
}//====================================================

void Window::handleKeyEvent(int key, int scancode, int action, int mods){
	(void)scancode;
	(void)mods;		// Modifiers are not reliable across systems

	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GL_TRUE);

	ImGuiIO& io = ImGui::GetIO();

	if(key >= 0 && key < 1024){
		if(action == GLFW_PRESS){
	        bKeyPressed[key] = true;
	        io.KeysDown[key] = true;
		}else if(action == GLFW_RELEASE){
	        bKeyPressed[key] = false;
	        io.KeysDown[key] = false;
	    }
	}

	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	// std::cout << title << ": Key " << key << " action!" << std::endl;
}//====================================================

void Window::handleMouseMoveEvent(double xpos, double ypos){
    ImGuiIO& io = ImGui::GetIO();	// TODO - Is getting a copy of this every time the mouse moves a good idea? The ImGui code was originally in the draw function
	if(glfwGetWindowAttrib(pWindow, GLFW_FOCUSED)){
		if(bMouse_firstFrame){
			mouse_lastX = xpos;
			mouse_lastY = ypos;
			bMouse_firstFrame = false;
		}

		io.MousePos = ImVec2(static_cast<float>(xpos), static_cast<float>(ypos));

		mouse_xOffset = xpos - mouse_lastX;
		mouse_yOffset = mouse_lastY - ypos;	// Reversed since y-coordinates go from bottom to top

		mouse_lastX = xpos;
		mouse_lastY = ypos;

		std::cout << title << ": Mouse moved!" << std::endl;
	}else{
		io.MousePos = ImVec2(-1,-1);
	}
}//====================================================

void Window::handleMouseButtonEvent(int button, int action, int mods){
	(void) mods;

	if(action == GLFW_PRESS && button >= 0 && button < 3)
		bMousePressed[button] = true;
}//====================================================

void Window::handleMouseScrollEvent(double xoffset, double yoffset){
	mouse_scrollXOffset = xoffset;
	mouse_scrollYOffset = yoffset;	// Use fractional mouse wheel, 1.0 unit 5 lines.

	std::cout << title << ": Mouse scrolled!" << std::endl;
}//====================================================

//-----------------------------------------------------
//      Set and Get Functions
//-----------------------------------------------------

int Window::getID() const { return id; }

GLFWwindow* Window::getWindowPtr(){ return pWindow; }

void Window::setID(int id){ this->id = id; }

const char* Window::getClipboardText() const{
    return glfwGetClipboardString(pWindow);
}//====================================================

void Window::setClipboardText(const char* text){
    glfwSetClipboardString(pWindow, text);
}//====================================================

//-----------------------------------------------------
//      Utility Functions
//-----------------------------------------------------

void Window::copyMe(const Window &w){
	ResourceUser::copyMe(w);

	pWindow = w.pWindow;
	id = w.id;
	title = w.title;

	imguiContext = w.imguiContext;
	imgui_VAO = w.imgui_VAO;
	imgui_VBO = w.imgui_VBO;
	imgui_EBO = w.imgui_EBO;
	imgui_fontTexture = w.imgui_fontTexture;

	frame_dt = w.frame_dt;
	lastFrameTime = w.lastFrameTime;

	width = w.width;
	height = w.height;
	bufferWidth = w.bufferWidth;
	bufferHeight = w.bufferHeight;

	std::copy(w.bKeyPressed, w.bKeyPressed+1024, bKeyPressed);
	std::copy(w.bMousePressed, w.bMousePressed+3, bMousePressed);
	bMouse_firstFrame = w.bMouse_firstFrame;

	mouse_lastX = w.mouse_lastX;
	mouse_lastY = w.mouse_lastY;
	mouse_xOffset = w.mouse_xOffset;
	mouse_yOffset = w.mouse_yOffset;
	mouse_scrollXOffset = w.mouse_scrollXOffset;
	mouse_scrollYOffset = w.mouse_scrollYOffset;

}//====================================================

}// End of gui namespace
}// End of Astrohelion namespace