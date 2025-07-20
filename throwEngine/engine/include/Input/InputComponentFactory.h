#pragma once
#include <iostream>
#include <string>

namespace GLgraphics { class Transformations; };

namespace LIGHTING { class Light; };

namespace Input
{
	// forward declaration
	class IInputComponent;
	enum struct InputType : int;

	class InputComponentFactory
	{
	public:
		InputComponentFactory() = default;
		~InputComponentFactory() = default;
		
		static std::shared_ptr<IInputComponent> createObjectComponent(InputType type, std::shared_ptr<GLgraphics::Transformations> transform);
		static std::shared_ptr<IInputComponent> createLightComponent(InputType type, std::shared_ptr<GLgraphics::Transformations> transform, std::shared_ptr<LIGHTING::Light>& light);
	};
} // namespace Input