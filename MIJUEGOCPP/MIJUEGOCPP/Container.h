#pragma once
#include<SFML/Graphics.hpp>
#include"GUI_Component.h"
namespace GUI{

class Container: public Component{
	public:
									Container();
		
		virtual bool				is_selectable()const;
		virtual void				handle_event(const sf::Event&);
		void						pack(Component::Ptr comp);
		
	private:
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
		bool						hasSelection() const;
		void						select(std::size_t index);
		void						selectNext();
		void						selectPrevious();

	private:
		std::vector<Component::Ptr>				mChildren;
		int										mSelectedChild;
};



}