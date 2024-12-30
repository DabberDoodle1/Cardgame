#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "sprite_renderer.hpp"
#include "cards.hpp"

enum Variation {
  SOLITAIRE_TURN_1,
  SOLITAIRE_TURN_3
};

struct Cursor {
  double        x, y, card_x, card_y;
  Card          *card_held;
  unsigned char pile_selected;
  bool          has_released;

  Cursor();
};

class Game {
public:
  Game(unsigned int width, unsigned int height, const char *title);
  ~Game();

  int         isRunning() const;
  void        update();

private:
  Cursor         cursor;
  SpriteRenderer *renderer;
  GLFWwindow     *window;
  Variation      game_variation;
  bool           game_won, change_variant_next;
  unsigned int   width, height;
  CardPile       stock_pile,
                 waste_pile,
                 foundation_piles[4],
                 tableau_piles[7];

  void        init(unsigned int width, unsigned int height, const char *title);
  void        load_textures(std::string location, std::string extension);
  void        render();
  void        input();
  void        new_game();
  void        clear_piles();
  void        check_hitboxes(int& hover, int& index, double& card_x, double& card_y);
  void        press_response(int hover, int index, double card_x, double card_y);
  void        release_response(int hover);
  void        return_to_pile();
  CardPile&   find_pile(int index);
  bool        check_game_won();

  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif
