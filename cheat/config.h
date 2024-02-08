namespace config {
    namespace triggerbot {
        bool enable = false;
    }
    namespace aimbot {
        enum AimType : int {
            DISTANCE_TO_CROSSHAIR = 0,
            DISTANCE_TO_PLAYER = 1
        };
        enum AimBone : int {
            HEAD = 0,
            NECK = 1,
            CHEST = 2,
            RANDOM = 3
        };

        bool enable = false;
        bool showFov = false;
        float fov = 100;
        float smoothing = 1;
        bool predictPosition = true;
        bool showPredictedPosition = true;
        int type = DISTANCE_TO_CROSSHAIR;
        int target = HEAD;
    }
    namespace esp {
        bool enable = false;
        bool showSnapLines;
        namespace players {
            bool show;
            bool showDistance;
            bool showBots;
            bool showTeam;
            bool showName;
            bool showWeapon;
            bool showRemainAmmo;
        }
        namespace items {
            bool show;
            bool showDistance;
            bool showName;
            int qualityFilter;
        }
    }

}