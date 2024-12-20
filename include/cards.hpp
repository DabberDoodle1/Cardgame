#ifndef CARDSLOT_HEADER
#define CARDSLOT_HEADER

#include <string>

//@ Linked list for cards @//
class Card
{
public:
  //@ Constructor @//
  Card();
  Card(int suit, int face, std::string back);

  //@ Copy Assignment operator @//
  Card& operator=(Card& card);

  //@ Flip card @//
  void flip_card();

  //@ Get texture name @//
  std::string get_current_texture();

  //@ Next card @//
  Card *next;

private:
  bool face_on_front; //@ Boolean to see if a card has been flipped or not @//
  const int suit; //@ 1 - spades, 2 - clubs, 3 - hearts, 4 - diamonds @//
  const int face; //@ 1 - ace, 2-10 corresponding, 11 - jack, 12 - queen, 13 - king @//
  std::string front_texture; 
  std::string back_texture;
};

class CardSlot
{
public:
  //@ Constructor and destructor methods @//
  CardSlot();
  CardSlot(float pos_x, float pos_y, short int expansion);

  //@ Get methods @//
  float get_x() const;
  float get_y() const;
  int get_expansion() const;
  Card* get_last_card() const;
  Card* get_first_card() const;
  unsigned short int get_card_count() const;

  //@ Adds and removes cards @//
  void add_card(Card* card, bool should_flip);
  void remove_card();

  //@ Free all cards @//
  void clear();

private:
  //@ Linked list to hold cards and a pointer to the last of the card;
  Card *cards;
  float pos_x, pos_y;
  short int expansion;

};

#endif
