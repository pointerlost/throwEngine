#pragma once
#include <iostream>
#include "Input/InputContext.h"
#include <vector>

namespace GLgraphics { class Transformations; }
namespace SCENE { class Scene; };

namespace InputComponent
{

	// This is a Interface class for Input components!
	class IInputComponent
	{
	public:
		IInputComponent() = default;

		virtual ~IInputComponent() = default;

		virtual void processInput(SCENE::Scene& scene) = 0;

		virtual void setUpInputContext() = 0;
	};


	/*class TriangleInputComponent : public IInputComponent
	{
	public:
		TriangleInputComponent() = default;

		void processInput(SCENE::Scene& scene) override;
	};*/


	/*class SquareInputComponent : public IInputComponent
	{
	public:
		SquareInputComponent() = default;

		void processInput(SCENE::Scene& scene) override;
	};*/

	class CubeInputComponent : public IInputComponent
	{
	public:
		CubeInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context);
		~CubeInputComponent() = default;

		void processInput(SCENE::Scene& scene) override;

		void setUpInputContext() override;

	private:
		std::shared_ptr<GLgraphics::Transformations> m_transformation;

		Input::InputContext m_dataContext;
	};


	class CircleInputComponent : public IInputComponent
	{
	public:
		CircleInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context);

		void processInput(SCENE::Scene& scene) override {};

		void setUpInputContext() override;

	private:
		std::shared_ptr<GLgraphics::Transformations> m_transformation;
		Input::InputContext m_dataContext;
	};


	class SunInputComponent : public IInputComponent
	{
	public:
		SunInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context);

		void processInput(SCENE::Scene& scene) override;

		void setUpInputContext() override;

	private:
		std::shared_ptr<GLgraphics::Transformations> m_transformation;
		Input::InputContext m_dataContext;

		int m_activeLightComponentIdx = 0;
		std::vector<bool> m_inputSelectors;
	};

}