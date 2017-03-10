#pragma once
#include <SFML/Graphics/Text.hpp>
#include <functional>
#include "GUI_Component.h"
#include "FontHolder.h"
class Game;

namespace GUI{


class Button : public Component
{

	public:
		typedef std::function<void()>	Callback;
		typedef std::shared_ptr<Button> Ptr;

								Button(FontHolder& fonts);

		void					setCallback(Callback callback);
		void					setText(const std::string& text, bool adjust_size=true);
		void					setToggle(bool flag);
		void					setSize(const sf::Vector2f&);


		virtual bool			is_selectable() const;
		virtual void			select();
		virtual void			deselect();

		virtual void			activate();
		virtual void			deactivate();

		virtual void			handle_event(const sf::Event& event);


	private:	
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
	
		static const sf::Color&				Color_Not_Selected;
		static const sf::Color&				Color_Selected;
		static const sf::Color&				Color_Pressed;
		static const sf::Color&				Color_Not_Pressed;



		sf::Text						mText;
		Callback						mCallback;
		sf::RectangleShape				mSprite;
		bool							mIsToggle;
};



}