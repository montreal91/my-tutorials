
#ifndef __CATEGORY_HPP__
#define __CATEGORY_HPP__

namespace Category {
enum Type {
    None                = 0,
    SceneAirLayer       = 1 << 0,
    PlayerAircraft      = 1 << 1,
    AlliedAircraft      = 1 << 2,
    EnemyAircraft       = 1 << 3,
    Pickup              = 1 << 4,
    AlliedProjectile    = 1 << 5,
    EnemyProjectile     = 1 << 6,

    Aircraft            = PlayerAircraft | AlliedAircraft | EnemyAircraft,
    Projectile          = AlliedProjectile | EnemyProjectile,
};
} // namespace Category

#endif // __CATEGORY_HPP__
