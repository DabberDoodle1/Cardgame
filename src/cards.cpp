#include "../include/cards.hpp"
#include <string>

Card::Card(int suit, int face, std::string back_texture):
  suit(suit), face(face), face_on_top(false), back_texture(back_texture), next(nullptr)
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

  switch (suit + 1)
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

  face_texture = front;
}

Card& Card::operator=(Card& card)
{
  return *this;
}

void Card::flip()
{
  if (face_on_top)
    face_on_top = false;
  else
    face_on_top = true;
}

unsigned char Card::get_suit() const
{
  return suit;
}

unsigned char Card::get_face() const
{
  return face;
}

std::string Card::get_texture() const
{
  if (face_on_top)
    return face_texture;
  else
    return back_texture;
}

bool Card::face_is_on_top() const
{
  return face_on_top;
}


CardPile::CardPile(): head(nullptr), card_count(0) {}

Card* CardPile::get_first()
{
  return head;
}

Card* CardPile::get_last()
{
  if (!head)
    return nullptr;

  Card *temp = head;


  while (temp->next != NULL)
    temp = temp->next;


  return temp;
}

Card* CardPile::take_nth(int index)
{
  if (index == 0)
    return nullptr;

  if (index == -1)
  {
    Card *last = get_last();
    remove_end();
    card_count--;

    return last;
  }

  if (index == 1)
  {
    Card *first = head;
    head = nullptr;
    card_count = 0;

    return first;
  }

  Card *temp = head, *nth_card;

  for (int i = 0; i < index - 2; i++)
    head = head->next;

  nth_card = head->next;
  head->next = nullptr;
  head = temp;

  recount();

  return nth_card;
}

unsigned char CardPile::get_count()
{
  return card_count;
}

bool CardPile::is_empty()
{
  if (!head)
    return true;
  else
    return false;
}

bool CardPile::is_hovered(double pile_x, double cursor_x, double cursor_y, int& index)
{
  if (!head)
  {
    card_count = 0;
    index = 0;

    if ((cursor_x >= pile_x && cursor_x <= pile_x + 100.0)  &&  (cursor_y >= 300.0 && cursor_y <= 400.0))
      return true;
    else
      return false;
  }

  double card_y      = 300.0;
  int    count       = 1;
  Card   *current    = head;

  while (current->next != nullptr)
  {
    if ((cursor_x >= pile_x && cursor_x <= pile_x + 100.0)  &&  (cursor_y >= card_y && cursor_y <= card_y + 20.0)  &&  current->face_is_on_top())
    {
      index = count;
      return true;
    }

    current = current->next;
    card_y += 20.0;
    count++;
  }

  if ((cursor_x >= pile_x && cursor_x <= pile_x + 100.0)  &&  (cursor_y >= card_y && cursor_y <= card_y + 140.0)  &&  current->face_is_on_top())
  {
    index = -1;
    return true;
  }

  index = 0;
  return false;
}

void CardPile::append(Card *card)
{
  if (!head)
  {
    head = card;
    card_count = 1;
    return;
  }

  Card *temp = head;

  while (head->next != NULL)
    head = head->next;

  head->next = card;
  head = temp;
  recount();
}

void CardPile::remove_end()
{
  if (!head->next)
  {
    head = nullptr;
    card_count = 0;
    return;
  }

  Card *temp = head;

  while (head->next->next != NULL)
    head = head->next;

  head->next = nullptr;
  head = temp;
  recount();
}

void CardPile::recount()
{
  if (!head)
  {
    card_count = 0;
    return;
  }

  Card *temp = head;
  int count = 0;

  while (temp != NULL)
  {
    temp = temp->next;
    count++;
  }

  card_count = count;
}

void CardPile::delete_pile()
{
  card_count = 0;

  while (head != NULL)
  {
    Card *temp = head->next;

    delete head;

    head = temp;
  }
}
