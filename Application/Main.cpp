#include "Engine.h"
#include <iostream> 
#include <Renderer/Program.cpp>

#define POST_PROCESS 

int main(int argc, char** argv)
{
	LOG("Application Started...");
	neu::InitializeMemory();
	neu::SetFilePath("../Assets");

	neu::Engine::Instance().Initialize();
	neu::Engine::Instance().Register();
	LOG("Engine Initialized...");

	neu::g_renderer.CreateWindow("Neumont", 800, 600);
	LOG("Window Created...");
	neu::g_gui.Initialize(neu::g_renderer);

	// create framebuffer texture
	auto texture = std::make_shared<neu::Texture>();
	texture->CreateTexture(1024, 1024);
	neu::g_resources.Add<neu::Texture>("fb_texture", texture);

	// create framebuffer
	auto framebuffer = neu::g_resources.Get<neu::Framebuffer>("framebuffer", "fb_texture");
	framebuffer->Unbind();

	// load scene 
	auto scene = neu::g_resources.Get<neu::Scene>("scenes/almost.scn");

	glm::vec3 rot = {0,0,0};
	float interpolation = 0;
	float ri = 1.4;
	bool quit = false;
	while (!quit)
	{
		neu::Engine::Instance().Update();
		neu::g_gui.BeginFrame(neu::g_renderer);

		if (neu::g_inputSystem.GetKeyState(neu::key_escape) == neu::InputSystem::KeyState::Pressed) quit = true;

		auto actor = scene->GetActorFromName("Light");
		if (actor)
		{
			// move light using sin wave 
			actor->m_transform.position = rot;
		}
		auto actor2 = scene->GetActorFromName("Ogre");
		if (actor2)
		{
			actor2->m_transform.rotation = math::EulerToQuaternion(rot);
		}

		auto actor5 = scene->GetActorFromName("Dog");
		if (actor5)
		{
			actor5->m_transform.rotation = math::EulerToQuaternion(rot);
		}

		auto actor6 = scene->GetActorFromName("RTT");

		auto actor3 = scene->GetActorFromName("Light2");
		auto actor4 = scene->GetActorFromName("Light3");

		auto program = neu::g_resources.Get<neu::Program>("shaders/fx/reflect_refract.prog");
		if (program)
		{
			program->Use();
			program->SetUniform("interpolation", interpolation);
			program->SetUniform("ri", ri);
		}

		auto program1 = neu::g_resources.Get<neu::Program>("shaders/postprocess/postprocess.prog");
		if (program1)
		{
			program->Use();
			program->SetUniform("offset", neu::g_time.time);
		}

		ImGui::Begin("Hello!");
		ImGui::DragFloat3("Rotation", &rot[0]);
		ImGui::SliderFloat("ri", &ri, 1, 3);
		ImGui::SliderFloat("interpolation", &interpolation, 0, 1);
		ImGui::End();

		scene->Update();

#ifdef POST_PROCESS 
		// don't draw post process actor when rendering to the framebuffer 
		{
			auto actor = scene->GetActorFromName("PostProcess");
			if (actor)
			{
				actor->SetActive(false);
			}
		}
		// render pass 1 (render scene to framebuffer) 
		neu::g_renderer.SetViewport(0, 0, framebuffer -> GetSize().x, framebuffer->GetSize().y);
		framebuffer->Bind();
		neu::g_renderer.BeginFrame();
		scene->PreRender(neu::g_renderer);
		scene->Render(neu::g_renderer);
		framebuffer->Unbind();

		// render pass 2 (render to screen) 
		neu::g_renderer.RestoreViewport();
		neu::g_renderer.BeginFrame();
		scene->PreRender(neu::g_renderer);

		// draw only the post process actor to the screen 
		{
			auto actor = scene->GetActorFromName("PostProcess");
			if (actor)
			{
				actor->SetActive(true);
				actor->Draw(neu::g_renderer);
			}
		}
#else 
		neu::g_renderer.BeginFrame();
		scene->PreRender(neu::g_renderer);
		scene->Render(neu::g_renderer);
#endif // POST_PROCESS 

		neu::g_gui.Draw();

		neu::g_renderer.EndFrame();

		neu::g_gui.EndFrame();
	}

	scene->RemoveAll();

	neu::Engine::Instance().Shutdown();

	return 0;
}

