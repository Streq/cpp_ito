#include "Container.h"

GUI::Container::Container():
	mSelectedChild(-1)
{}

bool GUI::Container::is_selectable() const{
	for(auto itr = mChildren.begin(); itr != mChildren.end(); itr++)
		if((*itr).get()->is_selectable())return true;
	return false;
}

void GUI::Container::handle_event(const sf::Event& e){
	switch(e.type){
		case sf::Event::EventType::KeyReleased:
			switch(e.key.code){
				case sf::Keyboard::Down:
					selectNext();
				break;
				case sf::Keyboard::Up:
					selectPrevious();
				break;
				case sf::Keyboard::Return:
					mChildren[mSelectedChild]->activate();
			}
		break;
	}
}

void GUI::Container::pack(Component::Ptr comp){
	mChildren.push_back(comp);
	if(!hasSelection()){
		if(comp->is_selectable()){
			select(mChildren.size()-1);
		}
	}
}

int GUI::Container::getIndex() const{
	return mSelectedChild;
}

void GUI::Container::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform *= getTransform();
	for(auto itr = mChildren.begin(); itr != mChildren.end(); itr++){
		target.draw(*(*itr),states);
	}
}

bool GUI::Container::hasSelection() const{
	return mSelectedChild >= 0;
}

void GUI::Container::select(std::size_t index){
	auto& to_be_selected=*mChildren[index];
	if(to_be_selected.is_selectable())
	{
		if(hasSelection()){
			auto& current = *mChildren[mSelectedChild];
			current.deselect();
		}
		to_be_selected.select();
		mSelectedChild = index;
	}
}

void GUI::Container::selectNext(){
	int index = mSelectedChild;
	if(!hasSelection()){
		index = -1;
	}
	size_t length = mChildren.size();
	do{	
		index = (index + 1) % length;
	}while(!mChildren[index]->is_selectable());
	select(index);
}

void GUI::Container::selectPrevious(){
	int index = mSelectedChild;
	if(!hasSelection()){
		index = 0;
	}
	size_t length=mChildren.size();
	do{
		index = (index + length - 1) % length;
	}while(!mChildren[index]->is_selectable());
	select(index);
}
