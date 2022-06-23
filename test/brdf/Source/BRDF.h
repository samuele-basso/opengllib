#pragma once

#include "Primitives/Shader.h"
#include "Scene/Mesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <string>

#include "Base/Core.h"
#include "Base/Log.h"

#include "EditorCameraController.h"

#include <imgui.h>
#include <imconfig.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Sandbox
{

	static bool Vec3Control(const std::string& f_Label, glm::vec3& f_Values, float f_ResetValue = 0.0f, float f_ColumnWidth = 100.0f)
	{
		bool modified = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::Text(f_Label.c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushItemWidth(f_ColumnWidth);

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(100, 0, 0, 150));
		if (ImGui::Button("X", buttonSize))
		{
			f_Values.x = f_ResetValue;
			modified = true;
		}
		ImGui::PopStyleColor();
		ImGui::PopFont();
		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &f_Values.x, 0.01f, 0.0f, 0.0f, "%.2f"))
		{
			modified = true;
		}
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 100, 0, 150));
		if (ImGui::Button("Y", buttonSize))
		{
			f_Values.y = f_ResetValue;
			modified = true;
		}
		ImGui::PopStyleColor();
		ImGui::PopFont();
		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &f_Values.y, 0.01f, 0.0f, 0.0f, "%.2f"))
		{
			modified = true;
		}
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(75, 150, 250, 150));
		if (ImGui::Button("Z", buttonSize))
		{
			f_Values.z = f_ResetValue;
			modified = true;
		}
		ImGui::PopStyleColor();
		ImGui::PopFont();
		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &f_Values.z, 0.01f, 0.0f, 0.0f, "%.2f"))
		{
			modified = true;
		}

		ImGui::PopStyleVar(2);
		ImGui::PopItemWidth();

		return modified;
	}

	class BRDF
	{
	public:
		BRDF()
		{
			p_Window.Init();

			Jam::Input::Init(&p_Window);

			p_Context.Init(&p_Window);

			p_Context.Depth(true);
			p_Context.Blend(true);

			p_World = { 0.34, 0.55, 0.78, 1.00 };
			p_Context.SetClearColor(p_World);

			Jam::Global::Init();

			p_Integrator.Init(
				{ "Shaders/integrator.vs.glsl" },
				{ "Shaders/integrator.fs.glsl", { "LIGHT_ARRAY_SIZE 64" } }
			);

			p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_AlphaRemap"), std::string("AR_Direct"));
			p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_NormalDistribution"), std::string("ND_Trowbridge"));
			p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_GeometryOcclusion"), std::string("GO_CookTorrance"));
			p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_Fresnel"), std::string("F_CookTorrance"));

			p_Integrator.Bind();

			p_Integrator.SetUniform4fv("u_World", 1, glm::value_ptr(p_World));

			p_Metalness = 0.9;
			p_Integrator.SetUniform1fv("u_Metalness", 1, &p_Metalness);

			p_Roughness = 0.2;
			p_Integrator.SetUniform1fv("u_Roughness", 1, &p_Roughness);

			p_Albedo = { 0.27, 0.70, 0.02 };
			p_Integrator.SetUniform3fv("u_Albedo", 1, glm::value_ptr(p_Albedo));

			p_Reflectivity = { 0.04, 0.04, 0.04 };
			p_Integrator.SetUniform3fv("u_BaseReflectivity", 1, glm::value_ptr(p_Reflectivity));

			p_AmbientWorld = 0.06;
			p_Integrator.SetUniform1fv("u_AmbientWorld", 1, &p_AmbientWorld);

			p_Ambient = 0.03;
			p_Integrator.SetUniform1fv("u_Ambient", 1, &p_Ambient);

			p_LightColor = { 1.0, 1.0, 1.0 };
			p_LightStrength = 10.0f;

			p_Integrator.SetUniformMatrix4fv("u_Model", 1, false, glm::value_ptr(p_Transform.ModelMatrix));

			p_Integrator.SetUniformMatrix4fv("u_ViewProjection", 1, false, glm::value_ptr(p_Controller.GetCamera().GetViewProjection()));
			p_Integrator.SetUniform3fv("u_ViewPosition", 1, glm::value_ptr(p_Controller.GetCamera().TranslationVector));


			auto model = Jam::Model::Load("Assets/sphere.obj");
			p_Sphere = std::get<0>(model[0]);

			p_VertexArray.Init();
			p_IndexBuffer.Init();
			p_VertexBuffer.Init();

			p_VertexArray.BindIndexBuffer(&p_IndexBuffer);

			int indices = p_Sphere->Indices.size();
			p_IndexBuffer.SetData(&p_Sphere->Indices[0], indices);

			size_t verticesSize = p_Sphere->VerticesSize;
			p_VertexBuffer.SetData(&p_Sphere->Vertices[0], verticesSize);

			p_VertexArray.BindVertexBuffer(
				&p_VertexBuffer,
				Jam::BufferLayout({
					Jam::ShaderDataType::Float3,
					Jam::ShaderDataType::Float3,
					Jam::ShaderDataType::Float3,
					Jam::ShaderDataType::Float3,
					Jam::ShaderDataType::Float2
				})
			);


			std::function<void(Jam::WindowResizeEvent*)>* windowResizeEvent = new std::function<void(Jam::WindowResizeEvent*)>([&](Jam::WindowResizeEvent* f_E) {
				Jam::EventQueue::Push(new Jam::ViewportResizeEvent(f_E->GetXSize(), f_E->GetYSize()));
				});
			Jam::Publisher<Jam::WindowResizeEvent*>::Subscribe(windowResizeEvent);

			auto& windowState = Jam::Global::GlobalWindowState;
			Jam::EventQueue::Push(new Jam::WindowResizeEvent(windowState.Width, windowState.Height));

			const char* glsl_version = "#version 460";
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui_ImplGlfw_InitForOpenGL(p_Window.GetGLFWWindow(), true);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}

		~BRDF()
		{
			JAM_PROFILE_END_SESSION();
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void Run()
		{
			while (!p_Window.ShouldClose())
			{
				Jam::Timestep st;

				Jam::EventQueue::Process();

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				UpdateBRDF();

				DrawUI();

				p_Window.Update();
			}
		}

		void UpdateBRDF()
		{
			p_Integrator.Bind();
			p_Integrator.SetUniformMatrix4fv("u_Model", 1, false, glm::value_ptr(p_Transform.ModelMatrix));
			p_Integrator.SetUniformMatrix4fv("u_ViewProjection", 1, false, glm::value_ptr(p_Controller.GetCamera().GetViewProjection()));
			p_Integrator.SetUniform3fv("u_ViewPosition", 1, glm::value_ptr(p_Controller.GetCamera().TranslationVector));
			p_Integrator.SetUniform1i("u_PointCount", 1);
			p_Integrator.SetUniform3fv("u_PointLight[0].Position", 1, glm::value_ptr(glm::rotateZ<float>(glm::rotateX<float>(glm::rotateY<float>(glm::vec3(1.5), glfwGetTime()), glfwGetTime()), glfwGetTime())));
			p_Integrator.SetUniform3fv("u_PointLight[0].Color", 1, glm::value_ptr(p_LightColor * p_LightStrength));

			p_Integrator.SubroutineFlush();

			p_VertexArray.Bind();

			glDrawElements(GL_TRIANGLES, p_Sphere->IndicesCount, GL_UNSIGNED_INT, (void*)0);
		}

		void DrawUI()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("BRDF");

			if (ImGui::DragFloat("Metalness", &p_Metalness, 0.001f, 0.0f, 1.0f, "%.3f"))
			{
				p_Integrator.SetUniform1fv("u_Metalness", 1, &p_Metalness);
			}

			if (ImGui::DragFloat("Roughness", &p_Roughness, 0.001f, 0.0f, 1.0f, "%.3f"))
			{
				p_Integrator.SetUniform1fv("u_Roughness", 1, &p_Roughness);
			}

			if (ImGui::ColorEdit4("World color", glm::value_ptr(p_World)))
			{
				p_Integrator.SetUniform4fv("u_World", 1, glm::value_ptr(p_World));
				p_Context.SetClearColor(p_World);
			}

			if (ImGui::DragFloat("Ambient world strength", &p_AmbientWorld, 0.001f, 0.0f, 1.0f, "%.3f"))
			{
				p_Integrator.SetUniform1fv("u_AmbientWorld", 1, &p_AmbientWorld);
			}

			if (ImGui::ColorEdit3("Albedo", glm::value_ptr(p_Albedo)))
			{
				p_Integrator.SetUniform3fv("u_Albedo", 1, glm::value_ptr(p_Albedo));
			}

			if (ImGui::DragFloat("Ambient strength", &p_Ambient, 0.001f, 0.0f, 1.0f, "%.3f"))
			{
				p_Integrator.SetUniform1fv("u_Ambient", 1, &p_Ambient);
			}

			if (ImGui::ColorEdit3("Light color", glm::value_ptr(p_LightColor)))
			{
				p_Integrator.SetUniform3fv("u_PointLight[0].Color", 1, glm::value_ptr(p_LightColor * p_LightStrength));
			}

			if (ImGui::DragFloat("Light strength", &p_LightStrength, 0.01f, 0.0f, 1000.0f, "%.2f"))
			{
				p_Integrator.SetUniform3fv("u_PointLight[0].Color", 1, glm::value_ptr(p_LightColor * p_LightStrength));
			}

			ImGui::PushID("Reflectivity");
			if (Vec3Control("Reflectivity", p_Reflectivity))
			{
				p_Integrator.SetUniform3fv("u_BaseReflectivity", 1, glm::value_ptr(p_Reflectivity));
			}
			ImGui::PopID();


			static const char* ar_list[] = {
				"AR_Direct",
				"AR_IBL"
			};
			static int ar_selected = 0;

			ImGui::Text("Alpha remap");
			if (ImGui::Combo("##Alpha", &ar_selected, ar_list, IM_ARRAYSIZE(ar_list)))
			{
				p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_AlphaRemap"), std::string(ar_list[ar_selected]));
			}


			static const char* nd_list[] = {
				"ND_Trowbridge",
				"ND_Beckmann",
				"ND_Blinn"
			};
			static int nd_selected = 0;

			ImGui::Text("Normal distribution");
			if (ImGui::Combo("##Normal", &nd_selected, nd_list, IM_ARRAYSIZE(nd_list)))
			{
				p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_NormalDistribution"), std::string(nd_list[nd_selected]));
			}


			static const char* go_list[] = {
				"GO_Beckmann",
				"GO_Implicit",
				"GO_Neumann",
				"GO_CookTorrance",
				"GO_Kelemen",
				"GO_Smith"
			};
			static int go_selected = 0;

			ImGui::Text("Geometry occlusion");
			if (ImGui::Combo("##Geometry", &go_selected, go_list, IM_ARRAYSIZE(go_list)))
			{
				p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_GeometryOcclusion"), std::string(go_list[go_selected]));
			}


			static const char* f_list[] = {
				"F_None",
				"F_Schlick",
				"F_CookTorrance"
			};
			static int f_selected = 0;

			ImGui::Text("Fresnel");
			if (ImGui::Combo("##Fresnel", &f_selected, f_list, IM_ARRAYSIZE(f_list)))
			{
				p_Integrator.SetSubroutine(Jam::Constant::FRAGMENT, std::string("s_Fresnel"), std::string(f_list[f_selected]));
			}

			ImGui::Separator();


			ImGui::PushID("CameraPosition");
			if (Vec3Control("Position", p_Controller.GetCamera().TranslationVector))
			{
				p_Controller.GetCamera().SetPosition(p_Controller.GetCamera().TranslationVector);
			}
			ImGui::PopID();

			ImGui::PushID("CameraRotation");
			if (Vec3Control("Rotation", p_Controller.GetCamera().RotationVector))
			{
				p_Controller.GetCamera().SetRotation(p_Controller.GetCamera().RotationVector);
			}
			ImGui::PopID();


			ImGui::Separator();

			auto t = Jam::Timestep::Get();

			ImGui::Text("Frametime: %f", t);
			ImGui::SameLine();
			ImGui::Text("FPS: %.2f", 1.0f / t);

			ImGui::End();

			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup);
			}
		}

	private:
		Jam::Window p_Window;
		Jam::Context p_Context;

		Jam::Shader p_Integrator;

		glm::vec4 p_World;

		float p_Metalness;
		float p_Roughness;

		glm::vec3 p_Albedo;
		glm::vec3 p_LightColor;
		glm::vec3 p_Reflectivity;

		float p_LightStrength;
		float p_AmbientWorld;
		float p_Ambient;

		Jam::Mesh* p_Sphere;

		Jam::VertexArray p_VertexArray;
		Jam::VertexBuffer p_VertexBuffer;
		Jam::IndexBuffer p_IndexBuffer;

		Sandbox::EditorCameraController p_Controller;

		Jam::Transform p_Transform;
	};

}