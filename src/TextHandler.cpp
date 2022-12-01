#include "TextHandler.h"

TextHandler::TextHandler(const sf::Font& font, float posX, float posY,
	sf::Color color, int fontSize, sf::String stringText)
{
	this->text.setFont(font);
	this->text.setCharacterSize(fontSize);
	this->text.setFillColor(color);
	this->text.setPosition(posX, posY);
	this->text.setString(stringText);
}

sf::Text TextHandler::getsfText()
{
	return this->text;
}

void TextHandler::updateString(sf::String newString)
{
	this->text.setString(newString);
}