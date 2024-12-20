#include "../include/cards.hpp"
#include <iostream>
#include <string>

Card::Card(int face, int suit, std::string back): face(face), suit(suit), face_on_front(false), next(nullptr), back_texture(back)
{
  std::string front("");

  switch (face)
  {
    case 1:
      front.append("ace");
      break;
    case 2 ... 9:
      front.append(1, static_cast<char>(face + 48));
      break;
    case 10:
      front.append("10");
      break;
    case 11:
      front.append("jack");
      break;
    case 12:
      front.append("queen");
      break;
    case 13:
      front.append("king");
      break;
  }

  front.append("_");

  switch (suit)
  {
    case 1:
      front.append("spades");
      break;
    case 2:
      front.append("clubs");
      break;
    case 3:
      front.append("hearts");
      break;
    case 4:
      front.append("diamonds");
      break;
  }

  front_texture = front;
}

Card& Card::operator=(Card& card)
{
  return *this;
}

void Card::flip_card()
{
  if (face_on_front)
    face_on_front = false;
  else
    face_on_front = true;
}

std::string Card::get_current_texture()
{
  if (face_on_front)
    return front_texture;
  else
    return back_texture;
}

CardSlot::CardSlot() {}

CardSlot::CardSlot(float pos_x, float pos_y, short int expansion): pos_x(pos_x), pos_y(pos_y), expansion(expansion), cards(nullptr) {}

float CardSlot::get_x() const
{
  return pos_x;
}

float CardSlot::get_y() const
{
  return pos_y;
}

int CardSlot::get_expansion() const
{
  return expansion;
}

Card* CardSlot::get_last_card() const
{
  if (cards == nullptr)
    return NULL;

  Card *first = cards;

  while (first->next != nullptr)
    first = first->next;

  return first;
}

Card* CardSlot::get_first_card() const
{
  return cards;
}

unsigned short int CardSlot::get_card_count() const
{
  if (cards == nullptr)
    return 0;

  unsigned short int count = 0;

  Card *first = cards;

  while (first != nullptr)
  {
    count++;
    first = first->next;
  }

  return count;
}

void CardSlot::clear()
{
  while (cards != nullptr)
  {
    Card *current = cards->next;

    delete cards;

    cards = current;
  }
}

void CardSlot::add_card(Card *card, bool should_flip)
{
  if (should_flip)
    card->flip_card();

  if (cards == nullptr)
  {
    cards = card;
    return;
  }

  Card *first = cards;

  while (cards->next != nullptr)
    cards = cards->next;

  cards->next = card;

  cards = first;
}

void CardSlot::remove_card()
{
  if (cards->next == nullptr)
  {
    cards = nullptr;
    return;
  }
  Card *first = cards;

  while (cards->next->next != nullptr)
    cards = cards->next;

  cards->next = nullptr;

  cards = first;
}
