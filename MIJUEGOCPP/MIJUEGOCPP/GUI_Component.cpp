#include "GUI_Component.h"

GUI::Component::Component():
	mSelected(false),
	mActive(false)
{}

bool GUI::Component::is_active() const{
	return mActive;
}

void GUI::Component::activate(){
	mActive=true;
}

void GUI::Component::deactivate(){
	mActive=false;
}

bool GUI::Component::is_selected() const{
	return mSelected;
}

void GUI::Component::select(){
	mSelected=true;
}

void GUI::Component::deselect(){
	mSelected=false;
}
