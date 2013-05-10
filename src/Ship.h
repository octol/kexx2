//    Kexx2
//    Copyright (C) 2001, 2013 Jon Häggblad
//
//    This file is part of Kexx2.
//
//    Kexx2 is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Kexx2 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Kexx2.  If not, see <http://www.gnu.org/licenses/>.

#ifndef KEXX2_SHIP_H
#define KEXX2_SHIP_H

#include <list>
#include <memory>
#include "SDLc.h"
#include "Object.h"
#include "PlayerState.h"

class Weapon;

const int BLINKING = 1;
const int INPUT_LOCKED = 2;

class Ship final : public Object {
    friend class ObjectManager;
public:
    Ship(std::string n, int energy, int score, sdlc::Surface& s, sdlc::Surface& hit_s,
         std::unique_ptr<Weapon>& main, std::unique_ptr<Weapon>& extra, 
         KeySet keyset);
    virtual ~Ship() = default;

    virtual void think(ObjectManager&, FxManager&) override;
    virtual void check_collisions(ObjectManager&, FxManager&) override;
    virtual void update(sdlc::Timer& timer) override;
    virtual void hurt(int value, ObjectManager&, FxManager&) override;
    virtual void kill(ObjectManager&, FxManager&) override;

    virtual const sdlc::Sprite& sprite() const override;

    static sdlc::Surface create_hit_img(const sdlc::Surface&);

private:
    void process_input(sdlc::Input&, ObjectManager&);
    void do_scripted_movement(sdlc::Timer&);

    void collide_with_object(std::shared_ptr<IObject>& current, 
                             ObjectManager&, FxManager&);

    void set_not_invincible();

    void update_smoketrail(ObjectManager&);
    void remove_smoketrail(ObjectManager&);

    std::unique_ptr<Weapon> main_weapon_;
    std::unique_ptr<Weapon> extra_weapon_;
    KeySet keyset_;

    int invincible_ = INPUT_LOCKED;
    bool level_complete_ = false;
    int blinking_timer_ = 0;
    int times_blinked_ = 0;
};

#endif // KEXX2_SHIP_H
