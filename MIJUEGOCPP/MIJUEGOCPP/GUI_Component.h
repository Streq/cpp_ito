#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
namespace GUI{

class Component : public sf::Drawable, public sf::Transformable{
	public:
		typedef  std::shared_ptr<Component> Ptr;
		
									Component();
		
		virtual bool				is_active()const;
		virtual void				activate();
		virtual void				deactivate();


		virtual bool				is_selectable()const = 0;
		bool						is_selected()const;
		virtual void				select();
		virtual void				deselect();

		virtual void				handle_event(const sf::Event&) = 0;
	
	private:
		bool mSelected;
		bool mActive;

};

}