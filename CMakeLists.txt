cmake_minimum_required(VERSION 3.5)

find_package(PkgConfig REQUIRED)

PKG_CHECK_MODULES( ALLEGRO-5 REQUIRED
	allegro-5 allegro_acodec-5 
	allegro_audio-5 allegro_font-5
	allegro_image-5 allegro_primitives-5
	allegro_ttf-5 allegro_video-5)

set (INCLUDE
	src/inc/*.hpp
	src/inc/*.h
	)

set (SOURCES
	src/Allegro5Exception.cpp
	src/AudioHelper.cpp
	src/Bullet.cpp
	src/Collider.cpp
	src/DirtyEffect.cpp
	src/Enemy.cpp
	src/InPlaySceneEffect.cpp
	src/GameEngine.cpp
	src/Group.cpp
	src/IControl.cpp
	src/ImageButton.cpp
	src/Image.cpp
	src/IObject.cpp
	src/IScene.cpp
	src/Label.cpp
	src/LOG.cpp
	src/LoseScene.cpp
	src/main.cpp
	src/Plane.cpp
	src/PlayScene.cpp
	src/PlugGunTurret.cpp
	src/Point.cpp
	src/RedNormalEnemy.cpp
	src/Resources.cpp
	src/Slider.cpp
	src/Sprite.cpp
	src/StageSelectScene.cpp
	src/StartScene.cpp
	src/FLPlayScene.cpp
	src/TurretButton.cpp
	src/Turret.cpp
	src/WinScene.cpp
	src/WoodBullet.cpp
	src/SettingScene.cpp
	src/ImADice.cpp
	src/Turret_MG.cpp
	src/DiceDuper.cpp
	src/MGBullet.cpp
	src/PlusMGBullet.cpp
	src/PlusMGTurret.cpp
	src/Shovel.cpp
	src/Mover.cpp
	src/CyclerTurret.cpp
	src/RotationBullet.cpp
	src/ModSel.cpp
	src/GenArg.cpp
	src/Obstacler.cpp
	src/Obstacles.cpp
)
	
set (CMAKE_CXX_FLAGS
	"-Wall --std=c++17"
	)

project(TowerDefense)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(TowerDefense ${ALLEGRO-5_LIBRARIES})
target_include_directories(TowerDefense PUBLIC ${ALLEGRO-5_INCLUDE_DIRS} src/inc)
