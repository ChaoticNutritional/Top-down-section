#include "Character.h"
#include "raylib.h"

Character::Character(int windowWidth, int windowHeight) : winWidth(windowWidth),
                                                          winHeight(windowHeight)
{
    width = currentTexture.width / maxFrames;
    height = currentTexture.height;
    speed = 5.f;
}


Vector2 Character::GetScreenPos()
{
    return Vector2{
        static_cast<float>(winWidth) / 2 - (scale * .5f * width),
        static_cast<float>(winHeight) / 2 - (scale * .5f * height)};
}

void Character::Tick(float deltaTime)
{
    // check if dead
    if (!getAlive()) return;

    // Check for input
    if (IsKeyDown(KEY_A))
        velocity.x -= 1.0;
    if (IsKeyDown(KEY_D))
        velocity.x += 1.0;
    if (IsKeyDown(KEY_W))
        velocity.y -= 1.0;
    if (IsKeyDown(KEY_S))
        velocity.y += 1.0;
    BaseCharacter::Tick(deltaTime);

    GetMousePosition().x < GetScreenPos().x + static_cast<float>(currentTexture.width) / 2.f ? RightLeft = -1.f : RightLeft = 1.f;

    // Debugging mouse pos
    //DrawCircle(GetMousePosition().x, GetMousePosition().y, 10.f, RED);

    Vector2 origin{0.f, 0.f};
    Vector2 offset{0.f, 0.f};

    // rotation for sword init
    float rotation{};

    if (RightLeft > 0.f)
    {
        origin = {0.f, weapon.height * scale};
        offset = {35.f, 55.f};
        weaponCollisionRec = {
            GetScreenPos().x + offset.x,
            GetScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale,
        };

        // check if clicking to attack
        IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? rotation = 35.f : rotation = 0.f;
    }
    else
    {
        origin = {weapon.width * scale, weapon.height * scale};
        offset = {25.f, 55.f};
        weaponCollisionRec = {
            GetScreenPos().x + offset.x - weapon.width * scale,
            GetScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale,
        };

        // check if clicking to attack
        IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? rotation = -35.f : rotation = 0.f;
    }

    // draw the sword
    Rectangle source{
        0.f,
        0.f,
        static_cast<float>(weapon.width) * RightLeft,
        static_cast<float>(weapon.height)};

    Rectangle dest{
        GetScreenPos().x + offset.x,
        GetScreenPos().y + offset.y,
        weapon.width * scale,
        weapon.height * scale};

    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);

    /*DrawRectangleLines(
        weaponCollisionRec.x,
        weaponCollisionRec.y,
        weaponCollisionRec.width,
        weaponCollisionRec.height,
        RED);*/

    // debugging knight's position on the screen displayed
    // DrawText(TextFormat("pos: %f %f", worldPos.x, worldPos.y), 0.f, 0.f, 33.f, RED);
}

void Character::TakeDamage(float damage, float deltaTime = 0.0f)
{
    damaged = true;
    health -= damage;

    if(damaged)
    {
        currentColor = normalColor;
        damaged = false;
    }

    if (health <= 0.f)
    {
        setAlive(false);
    }
}