AM_CPPFLAGS = $(LIBSDLC_CFLAGS) $(BOOST_CPPFLAGS) -DDATADIR='"$(datadir)"'
AM_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_FILESYSTEM_LDFLAGS) $(BOOST_SYSTEM_LDFLAGS)
LDADD = $(LIBSDLC_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_SYSTEM_LIBS)

bin_PROGRAMS = kexx2

kexx2_SOURCES = \
	BuyScreen.cpp BuyScreen.h \
	Defines.h \
	EnemyBonus.cpp EnemyBonus.h \
	EnemyRammer.cpp EnemyRammer.h \
	EnemySideways.cpp EnemySideways.h \
	EnemyStandard.cpp EnemyStandard.h \
	EnumHash.h \
	Finished.cpp Finished.h \
	FxManager.cpp FxManager.h \
	Game.cpp Game.h \
	GameOver.cpp GameOver.h \
	IGameState.h \
	IObject.h \
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
	
