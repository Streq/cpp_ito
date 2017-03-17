#pragma once
#include "System.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>
#include "FontHolder.h"
class PlayerGUISystem : public System, public sf::Drawable
{
	public:
	PlayerGUISystem(World&,  FontHolder& fonts, const sf::Vector2f& pos = sf::Vector2f(20,20), const sf::Vector2f& size = sf::Vector2f(100.f,20.f));

	

	virtual void							update(sf::Time);
	virtual void							update_entity(const Flagset&, Handle);
	Handle									getPlayer()const;
	
	
	private:
	virtual void							draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	
	FontHolder*								mFonts;

	sf::RectangleShape						mCurrentLifeBar;
	sf::RectangleShape						mFullLifeBar;
	const std::array<size_t,Tag::size>&		mAmount_entities;
	sf::Text								mStats;
	sf::Text								mWinLose;
	Handle									current_player;
};

