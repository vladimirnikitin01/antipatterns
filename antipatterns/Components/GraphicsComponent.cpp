//
// Created by dmitry on 3/31/20.
//

#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent(
    sf::Sprite &sprite, const sf::Texture &texture_sheet,
    const std::map<std::string, Json::Node> &settings)
    : _sprite(&sprite), _texture_sheet(texture_sheet) {
  LoadFromMap(settings);
}

GraphicsComponent::~GraphicsComponent() {}

void GraphicsComponent::LoadFromMap(
    const std::map<std::string, Json::Node> &settings) {
  for (const auto &[animation_key, settings_node] :
       settings.at("animations").AsMap()) {
    float time_normalize = kTimeNormalize;
    const std::map<std::string, Json::Node> &anim_settings =
        settings_node.AsMap();
    float anim_time =
        static_cast<float>(anim_settings.at("animation_time").AsDouble());
    int start_frame_x =
        static_cast<int>(anim_settings.at("start_frame_x").AsDouble());
    int start_frame_y =
        static_cast<int>(anim_settings.at("start_frame_y").AsDouble());
    int end_frame_x =
        static_cast<int>(anim_settings.at("end_frame_x").AsDouble());
    int end_frame_y =
        static_cast<int>(anim_settings.at("end_frame_y").AsDouble());
    int rect_params_x =
        static_cast<int>(anim_settings.at("rect_params_x").AsDouble());
    int rect_params_y =
        static_cast<int>(anim_settings.at("rect_params_y").AsDouble());
    if (anim_settings.count("time_normalize") != 0) {
      time_normalize = anim_settings.at("time_normalize").AsFloat();
    }
    AnimationParams a_params = AnimationParams(
        anim_time, start_frame_x, start_frame_y, end_frame_x, end_frame_y,
        sf::Vector2i(rect_params_x, rect_params_y), time_normalize);
    AddAnimation(animation_key, a_params);
  }
}

bool GraphicsComponent::IsDone(const std::string &animation_key) const {
  return _animations.at(animation_key)->IsDone();
}

void GraphicsComponent::AddAnimation(const std::string &animation_key,
                                     AnimationParams a_params) {
  _animations[animation_key] =
      std::make_unique<Animation>(_sprite, _texture_sheet, a_params);
}

bool GraphicsComponent::Play(const std::string &animation_key,
                             const float time_elapsed,
                             const float speed_modifier, const bool priority) {
  if (_prior_animation != nullptr) {
    if (_animations.at(animation_key).get() == _prior_animation) {
      CheckLastAnimation(animation_key);
      if (_animations.at(animation_key)->Play(time_elapsed, speed_modifier)) {
        _prior_animation = nullptr;
      }
    }
  } else {
    if (priority) {
      _prior_animation = _animations.at(animation_key).get();
    }
    CheckLastAnimation(animation_key);
    _animations.at(animation_key)->Play(time_elapsed, speed_modifier);
  }
  return _animations.at(animation_key)->IsDone();
}

void GraphicsComponent::CheckLastAnimation(const std::string &animation_key) {
  if (_last_animation != _animations.at(animation_key).get()) {
    if (_last_animation == nullptr) {
      _last_animation = _animations.at(animation_key).get();
    } else {
      //ВНИМАНИЕ НА ПОРЯДОК
      _last_animation->Reset();
      _last_animation = _animations.at(animation_key).get();
    }
  }
}

void GraphicsComponent::Reset() {
  if (_last_animation != nullptr) {
    _last_animation->Reset();
    _last_animation = nullptr;
  }
  if (_prior_animation != nullptr) {
    _prior_animation->Reset();
    _prior_animation = nullptr;
  }
}

void GraphicsComponent::UpdateCopy(sf::Sprite &sprite) {
  _sprite = &sprite;
  for (auto &[key, animation] : _animations) {
    animation->sprite_ = &sprite;
  }
}

GraphicsComponent &
GraphicsComponent::operator=(const GraphicsComponent &other) {
  if (&other == this) {
    return *this;
  }
  _last_animation = nullptr;
  _prior_animation = nullptr;
  for (const auto &[key, animation] : other._animations) {
    _animations[key] = std::make_unique<Animation>(*animation);
  }
  return *this;
}

GraphicsComponent::GraphicsComponent(const GraphicsComponent &other)
    : _texture_sheet(other._texture_sheet), _sprite(other._sprite) {
  *this = other;
}

GraphicsComponent::AnimationParams::AnimationParams(
    float anim_time, int start_frame_x, int start_frame_y, int end_frame_x,
    int end_frame_y, sf::Vector2i rect_params, float time_normalize) {
  time_to_animate_ = anim_time;
  start_rect_ =
      sf::IntRect(start_frame_x * rect_params.x, start_frame_y * rect_params.y,
                  rect_params.x, rect_params.y);
  current_rect_ = start_rect_;
  end_rect_ =
      sf::IntRect(end_frame_x * rect_params.x, end_frame_y * rect_params.y,
                  rect_params.x, rect_params.y);
  rect_params_ = rect_params;
  time_normalize_ = time_normalize;
}

GraphicsComponent::Animation::Animation(sf::Sprite *sprite,
                                        const sf::Texture &texture_sheet,
                                        AnimationParams a_params)
    : sprite_(sprite), texture_sheet_(texture_sheet), a_params_(a_params) {

  sprite_->setTexture(texture_sheet_, true);
  sprite_->setTextureRect(a_params.start_rect_);
}

bool GraphicsComponent::Animation::Play(const float time_elapsed,
                                        const float speed_modifier) {
  a_params_.time_ += a_params_.time_normalize_ * time_elapsed * speed_modifier;
  is_done_ = false;
  if (a_params_.time_ >= a_params_.time_to_animate_) {
    a_params_.time_ = 0;
    Animate();
  }
  return is_done_;
}

void GraphicsComponent::Animation::Animate() {
  if (a_params_.current_rect_ != a_params_.end_rect_) {
    a_params_.current_rect_.left += a_params_.rect_params_.x;
  } else {
    a_params_.current_rect_.left = a_params_.start_rect_.left;
    is_done_ = true;
  }
  sprite_->setTextureRect(a_params_.current_rect_);
}

void GraphicsComponent::Animation::Reset() {
  a_params_.time_ = a_params_.time_to_animate_;
  a_params_.current_rect_ = a_params_.start_rect_;
}

bool GraphicsComponent::Animation::IsDone() const { return is_done_; }
