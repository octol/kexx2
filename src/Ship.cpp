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

#include "Ship.h"
#include "SDLc.h"
#include "ObjectManager.h"
#include "WeaponBlaster.h"
#include "WeaponRocket.h"
#include "FxManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Ship::Ship(std::string n, int energy, int score, Surface& s, 
           Weapon* main, Weapon* extra, KeySet keyset)
    : Object(n, energy, s, OBJ_PLAYER),
      main_weapon_(main),
      extra_weapon_(extra),
      keyset_(keyset)
{
    set_score(score);

    // TODO: should redo this.
    int tmp = n[n.length() - 1] - (int)'0';
    set_owner((Owner)(OWNER_PLAYER1 + tmp - 1));

    lockedToScreen(false);
    active(true);

}

Ship::~Ship()
{
    delete main_weapon_;
    delete extra_weapon_;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Ship::think(ObjectManager& object_manager, FxManager& fx_manager)
{
    // TODO: remove extern Input class.
    extern sdlc::Input* input;

    // reset movement velocity (if not input locked)
    if (invincible_ != INPUT_LOCKED) {
        setXVel(0);
        setYVel(0);

        // check keyboard
        if (input->keyPressed(keyset_.left, sdlc::AUTOFIRE))
            setXVel(-100.0f/*-0.1f*/);
        else if (input->keyPressed(keyset_.right, sdlc::AUTOFIRE))
            setXVel(100.0f);
        if (input->keyPressed(keyset_.up, sdlc::AUTOFIRE))
            setYVel(-100.0f);
        else if (input->keyPressed(keyset_.down, sdlc::AUTOFIRE))
            setYVel(100.0f);

        if (input->keyPressed(keyset_.fire_main, sdlc::AUTOFIRE) 
                && main_weapon_) {
            main_weapon_->shoot((int)(getX() + (getWidth() / 2)), 
                               (int)(getY() + 10), object_manager);
        }
        if (input->keyPressed(keyset_.fire_extra, sdlc::NO_AUTOFIRE) 
                && extra_weapon_) {
            extra_weapon_->shoot((int)(getX() + (getWidth() / 2)), 
                                (int)(getY() + 10), object_manager);
        }
    } else {
        // do some scripted movement when entering/leaving a level
        if (!level_complete_) {
            // TODO: remove extern Timer class
            extern sdlc::Timer* timer;
            setYVel(getYVel() + (130.0f * timer->frame_time()));

            if (getYVel() > -10.0f /*-0.0100f*/) {
                setYVel(0);
                invincible_ = 0;
                lockedToScreen(true);
            }
        } else if (level_complete_) {
            if (lockedToScreen()) {
                setXVel(0);
                setYVel(0);
                lockedToScreen(false);
            }

            // TODO: remove extern Timer class
            extern sdlc::Timer* timer;
            setYVel(getYVel() - (130.0f * timer->frame_time()));
        }
    }

    // blinking when hit
    if (invincible_ == BLINKING && SDL_GetTicks() - blinking_timer_ > 50) {
        times_blinked_++;
        blinking_timer_ = SDL_GetTicks();

        // flip SDL_Surface's
        SDL_Surface* tmp = data;
        data = hit_img.data;
        hit_img.data = tmp;
    }

    // check if empty clip
    if (extra_weapon_ && extra_weapon_->count() < 1) {
        delete extra_weapon_;
        extra_weapon_ = 0;
    }

    // smoke trail
    update_smoketrail(object_manager);

    // if level complete
    if (object_manager.num_of_enemies() == 0) {
        invincible_ = INPUT_LOCKED;
        level_complete_ = true;
    }
}

void Ship::check_collisions(ObjectManager& object_manager, FxManager& fx_manager)
{
    // check collisions  player <-> objects
    ObjectList::iterator i = object_manager.list.begin();
    for (; i != object_manager.list.end(); i++) {
        Object* current = *i;

        SDL_Rect tmp1 = getReducedRect();
        SDL_Rect tmp2 = current->getReducedRect();
        if (current->type() == OBJ_ENEMY && current->energy() && \
                overlap(tmp1, tmp2) && !invincible_) {
            // kill both objects
            kill(object_manager, fx_manager);
            current->kill(object_manager, fx_manager);
        } else if (current->type() == OBJ_BONUS && current->energy() && \
                   overlap(tmp1, tmp2)) {
            // blaster
            if (current->name == "Blaster Bonus" && \
                    main_weapon_->name == "Blaster Weapon")
                main_weapon_->upgrade();
            else if (current->name == "Blaster Bonus")
                set_main_weapon(new WeaponBlaster(
                            object_manager.snd[SND_SHOTBLASTER], owner()));

            current->set_energy(0); // want mute removal
        }
    }
}

void Ship::update(sdlc::Timer& timer)
{
    Sprite::update(timer);
    if (hit_timer > 0 && times_blinked_ > 100) {
        hit_timer = 0;
        times_blinked_ = 0;
        blinking_timer_ = 0;
        invincible_ = 0;
    }
}

void Ship::hurt(int value, ObjectManager& object_manager, FxManager& fx_manager)
{
    if (!invincible_ && hit_timer == 0) {
        Object::hurt(value, object_manager, fx_manager);
        if (energy())
            fx_manager.play_player_hit_snd();

        invincible_ = BLINKING;
        // TODO: replace with call to Timer class.
        blinking_timer_ = SDL_GetTicks();
    }
}

void Ship::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    if (!invincible_) {
        Object::kill(object_manager, fx_manager);
        remove_smoketrail(object_manager);
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Ship::set_main_weapon(Weapon* w)
{
    delete main_weapon_;
    main_weapon_ = w;
}

void Ship::set_extra_weapon(Weapon* w)
{
    delete extra_weapon_;
    extra_weapon_ = w;
}

void Ship::update_smoketrail(ObjectManager& object_manager)
{
    bool smoke_found = false;
    // TODO: Change to range based loop
    ObjectList::iterator i = object_manager.list.begin();
    for (; i != object_manager.list.end(); i++) {
        Object* obj = *i;
        if (obj->name == "Smoketrail" && obj->owner() == owner()) {
            smoke_found = true;
            break;
        }
    }
    if (!smoke_found) {
        float x = getX() + 60;
        float y = getY() + 60;
        object_manager.create_object((int)x, (int)y, 0, 0, SMOKETRAIL, owner());
        object_manager.create_object((int)(x + 40), (int)y, 0, 0, SMOKETRAIL, owner());
    }

    int current = 1;
    // TODO: Change to range based loop
    i = object_manager.list.begin();
    for (; i != object_manager.list.end(); i++) {
        Object* obj = *i;
        if (obj->name == "Smoketrail" && obj->owner() == owner()) {
            if (current == 1) {
                obj->setPos(getX() + 13, getY() + getHeight() - 2);
                obj->setVel(getXVel(), getYVel());

                extern sdlc::Timer* timer;
                obj->update(*timer);
                obj->setVel(0, 0);
                current++;
            } else if (current == 2) {
                obj->setPos(getX() + 24, getY() + getHeight() - 2);
                obj->setVel(getXVel(), getYVel());

                extern sdlc::Timer* timer;
                obj->update(*timer);
                obj->setVel(0, 0);
                break;
            }
        }
    }
}

void Ship::remove_smoketrail(ObjectManager& object_manager)
{
    int current = 1;
    // TODO: change to range based for loop
    ObjectList::iterator i = object_manager.list.begin();
    for (; i != object_manager.list.end(); i++) {
        Object* obj = *i;
        if (obj->name == "Smoketrail" && obj->owner() == owner()) {
            if (current == 1)
                obj->set_energy(0);
            else if (current == 2) {
                obj->set_energy(0);
                break;
            }
        }
    }
}

void Ship::calculate_hit_img()
{
    // TODO: remove raw SDL call.
    hit_img.data = SDL_DisplayFormat(data);
    hit_img.fillRect(0, 0, hit_img.data->w, hit_img.data->h, 255, 0, 255);
}
