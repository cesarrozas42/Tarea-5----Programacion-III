#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <stdexcept>
#include <optional>
#include <iomanip>

class Window {
public:
    virtual ~Window() = default;
    virtual void draw() const = 0;
};

class Button {
public:
    virtual ~Button() = default;
    virtual void draw() const = 0;
};

class Shader {
public:
    virtual ~Shader() = default;
    virtual void compile() const = 0;
};

class OpenGLWindow : public Window {
public:
    void draw() const override {
        std::cout << "[OpenGL] Dibujando ventana con OpenGL.\n";
    }
};

class VulkanWindow : public Window {
public:
    void draw() const override {
        std::cout << "[Vulkan] Dibujando ventana con Vulkan.\n";
    }
};

class OpenGLButton : public Button {
public:
    void draw() const override {
        std::cout << "[OpenGL] Dibujando boton con estilo OpenGL.\n";
    }
};

class VulkanButton : public Button {
public:
    void draw() const override {
        std::cout << "[Vulkan] Dibujando boton con estilo Vulkan.\n";
    }
};

class OpenGLShader : public Shader {
public:
    void compile() const override {
        std::cout << "[OpenGL] Compilando shader GLSL.\n";
    }
};

class VulkanShader : public Shader {
public:
    void compile() const override {
        std::cout << "[Vulkan] Compilando shader para Vulkan (SPIR-V).\n";
    }
};

class RenderFactory {
public:
    virtual ~RenderFactory() = default;
    virtual std::unique_ptr<Window> createWindow() const = 0;
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<Shader> createShader() const = 0;
};

class OpenGLFactory : public RenderFactory {
public:
    std::unique_ptr<Window> createWindow() const override {
        return std::make_unique<OpenGLWindow>();
    }
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<OpenGLButton>();
    }
    std::unique_ptr<Shader> createShader() const override {
        return std::make_unique<OpenGLShader>();
    }
};

class VulkanFactory : public RenderFactory {
public:
    std::unique_ptr<Window> createWindow() const override {
        return std::make_unique<VulkanWindow>();
    }
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<VulkanButton>();
    }
    std::unique_ptr<Shader> createShader() const override {
        return std::make_unique<VulkanShader>();
    }
};

int main() {
    std::cout << "========== EJERCICIO 2: ABSTRACT FACTORY ==========\n";
    std::unique_ptr<RenderFactory> factory = std::make_unique<OpenGLFactory>();
    auto window = factory->createWindow();
    auto button = factory->createButton();
    auto shader = factory->createShader();
    window->draw();
    button->draw();
    shader->compile();
    std::cout << "\nCambiando la fabrica a Vulkan...\n";
    factory = std::make_unique<VulkanFactory>();
    auto vulkanWindow = factory->createWindow();
    auto vulkanButton = factory->createButton();
    auto vulkanShader = factory->createShader();
    vulkanWindow->draw();
    vulkanButton->draw();
    vulkanShader->compile();
    return 0;
}
