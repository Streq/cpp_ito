#pragma once
#include "GUI_Component.h"

class TextBox : public GUI::Component{
	public:
									TextBox(const sf::Font& font,const std::string& text = std::string(), unsigned line_width=100, unsigned character_size=15U);
	void							setLineWidth(unsigned size);
	unsigned						getLineWidth()const;
	void							setText(const std::string&);

	virtual bool					is_selectable()const;
	
	virtual void					handle_event(const sf::Event&);
	
	
	private:
	void							formatText();
	void							update_box();
	virtual void					draw(sf::RenderTarget& target,sf::RenderStates states)const;

	sf::Text						mText;
	std::string						mString;
	sf::RectangleShape				mBackground;
	unsigned						mLineWidth;
};