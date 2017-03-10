#include "Button.h"
#include "Text_Util.h"
#include "Color.h"
const sf::Color& GUI::Button::Color_Not_Selected = Color::Transparent;
const sf::Color& GUI::Button::Color_Selected = Color::White;
const sf::Color& GUI::Button::Color_Pressed = sf::Color(100,100,100);
const sf::Color& GUI::Button::Color_Not_Pressed = sf::Color(200,200,200);


GUI::Button::Button(FontHolder & fonts): 
	Component(),
	mSprite(sf::Vector2f(80.f,30.f))
{
	mText.setCharacterSize(15U);
	mText.setFont(fonts.get(Font::arial));
	mText.setFillColor(sf::Color::Black);
	mSprite.setFillColor(Color_Not_Pressed);
	mSprite.setOutlineColor(Color_Not_Selected);
	mSprite.setOutlineThickness(2.f);
	auto bounds=mSprite.getLocalBounds();
	//centerOrigin(mText);
	mText.setPosition(bounds.left + bounds.width / 2.f,bounds.top + bounds.height / 2.f);
	
}

void GUI::Button::setCallback(Callback callback){
	mCallback=callback;
}

void GUI::Button::setText(const std::string & text, bool adjust_size){
	mText.setString(text);
	centerOrigin(mText);
	if(adjust_size){
		auto bounds = mText.getGlobalBounds();
		auto cur_size = mSprite.getSize();
		if(bounds.width >= cur_size.x){
			float dif=bounds.width + 10 - cur_size.x;
			cur_size.x += dif;

			
			mSprite.setSize(cur_size);
			mSprite.move(-dif / 2.f, 0.f);
		}
	}
}

void GUI::Button::setToggle(bool flag){
	mIsToggle=flag;
}

void GUI::Button::setSize(const sf::Vector2f &size){
	mSprite.setSize(size);
}

bool GUI::Button::is_selectable() const{
	return true;
}

void GUI::Button::select(){
	Component::select();
	mSprite.setOutlineColor(Color_Selected);
}

void GUI::Button::deselect(){
	Component::deselect();
	mSprite.setOutlineColor(Color_Not_Selected);
}

void GUI::Button::activate(){
	Component::activate();
	if(mCallback)mCallback();
	mSprite.setFillColor(Color_Pressed);
	deactivate();
}

void GUI::Button::deactivate(){
	Component::deactivate();
	mSprite.setFillColor(Color_Not_Pressed);
}

void GUI::Button::handle_event(const sf::Event & event){
}

void GUI::Button::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}
