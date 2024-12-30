#ifndef CARDSLOT_HEADER
#define CARDSLOT_HEADER

#include <string>

// Linked list for cards because it is easier to move efficient when moving piles of cards than one at a time
struct Card
{
public:
  Card(int suit, int face, std::string back_texture);
  Card& operator=(Card& card);

  unsigned char get_suit() const;
  unsigned char get_face() const;
  std::string   get_texture() const;
  bool          face_is_on_top() const;
  void          flip();

  Card          *next;
private:
  // Card suit and face will be numbers to make comparisons with other cards easier
  // Textures for face and back will be strings because ResourceManager takes in strings for texture names
  unsigned char suit;
  unsigned char face;
  std::string   face_texture;
  std::string   back_texture;
  bool          face_on_top;
};

// Wrapper class for linked list to make interactions like appending and removal easier
// It also reduces the likeliness of encountering bugs when "getting" cards(list nodes)
class CardPile {
public:
  CardPile();
  
  Card*         get_first();
  Card*         get_last();
  Card*         take_nth(int index);
  unsigned char get_count();
  bool          is_empty();
  bool          is_hovered(double pile_x, double cursor_x, double cursor_y, int& index);
  void          append(Card* card);
  void          remove_end();
  void          delete_pile();
private:
  void          recount();

  Card          *head;
  unsigned char card_count;
};

#endif
