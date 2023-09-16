#ifndef STARTSCENE_HPP
#define STARTSCENE_HPP
#include "IScene.hpp"
#include <memory>
#include <allegro5/allegro_audio.h>

class StartScene final : public Engine::IScene {
private:
		std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit StartScene() = default;
    void Initialize() override;
    void BackOnClick(int stage);
};

#endif // STARTSCENE_HPP
