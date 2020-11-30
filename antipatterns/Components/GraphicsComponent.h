//
// Created by dmitry on 3/31/20.
//

#ifndef ANTIPATTERNS_GRAPHICSCOMPONENT_H
#define ANTIPATTERNS_GRAPHICSCOMPONENT_H

#include "../Resources/Constants/Constants.h"
#include "PhysicsComponent.h"

class GraphicsComponent {
private:
  struct AnimationParams;

public:
  GraphicsComponent(sf::Sprite &sprite, const sf::Texture &texture_sheet,
                    const std::map<std::string, Json::Node> &settings);

  GraphicsComponent(const GraphicsComponent &other);

  GraphicsComponent &operator=(const GraphicsComponent &other);

  ~GraphicsComponent();

  /* initializer */
  void LoadFromMap(const std::map<std::string, Json::Node> &settings);

  /* getter method */
  bool IsDone(const std::string &animation_key) const;

  void Reset();

  void AddAnimation(const std::string &animation_key, AnimationParams a_params);

  bool Play(const std::string &animation_key, float time_elapsed,
            float speed_modifier = 1, bool priority = false);

  void UpdateCopy(sf::Sprite &sprite);

private:
  struct AnimationParams {
    float time_to_animate_{};
    float time_{0};
    sf::IntRect start_rect_;
    sf::IntRect current_rect_;
    sf::IntRect end_rect_;
    sf::Vector2i rect_params_;
    float time_normalize_;

    AnimationParams() = default;

    AnimationParams(float anim_time, int start_frame_x, int start_frame_y,
                    int end_frame_x, int end_frame_y, sf::Vector2i rect_params, float time_normalize_ = kTimeNormalize);
  };

  class Animation {
  public:
    sf::Sprite *sprite_;
    const sf::Texture &texture_sheet_;
    AnimationParams a_params_;
    bool is_done_{false};

    Animation(sf::Sprite *sprite, const sf::Texture &texture_sheet,
              AnimationParams a_params);

    Animation(const Animation &other) = default;

    Animation &operator=(const Animation &other) = delete;

    bool Play(float time_elapsed, float speed_modifier = 1);

    void Reset();

    bool IsDone() const;

  private:
    void Animate();
  };

  void CheckLastAnimation(const std::string &animation_key);

  sf::Sprite *_sprite;
  const sf::Texture &_texture_sheet;
  std::unordered_map<std::string, std::unique_ptr<Animation>> _animations;
  Animation *_last_animation{nullptr};
  Animation *_prior_animation{nullptr};
};


#endif // ANTIPATTERNS_GRAPHICSCOMPONENT_H
