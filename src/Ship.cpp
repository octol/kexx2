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

Ship::Ship(std::string n, int energy, int score, sdlc::Surface& s, 
           std::unique_ptr<Weapon>& main, std::unique_ptr<Weapon>& extra, 
           KeySet keyset)
    : Object(n, energy, s, OBJ_PLAYER),
      main_weapon_(std::move(main)),
      extra_weapon_(std::move(extra)),
      keyset_(keyset)
{
    set_score(score);

    // Parse the player number from the name.
    int player_number = n[n.length() - 1] - (int)'0';
    set_owner((Owner)(OWNER_PLAYER1 + player_number - 1));

    set_locked_to_screen(false);
    set_active(true);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Ship::think(ObjectManager& object_manager, FxManager& fx_manager)
{
    UNUSED(fx_manager);

    // TODO: remove extern Input class.
    extern sdlc::Input* input;
    // TODO: remove extern Timer class
    extern sdlc::Timer* timer;

    if (invincible_ != INPUT_LOCKED) {
        process_input(*input, object_manager);
    } else {
        do_scripted_movement(*timer);
    }

    // blinking when hit
    if (invincible_ == BLINKING && timer->ticks() - blinking_timer_ > 50) {
        times_blinked_++;
        blinking_timer_ = timer->ticks();

        // flip SDL_Surface's
        // TODO: use Surface/BaseSurface instead.
        //SDL_Surface* tmp = data;
        //data = hit_img.data;
        //hit_img.data = tmp;
    }

    // check if empty clip
    if (extra_weapon_ && extra_weapon_->count() < 1) 
        extra_weapon_.reset();

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
    for (auto current : object_manager.list) {

        SDL_Rect ship_rect = reduced_rect();
        SDL_Rect object_rect = current->reduced_rect();

        if (current->energy() && sdlc::overlap(ship_rect, object_rect))
            // TODO: should we really pass *current instead of the
            // shared_ptr ?
            collide_with_object(*current, object_manager, fx_manager);
    }
}

void Ship::update(sdlc::Timer& timer)
{
    Object::update(timer);

    if (hit_timer > 0 && times_blinked_ > 100) 
        set_not_invincible();
}

void Ship::hurt(int value, ObjectManager& object_manager, FxManager& fx_manager)
{
    // TODO: remove extern Timer class.
    extern sdlc::Timer *timer;

    if (!invincible_ && hit_timer == 0) {
        Object::hurt(value, object_manager, fx_manager);
        if (energy())
            fx_manager.play_player_hit_snd();

        invincible_ = BLINKING;
        blinking_timer_ = timer->ticks();
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

void Ship::process_input(sdlc::Input& input, ObjectManager& object_manager)
{
    // reset movement velocity
    set_x_vel(0);
    set_y_vel(0);

    // check keyboard
    if (input.key_pressed(keyset_.left, sdlc::AUTOFIRE))
        set_x_vel(-100.0f);
    else if (input.key_pressed(keyset_.right, sdlc::AUTOFIRE))
        set_x_vel(100.0f);
    if (input.key_pressed(keyset_.up, sdlc::AUTOFIRE))
        set_y_vel(-100.0f);
    else if (input.key_pressed(keyset_.down, sdlc::AUTOFIRE))
        set_y_vel(100.0f);

    int sx = x() + width() / 2;
    int sy = y() + 10;

    if (input.key_pressed(keyset_.fire_main, sdlc::AUTOFIRE) && main_weapon_) 
        main_weapon_->shoot(sx, sy, object_manager);
    if (input.key_pressed(keyset_.fire_extra, sdlc::NO_AUTOFIRE) && extra_weapon_) 
        extra_weapon_->shoot(sx, sy, object_manager);
}

void Ship::do_scripted_movement(sdlc::Timer& timer)
{
    // do some scripted movement when entering/leaving a level
    if (!level_complete_) {
        set_y_vel(y_vel() + (130.0f * timer.frame_time()));

        if (y_vel() > -10.0f) {
            set_y_vel(0);
            invincible_ = 0;
            set_locked_to_screen(true);
        }
    } else if (level_complete_) {
        if (locked_to_screen()) {
            set_x_vel(0);
            set_y_vel(0);
            set_locked_to_screen(false);
        }

        set_y_vel(y_vel() - (130.0f * timer.frame_time()));
    }
}

void Ship::collide_with_object(IObject& current, ObjectManager& object_manager, 
                               FxManager& fx_manager) 
{
    switch (current.type()) {
    case OBJ_ENEMY:
        if (!invincible_) {
            // kill both objects
            kill(object_manager, fx_manager);
            current.kill(object_manager, fx_manager);
        }
        break;
    case OBJ_BONUS:
        // blaster
        if (current.name() == "Blaster Bonus" && 
                main_weapon_->name == "Blaster Weapon") {
            main_weapon_->upgrade();
        } else if (current.name() == "Blaster Bonus") {
            main_weapon_ = std::unique_ptr<Weapon>(new WeaponBlaster(
                        object_manager.snd[SND_SHOTBLASTER], owner()));
        }

        // mute removal of the bonus object
        current.set_energy(0); 
        break;
    default:
        break;
    }
}

void Ship::set_not_invincible() 
{
    hit_timer = 0;
    times_blinked_ = 0;
    blinking_timer_ = 0;
    invincible_ = 0;
}

void Ship::update_smoketrail(ObjectManager& object_manager)
{
    // TODO: remove extern timer class.
    extern sdlc::Timer* timer;

    // First we check if there already is a smoke trail
    bool smoke_found = false;
    for (auto obj : object_manager.list) {
        if (obj->name() == "Smoketrail" && obj->owner() == owner()) {
            smoke_found = true;
            break;
        }
    }

    // If not, then we create one
    if (!smoke_found) {
        float sx = x() + 60;
        float sy = y() + 60;
        object_manager.create_object((int)sx, (int)sy, 0, 0, SMOKETRAIL, owner());
        object_manager.create_object((int)(sx + 40), (int)sy, 0, 0, SMOKETRAIL, owner());
    }

    // Actually update the smokee trail
    int current_trail = 1;
    for (auto obj : object_manager.list) {
        if (obj->name() == "Smoketrail" && obj->owner() == owner()) {
            auto set_trail_position = [this,obj](int xshift) {
                obj->set_pos(x() + xshift, y() + height() - 2);
                obj->set_vel(x_vel(), y_vel());

                obj->update(*timer);
                obj->set_vel(0, 0);
            };

            if (current_trail == 1) {
                set_trail_position(13);
                current_trail++;
            } else if (current_trail == 2) {
                set_trail_position(24);
                break;
            }
        }
    }
}

void Ship::remove_smoketrail(ObjectManager& object_manager)
{
    for (auto obj : object_manager.list) {
        if (obj->name() == "Smoketrail" && obj->owner() == owner()) {
            obj->set_energy(0);
        }
    }
}

void Ship::calculate_hit_img()
{
    // TODO: remove raw SDL call.
    //hit_img.data = SDL_DisplayFormat(data);
    //hit_img.fill_rect(0, 0, hit_img.data->w, hit_img.data->h, 255, 0, 255);
}
