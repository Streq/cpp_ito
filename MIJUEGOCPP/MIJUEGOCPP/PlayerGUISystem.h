#pragma once
#include "System.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "SFML/Graphics/RectangleShape.hpp"
class PlayerGUISystem : public System, public sf::Drawable
{
	public:
	PlayerGUISystem(World&,const sf::Vector2f& pos = sf::Vector2f(20,20), const sf::Vector2f& size = sf::Vector2f(100.f,20.f));

	

	virtual void							update(sf::Time);
	virtual void							update_entity(const Flagset&, Handle);
	Handle									getPlayer()const;
	private:
	
	virtual void							draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	sf::RectangleShape						mCurrentLifeBar;
	sf::RectangleShape						mFullLifeBar;

	Handle									current_player;
};

