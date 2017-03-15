#include "TextBox.h"
#include "Color.h"
#include <SFML\Graphics\Color.hpp>
#include "Text_Util.h"
TextBox::TextBox(const sf::Font& font, const std::string& text, unsigned line_width, unsigned character_size):
	mLineWidth(line_width),
	mBackground(sf::Vector2f(200.f,50.f)),
	mText(sf::String(text),font,character_size),
	mString(text)
{
	sf::Color color(Color::Dark_Blue);
	color.a = 200;
	mBackground.setFillColor(color);
	update_box();
}

void TextBox::setLineWidth(unsigned size){
	mLineWidth=size;
	update_box();
}

unsigned TextBox::getLineWidth() const{
	return mLineWidth;
}

void TextBox::setText(const std::string& text){
	mString=text;
	update_box();
}

bool TextBox::is_selectable() const{
	return false;
}

void TextBox::handle_event(const sf::Event &){
}

void TextBox::formatText(){
	std::string formated=mString;

	if(!mString.empty()){
		size_t covered_string = 0;
		size_t next_break;
		size_t line_length;
		do{
			next_break = formated.find("\n",covered_string);//find line break
			line_length = (next_break == std::string::npos ? formated.size() - 1: next_break) - covered_string;
			while(line_length > mLineWidth){//if the line is bigger than it should
				size_t previous_space = formated.find_last_of(' ', line_length + covered_string - 1);//look for last space in the line
				if(previous_space != std::string::npos && previous_space > covered_string){//if it exists and is in the current line
					line_length = previous_space - covered_string;//assign the line length to it
				}
				else{//if it doesn't exist or belongs to a previous line then fuck it
					break;
				}
			}
			formated[covered_string + line_length] = '\n'; //assign the space (or line break if we found nothin) we are on to a break
			covered_string += line_length+1;//the line length is covered
		}while(line_length + covered_string != formated.size() - 1 );// while a line break exists
	}
	mText.setString(formated);
}

void TextBox::update_box(){
	formatText();
	auto bounds = mText.getGlobalBounds();
	centerOrigin(mText);
	mBackground.setSize(sf::Vector2f(bounds.width + 20,bounds.height + 20));
	mText.setPosition(floor(mBackground.getSize().x/2.f),floor(mBackground.getSize().y/2.f));
}

void TextBox::draw(sf::RenderTarget& target,sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(mBackground, states);
	target.draw(mText, states);
}
