## For when libsdlc is included in source tree.
#AM_CPPFLAGS = -I../libsdlc

## For when libsdlc is installed on the system. This makes use of pkg-config.
AM_CPPFLAGS = $(LIBSDLC_CFLAGS)

bin_PROGRAMS = kexx2

kexx2_SOURCES = \
	BuyScreen.cpp BuyScreen.h \
	Defines.h \
	EnemyBonus.cpp EnemyBonus.h \
	EnemyRammer.cpp EnemyRammer.h \
	EnemySideways.cpp EnemySideways.h \
	Finished.cpp Finished.h \
	FxManager.cpp FxManager.h \
	Game.cpp Game.h \
	GameOver.cpp GameOver.h \
	IGameState.h \
	Interface.cpp Interface.h \
	LevelManager.cpp LevelManager.h \
	Main.cpp \
	Menu.cpp Menu.h \
	Object.cpp Object.h \
	ObjectManager.cpp ObjectManager.h \
	Options.cpp Options.h \
	ParticleManager.cpp ParticleManager.h \
	PlayerState.cpp PlayerState.h \
	Ship.cpp Ship.h \
	Shot.cpp Shot.h \
	ShotRocket.cpp ShotRocket.h \
	Starfield.cpp Starfield.h \
	Weapon.cpp Weapon.h \
	WeaponBlaster.cpp WeaponBlaster.h \
	WeaponRocket.cpp WeaponRocket.h \
	World.cpp World.h
	
## For when libsdlc is included in source tree.
#kexx2_LDADD = ../libsdlc/libsdlc-0.2.la

## For when libsdlc is installed on the system. This makes use of pkg-config.
kexx2_LDADD = $(LIBSDLC_LIBS)
